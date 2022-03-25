#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

#include "Garuta.h"
#include "Garutata.h"

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
	safe_delete(pBodyModel);
	safe_delete(pHead01Model);
	safe_delete(pHead02Model);
	safe_delete(pHead03Model);
	safe_delete(pBullModel);
	safe_delete(enemy01Model);
	safe_delete(enemy02Model);
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
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		safe_delete(*itrEnemy);
	}
	//敵のリスト解放
	enemys.clear();
	//敵の弾解放
	for (int i = 0; i < enemyBulletNum; i++)
	{
		safe_delete(enemyBullet[i]);
	}
	//死んだ敵の位置解放
	for (auto itrDeadPoint = deadEnemyPoints.begin(); itrDeadPoint != deadEnemyPoints.end(); itrDeadPoint++)
	{
		safe_delete(*itrDeadPoint);
	}
	//死んだ敵の位置のリスト解放
	deadEnemyPoints.clear();

	//パワーアップ線解放
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		safe_delete(*itrLine);
	}
	//パワーアップ線のリスト解放
	powerUpLines.clear();

	//エフェクトの解放
	safe_delete(effects);
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
	pBodyModel = Model::CreateFromOBJ("playerbody");//プレイヤーの体のモデル
	pHead01Model = Model::CreateFromOBJ("playerhead1");//プレイヤーの頭のモデル(HP1)
	pHead02Model = Model::CreateFromOBJ("playerhead2");//プレイヤーの頭のモデル(HP2)
	pHead03Model = Model::CreateFromOBJ("playerhead3");//プレイヤーの頭のモデル(HP3)
	pBullModel = Model::CreateFromOBJ("playerbullet");//プレイヤーの弾のモデル
	enemy01Model = Model::CreateFromOBJ("enemy");//敵01(ガルタ)のモデル
	enemy02Model = Model::CreateFromOBJ("garutata");//敵02(ガルタタ)のモデル
	eBullModel = Model::CreateFromOBJ("enemybullet");//敵の弾のモデル
	deadEnemyModel = Model::CreateFromOBJ("desenemy");//死んだ敵のモデル

	//プレイヤーウエポンのモデルをセット
	Player::SetWeaponModel(pHead01Model, pHead02Model, pHead03Model);
	//プレイヤー生成
	player = Player::Create(pBodyModel);

	//弾生成
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(pBullModel);
	}

	//敵の弾生成
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/kari.png");

	//スプライト生成
	sprite = Sprite::Create(1);
	sprite->SetPosition({ 100, 100 });
	sprite->SetSize({ 100, 100 });
	sprite->SetTexSize({ 64, 64 });

	//デバッグテキスト生成
	DebugText::GetInstance()->Initialize(0);

	//サウンド用
	audio = new Audio();

	//エフェクト初期化
	effects = new StageEffect();
	effects->Initialize();
}

