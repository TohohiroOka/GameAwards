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
	safe_delete(playerModel);
	safe_delete(pBullModel);
	safe_delete(enemy01Model);
	safe_delete(eBullModel);
	safe_delete(deadEnemyModel);

	//プレイヤー解放
	safe_delete(player);

	//プレイヤー弾解放
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}
	//敵解放
	for (int i = 0; i < enemyNum; i++)
	{
		safe_delete(enemy[i]);
	}
	//敵の弾解放
	for (int i = 0; i < enemyBulletNum; i++)
	{
		safe_delete(enemyBullet[i]);
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

void GameScene::Initialize(Camera *camera)
{
	// nullptrチェック
	assert(camera);

	//ライト
	light = LightGroup::Create();
	light->DefaultLightSetting();
	Object3d::SetLightGroup(light);

	circleModel = Model::CreateFromOBJ("circle");//タバコのモデル
	playerModel = Model::CreateFromOBJ("player");//プレイヤーのモデル
	pBullModel = Model::CreateFromOBJ("playerbullet");//プレイヤーの弾のモデル
	enemy01Model = Model::CreateFromOBJ("enemy");//敵01のモデル
	eBullModel = Model::CreateFromOBJ("enemybullet");//敵の弾のモデル
	deadEnemyModel = Model::CreateFromOBJ("desenemy");//死んだ敵のモデル

	//プレイヤー生成
	player = Player::Create(playerModel);

	//弾生成
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(pBullModel);
	}

	//敵生成
	enemy[0] = Garuta::Create(enemy01Model, { 0, 20, 0 });
	enemy[1] = Garuta::Create(enemy01Model, { 0, 30, 0 });
	enemy[2] = Garuta::Create(enemy01Model, { 0, 40, 0 });
	enemy[3] = Garuta::Create(enemy01Model, { 10, 20, 0 });
	enemy[4] = Garuta::Create(enemy01Model, { 10, 30, 0 });
	enemy[5] = Garuta::Create(enemy01Model, { 10, 40, 0 });
	enemy[6] = Garuta::Create(enemy01Model, { 20, 20, 0 });
	enemy[7] = Garuta::Create(enemy01Model, { 20, 30, 0 });
	enemy[8] = Garuta::Create(enemy01Model, { 20, 40, 0 });

	//敵の弾生成
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

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
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//プレイヤー更新
	player->Update();

	//デバッグ用線の色
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		(*itr)->SetColor({ 0.4f, 1, 0.2f, 1 });
	}

	//プレイヤー弾発射
	if (input->TriggerKey(DIK_SPACE) || Xinput->TriggerButton(XInputManager::PAD_RB))
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

	//プレイヤー弾更新
	for (int i = 0; i < playerBulletNum; i++)
	{
		//弾が生きていなければ飛ばす
		if (!playerBullet[i]->GetIsAlive()) { continue; }

		//更新処理
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
				DeadEnemyPoint::Create(deadEnemyModel, deadPoint, radius));

			//敵の存在がなくなったので飛ばす
			continue;
		}

		//敵が生きていなければ飛ばす
		if (!enemy[j]->GetIsAlive()) { continue; }

		//敵弾発射フラグがtrueなら
		if (enemy[j]->GetIsBulletShot())
		{
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//敵弾が発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//敵の座標と標的の座標（プレイヤーの座標）を弾も持つ
				XMFLOAT3 startPos = enemy[j]->GetPosition();
				XMFLOAT3 targetPos = player->GetPosition();

				//敵弾発射
				enemyBullet[i]->BulletStart(startPos, targetPos);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
		}

		//プレイヤーと敵の当たり判定
		//プレイヤーがダメージ状態でないなら判定をする
		if (!player->GetIsDamege())
		{
			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 enemyPos = enemy[j]->GetPosition();
			float enemySize = enemy[j]->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//敵とプレイヤー弾が衝突状態
			if (isCollision)
			{
				//プレイヤーはダメージを喰らう
				player->Damage();

				//HPが0なら
				if (player->GetHP() <= 0)
				{
					//プレイヤー死亡
					player->Dead();
				}
			}
		}

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

	//敵の弾更新
	for (int i = 0; i < enemyBulletNum; i++)
	{
		//弾が生きていなければ飛ばす
		if (!enemyBullet[i]->GetIsAlive()) { continue; }

		//更新処理
		enemyBullet[i]->Update();

		//プレイヤーがダメージ状態だったら飛ばす
		if (player->GetIsDamege()) { continue; };

		//プレイヤーと敵の弾の当たり判定
		//衝突用に座標と半径の大きさを借りる
		XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
		float bulletSize = enemyBullet[i]->GetScale().x;
		XMFLOAT3 playerPos = player->GetPosition();
		float playerSize = player->GetScale().x;

		//衝突判定を計算
		bool isCollision = Collision::CheckCircle2Circle(
			bulletPos, bulletSize, playerPos, playerSize);

		//敵とプレイヤー弾が衝突状態でなければ飛ばす
		if (!isCollision) { continue; }

		//敵弾は死亡
		enemyBullet[i]->Dead();
		//プレイヤーはダメージを喰らう
		player->Damage();

		//ダメージを喰らってもHPが残っていたら飛ばす
		if (player->GetHP() > 0) { continue; }

		//プレイヤー死亡
		player->Dead();
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

	
	if (player->GetHP() == 3) { DebugText::GetInstance()->Print("HP : 3", 100, 500); }
	else if (player->GetHP() == 2) { DebugText::GetInstance()->Print("HP : 2", 100, 500); }
	else if (player->GetHP() == 1) { DebugText::GetInstance()->Print("HP : 1", 100, 500); }
	else if (player->GetHP() == 0) { DebugText::GetInstance()->Print("HP : 0", 100, 500); }

	if (player->GetIsAlive()) { DebugText::GetInstance()->Print("PLAYER ALIVE", 100, 550); }
	else { DebugText::GetInstance()->Print("PLAYER DEAD", 100, 550); }

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

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 1000, 100);
	DebugText::GetInstance()->Print("RSTICK:BulletAngle", 1000, 150);
	DebugText::GetInstance()->Print("LB:Sneak", 1000, 200);
	DebugText::GetInstance()->Print("RB:BulletShot", 1000, 250);
}

void GameScene::Draw(ID3D12GraphicsCommandList *cmdList)
{
	//スプライト背面描画
	Sprite::PreDraw(cmdList);

	sprite->Draw();
	Sprite::PostDraw();

	//オブジェクト描画
	Object3d::PreDraw(cmdList);

	//プレイヤー描画
	player->Draw();

	Object3d::PostDraw();

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
	//player->Draw();

	//プレイヤー弾描画
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Draw();
	}
	//敵描画
	for (int i = 0; i < enemyNum; i++)
	{
		enemy[i]->Draw();
	}
	//敵の弾描画
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i]->Draw();
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
