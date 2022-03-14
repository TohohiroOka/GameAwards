#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"


const float radian = XM_PI / 180.0f;//ラジアン

GameScene::~GameScene()
{
	//newオブジェクトの破棄
	safe_delete(audio);
	safe_delete(light);
	safe_delete(sprite);

	//モデル解放
	safe_delete(circleModel);

	//プレイヤー解放
	safe_delete(player);

	//弾解放
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}
	//敵解放
	for (int i = 0; i < enemyNum; i++)
	{
		safe_delete(enemy[i]);
	}
	//死んだ敵の位置解放
	for (auto itr = deadEnemyPoints.begin(); itr != deadEnemyPoints.end(); itr++)
	{
		safe_delete(*itr);
	}
	//死んだ敵の位置のリスト解放
	deadEnemyPoints.clear();

	//パワーアップ線解放
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		safe_delete(*itr);
	}
	//パワーアップ線のリスト解放
	powerUpLines.clear();
}

void GameScene::Initialize(Input *input, Camera *camera)
{
	// nullptrチェック
	assert(input);
	assert(camera);
	this->input = input;

	//ライト
	light = LightGroup::Create();
	light->DefaultLightSetting();
	Object3d::SetLightGroup(light);

	circleModel = Model::CreateFromOBJ("circle");

	//プレイヤー生成
	player = Player::Create(circleModel);

	//弾生成
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(circleModel);
	}

	//敵生成
	enemy[0] = Zakorin::Create(circleModel, { 0, 0, 0 });
	enemy[1] = Zakorin::Create(circleModel, { 0, 10, 0 });
	enemy[2] = Zakorin::Create(circleModel, { 0, 20, 0 });
	enemy[3] = Zakorin::Create(circleModel, { 10, 0, 0 });
	enemy[4] = Zakorin::Create(circleModel, { 10, 10, 0 });
	enemy[5] = Zakorin::Create(circleModel, { 10, 20, 0 });
	enemy[6] = Zakorin::Create(circleModel, { 20, 0, 0 });
	enemy[7] = Zakorin::Create(circleModel, { 20, 10, 0 });
	enemy[8] = Zakorin::Create(circleModel, { 20, 20, 0 });


	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/amm.jpg");

	//スプライト生成
	sprite = Sprite::Create(1);
	sprite->SetPosition({ 100, 100 });
	sprite->SetSize({ 100, 100 });
	sprite->SetTexSize({ 1020, 965 });

	//デバッグテキスト生成
	DebugText::GetInstance()->Initialize(0);

	//サウンド用
	audio = new Audio();
}