void GameScene::Update(Camera *camera)
{
	Input *input = Input::GetInstance();
	XInputManager *Xinput = XInputManager::GetInstance();

	//プレイヤー更新
	player->Update(effects);

	//デバッグ用線の色
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		(*itr)->SetColor({ 0.4f, 1, 0.2f, 1 });
	}

	//プレイヤー弾発射
	bulletShotTimer--;
	if (bulletShotTimer <= 0 && (input->PushKey(DIK_SPACE) || Xinput->PushButton(XInputManager::PAD_RB)))
	{
		//プレイヤーウエポンの座標と角度を弾も持つ
		XMFLOAT3 pos = player->GetWeaponPosition();
		XMFLOAT3 rota = player->GetWeaponRotation();

		//弾の発射をプレイヤーウエポンの真上に設定
		float angle = DirectX::XMConvertToRadians(rota.z + 90);
		pos.x += 8.0f * cosf(angle);
		pos.y += 8.0f * sinf(angle);

		//左側の弾発射
		for (int i = 0; i < playerBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (playerBullet[i]->GetIsAlive()) { continue; }

			//弾の発射位置を左側にずらす
			float angle2 = DirectX::XMConvertToRadians(rota.z + 180);
			XMFLOAT3 shotPosLeft = {};
			shotPosLeft.x = 1.5f * cosf(angle2) + pos.x;
			shotPosLeft.y = 1.5f * sinf(angle2) + pos.y;

			//弾発射
			playerBullet[i]->BulletStart(shotPosLeft, rota);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}

		//右側の弾発射
		for (int i = 0; i < playerBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (playerBullet[i]->GetIsAlive()) { continue; }

			//弾の発射位置を右側にずらす
			float angle2 = DirectX::XMConvertToRadians(rota.z);
			XMFLOAT3 shotPosRight = {};
			shotPosRight.x = 1.5f * cosf(angle2) + pos.x;
			shotPosRight.y = 1.5f * sinf(angle2) + pos.y;

			//弾発射
			playerBullet[i]->BulletStart(shotPosRight, rota);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}

		//次の弾発射までのタイマーを初期化する
		bulletShotTimer = 10;
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
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			//衝突用に弾の座標と半径、線の始点と終点を借りる
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletRadius = playerBullet[i]->GetScale().x;
			XMFLOAT3 lineStartPoint = (*itrLine)->GetStartPoint();
			XMFLOAT3 lineEndPoint = (*itrLine)->GetEndPoint();

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Line(
				bulletPos, bulletRadius, lineStartPoint, lineEndPoint);

			//弾と線が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//デバッグ用線の色変更
			(*itrLine)->SetColor({ 1, 0, 0, 1 });

			//既に衝突したことがあるか確認(衝突中パワーアップし続けてしまうため)
			if (playerBullet[i]->IsKnowLine((*itrLine))) { continue; }

			//弾をパワーアップさせる
			playerBullet[i]->PowerUp();
		}
	}

	//敵生成
	if (input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_RT))
	{
		//敵をスポーン
		SpawnEnemy();
	}

	//敵更新
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		//存在がなかったら飛ばす
		if (!(*itrEnemy)->GetIsExistence()) { continue; }

		//更新処理
		(*itrEnemy)->Update(effects);

		//ノックバックが終わり、存在がなくなったら
		if (!(*itrEnemy)->GetIsExistence())
		{
			//オブジェクトのモデルを変更する
			(*itrEnemy)->SetModel(deadEnemyModel);

			//死んだ敵の位置を増やす
			XMFLOAT3 deadPoint = (*itrEnemy)->GetPosition();

			//死んだ敵の円の半径をセットする（ 敵の大きさ×（ 倒された時の弾の強さ / 4 ））
			float radius = (*itrEnemy)->GetScale().x * ((float)(*itrEnemy)->GetKillBulletPower() / 4);
			deadEnemyPoints.push_back(
				DeadEnemyPoint::Create(circleModel, deadPoint, radius));

			//敵の存在がなくなったので飛ばす
			continue;
		}

		//敵が生きていなければ飛ばす 敵がスポーン中だったら飛ばす 敵が逃走中だったら飛ばす
		if (!(*itrEnemy)->GetIsAlive() || (*itrEnemy)->GetIsDuringSpawn() || (*itrEnemy)->GetIsEscape()) { continue; }

		//敵弾発射フラグがtrueなら
		if ((*itrEnemy)->GetIsBulletShot())
		{
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//敵弾が発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//敵の座標と標的の座標（プレイヤーの座標）を弾も持つ
				XMFLOAT3 startPos = (*itrEnemy)->GetPosition();
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
			XMFLOAT3 enemyPos = (*itrEnemy)->GetPosition();
			float enemySize = (*itrEnemy)->GetScale().x;
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
				player->SetKnockback();

				isShake = true;

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
			XMFLOAT3 enemyPos = (*itrEnemy)->GetPosition();
			float enemySize = (*itrEnemy)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//敵と弾が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//弾は死亡
			playerBullet[i]->Dead();

			//敵はダメージを喰らう
			int bulletPower = playerBullet[i]->GetPower();
			(*itrEnemy)->Damage(bulletPower);

			//ダメージを喰らってもHPが残っていたら飛ばす
			if ((*itrEnemy)->GetHP() > 0) { continue; }

			//敵のHPが0以下なのでノックバックを開始する
			float bulletAngle = playerBullet[i]->GetAngle();
			(*itrEnemy)->SetKnockBack(bulletAngle, bulletPower);
		}
	}

	//敵削除
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//逃走が完了しているなら
		if ((*itrEnemy)->GetIsEscapeCompleted())
		{
			safe_delete(*itrEnemy);
			itrEnemy = enemys.erase(itrEnemy);
			continue;
		}

		itrEnemy++;
	}

	//死んだ敵の位置更新
	for (auto itrDeadPoint = deadEnemyPoints.begin(); itrDeadPoint != deadEnemyPoints.end(); itrDeadPoint++)
	{
		//サイズ変更状態でない場合は飛ばす
		if (!(*itrDeadPoint)->GetIsChangeRadius()) { continue; }

		//更新
		(*itrDeadPoint)->Update();

		//衝突を判定してパワーアップ線を作成
		for (auto itrDeadPoint2 = deadEnemyPoints.begin(); itrDeadPoint2 != deadEnemyPoints.end(); itrDeadPoint2++)
		{
			CreatePowerUpLine(*itrDeadPoint, *itrDeadPoint2);
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
	if (isShake)
	{
		camera->CameraShake(5);
		ShakeTime++;
		if (ShakeTime > 10) {
			isShake = false;
			ShakeTime = 0;
		}
	}
	camera->TpsCamera({ 0,0,-100 });
	camera->Update();

	//スプライト更新
	sprite->Update();


	if (player->GetHP() == 3) { DebugText::GetInstance()->Print("HP : 3", 100, 500); }
	else if (player->GetHP() == 2) { DebugText::GetInstance()->Print("HP : 2", 100, 500); }
	else if (player->GetHP() == 1) { DebugText::GetInstance()->Print("HP : 1", 100, 500); }
	else if (player->GetHP() == 0) { DebugText::GetInstance()->Print("HP : 0", 100, 500); }

	if (player->GetIsAlive()) { DebugText::GetInstance()->Print("PLAYER ALIVE", 100, 550); }
	else { DebugText::GetInstance()->Print("PLAYER DEAD", 100, 550); }

	if (playerBullet[0]->GetPower() == 10) { DebugText::GetInstance()->Print("POWER : 10", 100, 600); }
	else if (playerBullet[0]->GetPower() == 12) { DebugText::GetInstance()->Print("POWER : 12", 100, 600); }
	else if (playerBullet[0]->GetPower() == 14) { DebugText::GetInstance()->Print("POWER : 14", 100, 600); }
	else if (playerBullet[0]->GetPower() == 16) { DebugText::GetInstance()->Print("POWER : 16", 100, 600); }
	else if (playerBullet[0]->GetPower() == 18) { DebugText::GetInstance()->Print("POWER : 18", 100, 600); }
	else if (playerBullet[0]->GetPower() == 20) { DebugText::GetInstance()->Print("POWER : 20", 100, 600); }
	else if (playerBullet[0]->GetPower() == 22) { DebugText::GetInstance()->Print("POWER : 22", 100, 600); }
	else if (playerBullet[0]->GetPower() == 24) { DebugText::GetInstance()->Print("POWER : 24", 100, 600); }
	else if (playerBullet[0]->GetPower() == 26) { DebugText::GetInstance()->Print("POWER : 26", 100, 600); }
	else if (playerBullet[0]->GetPower() == 28) { DebugText::GetInstance()->Print("POWER : 28", 100, 600); }

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 1000, 100);
	DebugText::GetInstance()->Print("LB:Sneak", 1000, 150);
	DebugText::GetInstance()->Print("RB:BulletShot", 1000, 200);
	DebugText::GetInstance()->Print("ENTERKEY:CreateEnemy", 1000, 250);

	//エフェクトの更新
	effects->Update(camera);
}

