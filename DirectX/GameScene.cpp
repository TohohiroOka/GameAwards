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
	/*XMFLOAT3 pos = { 10, 30, 0 };
	for (int i = 0; i < enemyNum; i++)
	{
		pos.x += 15;
	}*/
	enemy[0] = Zakorin::Create(circleModel, { 0, 30, 0 });
	enemy[1] = Zakorin::Create(circleModel, { 15, 30, 0 });
	enemy[2] = Zakorin::Create(circleModel, { 0, 15, 0 });
	enemy[3] = Zakorin::Create(circleModel, { 15, 15, 0 });


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
			//発射されていない弾を探す
			if (!playerBullet[i]->GetIsAlive())
			{
				//プレイヤーの座標と角度を弾も持つ
				XMFLOAT3 pos = player->GetPosition();
				XMFLOAT3 rota = player->GetRotation();

				//弾発射
				playerBullet[i]->BulletStart(pos, rota);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
		}
	}
	//弾更新
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Update();

		//弾が生きていなければ飛ばす
		if (!playerBullet[i]->GetIsAlive()) continue;

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
			if (!isCollision) continue;

			//デバッグ用線の色変更
			(*itr)->SetColor({ 1, 0, 0, 1 });

			//既に衝突したことがあるか確認(衝突中パワーアップし続けてしまうため)
			if (playerBullet[i]->IsKnowLine((*itr))) continue;

			//弾をパワーアップさせる
			playerBullet[i]->PowerUp();
		}
	}

	//1フレーム前の死んだ敵の数を更新
	oldDeadPointNum = deadEnemyPoints.size();
	//敵更新
	for (int j = 0; j < enemyNum; j++)
	{
		enemy[j]->Update();

		//敵が生きていなければ飛ばす
		if (!enemy[j]->GetIsAlive()) continue;

		//弾と敵の当たり判定
		for (int i = 0; i < playerBulletNum; i++)
		{
			//弾が発射状態でなければ飛ばす
			if (!playerBullet[i]->GetIsAlive()) continue;

			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetScale().x;
			XMFLOAT3 enemyPos = enemy[j]->GetPosition();
			float enemySize = enemy[j]->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//敵と弾が衝突状態でなければ飛ばす
			if (!isCollision) continue;

			//弾は死亡
			playerBullet[i]->Dead();

			//敵はダメージを喰らう
			int bulletPower = playerBullet[i]->GetPower();
			enemy[j]->Damage(bulletPower);

			//ダメージを喰らってもHPが残っていたら飛ばす
			if (enemy[j]->GetHP() > 0) continue;

			//HPが0以下になったら死亡
			enemy[j]->Dead();

			//死んだ敵の位置を増やす
			XMFLOAT3 deadPoint = enemy[j]->GetPosition();
			//死んだ敵の円の大きさ
			float radius = enemy[j]->GetScale().x * ((float)bulletPower / 2);
			deadEnemyPoints.push_back(
				DeadEnemyPoint::Create(circleModel, deadPoint, radius));
		}
	}

	//死んだ敵の位置更新
	for (auto itr = deadEnemyPoints.begin(); itr != deadEnemyPoints.end(); itr++)
	{
		(*itr)->Update();
	}

	//死んだ敵の数が1フレーム前より多ければ新しい線を作るか検討
	if (oldDeadPointNum < deadEnemyPoints.size())
	{
		//リストの(end - 1)の要素を探す
		std::list<DeadEnemyPoint *>::iterator newPointIterator;
		newPointIterator = deadEnemyPoints.end();
		newPointIterator--;

		for (auto itr = deadEnemyPoints.begin(); itr != deadEnemyPoints.end(); itr++)
		{
			//ループ中、(end - 1)の要素は判定しない(startとendが同じ位置の線が出来てしまうため)
			if (itr == newPointIterator) continue;

			//衝突用に新しい円と既存の円の座標と半径の大きさを借りる
			XMFLOAT3 newDeadPoint = (*newPointIterator)->GetPosition();
			float newDeadRadius = (*newPointIterator)->GetRadius();
			XMFLOAT3 existingDeadPoint = (*itr)->GetPosition();
			float existingDeadRadius = (*itr)->GetRadius();

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				newDeadPoint, newDeadRadius, existingDeadPoint, existingDeadRadius);

			//新しい円と既存の円が衝突状態でなければ飛ばす
			if (!isCollision) continue;

			//新しい円と既存の円を繋ぐ新しい線を作る
			powerUpLines.push_back(PowerUpLine::Create(
				newDeadPoint, existingDeadPoint));
		}
	}

	//パワーアップ線更新
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		(*itr)->Update(camera);
	}

	//カメラセット
	if (input->TriggerKey(DIK_RETURN)) { cameraZ *= -1; }
	camera->UpdateTps({ 0,0,cameraZ });

	//スプライト更新
	sprite->Update();
	//デバッグテキスト
	//if (deadEnemyPoints.size() == 1) DebugText::GetInstance()->Print("DEADNUM : 1", 100, 500);	
	//else if (deadEnemyPoints.size() == 2) DebugText::GetInstance()->Print("DEADNUM : 2", 100, 500);	
	//else if (deadEnemyPoints.size() == 3) DebugText::GetInstance()->Print("DEADNUM : 3", 100, 500);

	if (powerUpLines.size() == 0) DebugText::GetInstance()->Print("Line : 0", 100, 600);
	else if (powerUpLines.size() == 1) DebugText::GetInstance()->Print("Line : 1", 100, 600);
	else if (powerUpLines.size() == 2) DebugText::GetInstance()->Print("Line : 2", 100, 600);
	else if (powerUpLines.size() == 3) DebugText::GetInstance()->Print("Line : 3", 100, 600);
	else if (powerUpLines.size() == 4) DebugText::GetInstance()->Print("Line : 4", 100, 600);
	else if (powerUpLines.size() == 5) DebugText::GetInstance()->Print("Line : 5", 100, 600);
	else if (powerUpLines.size() == 6) DebugText::GetInstance()->Print("Line : 6", 100, 600);
	

	DebugText::GetInstance()->Print("PlayerMove:LSTICK", 1000, 100);
	DebugText::GetInstance()->Print("BulletAngle:RSTICK", 1000, 150);
	DebugText::GetInstance()->Print("BulletShot:RB", 1000, 200);
}

void GameScene::Draw(ID3D12GraphicsCommandList *cmdList)
{
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


	//スプライト描画
	Sprite::PreDraw(cmdList);

	sprite->Draw();


	//デバッグテキスト描画
	DebugText::GetInstance()->DrawAll(cmdList);
	Sprite::PostDraw();
}