void GameScene::Update(Camera *camera)
{
	//プレイヤー更新
	player->Update();

	//デバッグ用線の色
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		(*itr)->SetColor({ 0.4f, 1, 0.2f, 1 });
	}

	//弾発射
	if (input->TriggerKey(DIK_SPACE) || input->TriggerGamePadButton(Input::PAD_RB))
	{
		for (int i = 0; i < playerBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (playerBullet[i]->GetIsAlive()) { continue; }

			//プレイヤーの座標と角度を弾も持つ
			XMFLOAT3 pos = player->GetPosition();
			XMFLOAT3 rota = player->GetRotation();

			//弾発射
			playerBullet[i]->BulletStart(pos, rota);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}
	}

	//弾更新
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Update();

		//弾が生きていなければ飛ばす
		if (!playerBullet[i]->GetIsAlive()) { continue; }

		//弾とパワーアップ線の当たり判定
		for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
		{
			//衝突用に弾の座標と半径、線の始点と終点を借りる
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletRadius = playerBullet[i]->GetScale().x;
			XMFLOAT3 lineStartPoint = (*itr)->GetStartPoint();
			XMFLOAT3 lineEndPoint = (*itr)->GetEndPoint();

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Line(
				bulletPos, bulletRadius, lineStartPoint, lineEndPoint);

			//弾と線が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//デバッグ用線の色変更
			(*itr)->SetColor({ 1, 0, 0, 1 });

			//既に衝突したことがあるか確認(衝突中パワーアップし続けてしまうため)
			if (playerBullet[i]->IsKnowLine((*itr))) { continue; }

			//弾をパワーアップさせる
			playerBullet[i]->PowerUp();
		}
	}

	//敵更新
	for (int j = 0; j < enemyNum; j++)
	{
		//存在がなかったら飛ばす
		if (!enemy[j]->GetIsExistence()) { continue; }

		//更新処理
		enemy[j]->Update();

		//ノックバックが終わり、存在がなくなったら
		if (!enemy[j]->GetIsExistence())
		{
			//死んだ敵の位置を増やす
			XMFLOAT3 deadPoint = enemy[j]->GetPosition();

			//死んだ敵の円の半径をセットする（ 敵の大きさ×（ 倒された時の弾の強さ / 5 ））
			float radius = enemy[j]->GetScale().x * ((float)enemy[j]->GetKillBulletPower() / 5);
			deadEnemyPoints.push_back(
				DeadEnemyPoint::Create(circleModel, deadPoint, radius));

			//敵の存在がなくなったので飛ばす
			continue;
		}

		//敵が生きていなければ飛ばす
		if (!enemy[j]->GetIsAlive()) { continue; }

		//弾と敵の当たり判定
		for (int i = 0; i < playerBulletNum; i++)
		{
			//弾が発射状態でなければ飛ばす
			if (!playerBullet[i]->GetIsAlive()) { continue; }

			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetScale().x;
			XMFLOAT3 enemyPos = enemy[j]->GetPosition();
			float enemySize = enemy[j]->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//敵と弾が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//弾は死亡
			playerBullet[i]->Dead();

			//敵はダメージを喰らう
			int bulletPower = playerBullet[i]->GetPower();
			enemy[j]->Damage(bulletPower);

			//ダメージを喰らってもHPが残っていたら飛ばす
			if (enemy[j]->GetHP() > 0) { continue; }

			//敵のHPが0以下なのでノックバックを開始する
			float bulletAngle = playerBullet[i]->GetAngle();
			enemy[j]->SetKnockBack(bulletAngle, bulletPower);
		}
	}

	//死んだ敵の位置更新
	for (auto itr = deadEnemyPoints.begin(); itr != deadEnemyPoints.end(); itr++)
	{
		//サイズ変更状態でない場合は飛ばす
		if (!(*itr)->GetIsChangeRadius()) { continue; }

		//更新
		(*itr)->Update();

		//衝突を判定してパワーアップ線を作成
		for (auto itr2 = deadEnemyPoints.begin(); itr2 != deadEnemyPoints.end(); itr2++)
		{
			CreatePowerUpLine(*itr, *itr2);
		}
	}

	//パワーアップ線更新
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		(*itr)->Update(camera);
	}

	//カメラセット
	camera->UpdateTps({ 0,0,-100 });

	//スプライト更新
	sprite->Update();

	if (powerUpLines.size() == 0) { DebugText::GetInstance()->Print("LINE : 0", 100, 600); }
	else if (powerUpLines.size() == 1) { DebugText::GetInstance()->Print("LINE : 1", 100, 600); }
	else if (powerUpLines.size() == 2) { DebugText::GetInstance()->Print("LINE : 2", 100, 600); }
	else if (powerUpLines.size() == 3) { DebugText::GetInstance()->Print("LINE : 3", 100, 600); }
	else if (powerUpLines.size() == 4) { DebugText::GetInstance()->Print("LINE : 4", 100, 600); }
	else if (powerUpLines.size() == 5) { DebugText::GetInstance()->Print("LINE : 5", 100, 600); }
	else if (powerUpLines.size() == 6) { DebugText::GetInstance()->Print("LINE : 6", 100, 600); }
	else if (powerUpLines.size() == 7) { DebugText::GetInstance()->Print("LINE : 7", 100, 600); }
	else if (powerUpLines.size() == 8) { DebugText::GetInstance()->Print("LINE : 8", 100, 600); }
	else if (powerUpLines.size() == 9) { DebugText::GetInstance()->Print("LINE : 9", 100, 600); }

	DebugText::GetInstance()->Print("PlayerMove:LSTICK", 1000, 100);
	DebugText::GetInstance()->Print("BulletAngle:RSTICK", 1000, 150);
	DebugText::GetInstance()->Print("BulletShot:RB", 1000, 200);
}

void GameScene::Draw(ID3D12GraphicsCommandList *cmdList)
{
	//スプライト背面描画
	Sprite::PreDraw(cmdList);

	sprite->Draw();
	Sprite::PostDraw();

	//線3d
	DrawLine3D::PreDraw(cmdList);

	//パワーアップ線描画
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		(*itr)->Draw();
	}

	DrawLine3D::PostDraw();

	//オブジェクト描画
	Object3d::PreDraw(cmdList);

	//プレイヤー描画
	player->Draw();

	//弾描画
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Draw();
	}
	//敵描画
	for (int i = 0; i < enemyNum; i++)
	{
		enemy[i]->Draw();
	}
	//死んだ敵の位置描画
	for (auto itr = deadEnemyPoints.begin(); itr != deadEnemyPoints.end(); itr++)
	{
		(*itr)->Draw();
	}

	Object3d::PostDraw();


	//スプライト前面描画
	Sprite::PreDraw(cmdList);

	//sprite->Draw();


	//デバッグテキスト描画
	DebugText::GetInstance()->DrawAll(cmdList);
	Sprite::PostDraw();
}

void GameScene::CreatePowerUpLine(DeadEnemyPoint *startPoint, DeadEnemyPoint *endPoint)
{
	//始点と終点が同じ場合は飛ばす(始点と終点が同じ位置の線が出来てしまうため)
	if (startPoint == endPoint) { return; }

	//既に線が引かれていたら飛ばす
	for (auto line = powerUpLines.begin(); line != powerUpLines.end(); line++)
	{
		if ((*line)->IsAlreadyCreateLine(startPoint, endPoint)) { return; }
	}

	//衝突用に円1と円2の座標と半径の大きさを借りる
	XMFLOAT3 deadPoint1 = startPoint->GetPosition();
	float deadRadius1 = startPoint->GetRadius();
	XMFLOAT3 deadPoint2 = endPoint->GetPosition();
	float deadRadius2 = endPoint->GetRadius();

	//衝突判定を計算
	bool isCollision = Collision::CheckCircle2Circle(
		deadPoint1, deadRadius1, deadPoint2, deadRadius2);

	//円1と円2が衝突状態でなければ飛ばす
	if (!isCollision) { return; }

	//衝突している円1と円2を繋ぐ新しい線を作る
	powerUpLines.push_back(PowerUpLine::Create(
		startPoint, endPoint));

	//繋がれた線の始点と終点の円をサイズ変更状態にする
	startPoint->ChangeRadius();
	endPoint->ChangeRadius();
}