void GameScene::Draw(ID3D12GraphicsCommandList *cmdList)
{
	//スプライト背面描画
	//Sprite::PreDraw(cmdList);

	//sprite->Draw();
	//Sprite::PostDraw();

	//オブジェクト描画
	Object3d::PreDraw(cmdList);

	//プレイヤー描画
	player->Draw();

	Object3d::PostDraw();

	//エフェクトの描画
	effects->Draw(cmdList);

	//線3d
	DrawLine3D::PreDraw(cmdList);

	//パワーアップ線描画
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		(*itrLine)->Draw();
	}

	DrawLine3D::PostDraw();

	//オブジェクト描画
	Object3d::PreDraw(cmdList);

	//プレイヤー弾描画
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Draw();
	}
	//敵の弾描画
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i]->Draw();
	}
	//敵描画
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		(*itrEnemy)->Draw();
	}
	//死んだ敵の位置描画
	for (auto itrDeadPoint = deadEnemyPoints.begin(); itrDeadPoint != deadEnemyPoints.end(); itrDeadPoint++)
	{
		(*itrDeadPoint)->Draw();
	}

	Object3d::PostDraw();


	//スプライト前面描画
	Sprite::PreDraw(cmdList);

	//デバッグテキスト描画
	DebugText::GetInstance()->DrawAll(cmdList);

	Sprite::PostDraw();
}

void GameScene::SpawnEnemy()
{
	//生成時にスポーン座標と移動後の座標を決める
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	spawnPos.x = (float)(rand() % 200 - 100);
	spawnPos.y = 100;

	stayPos.x = (float)(rand() % 200 - 100);
	stayPos.y = (float)(rand() % 120 - 60);


	//20%の確率でガルタタ　80%の確率でガルタを生成
	int enemyKindRand = rand() % 5;
	if (enemyKindRand == 0)
	{
		enemys.push_back(Garutata::Create(enemy02Model, enemy02Model, spawnPos, stayPos));
	}
	else
	{
		enemys.push_back(Garuta::Create(enemy01Model, enemy01Model, spawnPos, stayPos));
	}
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
