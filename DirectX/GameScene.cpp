#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

#include "Garuta.h"
#include "Garutata.h"
#include "Tuff.h"
#include "PinCircle.h"
#include "StartSetCircle.h"

const float radian = XM_PI / 180.0f;//ラジアン

GameScene::~GameScene()
{
	XInputManager* Xinput = XInputManager::GetInstance();
	//振動オフ
	Xinput->EndVibration();
	Xinput = nullptr;

	//newオブジェクトの破棄
	safe_delete(audio);
	safe_delete(light);

	//モデル解放
	safe_delete(circleModel);
	safe_delete(playerModel);
	safe_delete(pBodyModel);
	safe_delete(pHead01Model);
	safe_delete(pHead02Model);
	safe_delete(pHead03Model);
	safe_delete(razorModel);
	safe_delete(pBullModel);
	safe_delete(enemy01Model);
	safe_delete(enemyPoint01Model);
	safe_delete(enemy02Model);
	safe_delete(enemyPoint02Model);
	safe_delete(eBullModel);
	safe_delete(deadEnemyModel);
	safe_delete(initialCircleCoreModel);
	safe_delete(initialCircleSquareModel);
	safe_delete(hexagonModel);
	safe_delete(happyModel);
	safe_delete(portaModel);
	safe_delete(charoModel);
	safe_delete(tuffModel);
	safe_delete(frameModel);
	safe_delete(waveModel);
	safe_delete(coreModel);
	safe_delete(RBbuttonModel);

	//プレイヤー解放
	safe_delete(player);

	//プレイヤー弾解放
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}

	//レーザーサイト解放
	safe_delete(laserSite);

	//エネルギーポイント解放
	safe_delete(energy);

	//コア解放
	safe_delete(core);

	//ガル族解放
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		safe_delete(*itrGaruEnemy);
	}
	//ガル族のリスト解放
	garuEnemys.clear();

	//チャロ解放
	for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
	{
		safe_delete(*itrCharo);
	}
	//チャロのリスト解放
	charoEnemys.clear();

	//ポルタ解放
	for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
	{
		safe_delete(*itrPorta);
	}
	//ポルタのリスト解放
	portaEnemys.clear();

	//ボス解放
	for (int i = 0; i < bossNum; i++)
	{
		safe_delete(bossEnemy[i]);
	}

	//敵の弾解放
	for (int i = 0; i < enemyBulletNum; i++)
	{
		safe_delete(enemyBullet[i]);
	}

	//ピン解放
	for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
	{
		safe_delete(*itrPin);
	}
	//ピンのリスト解放
	pins.clear();

	//固定敵解放
	for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
	{
		safe_delete(*itrFixedEnemy);
	}
	//固定敵のリスト解放
	fixedEnemys.clear();

	//コネクトサークル解放
	for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
	{
		safe_delete(*itrConnectCircle);
	}
	//コネクトサークルのリスト解放
	connectCircles.clear();

	//パワーアップ線解放
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		safe_delete(*itrLine);
	}
	//パワーアップ線のリスト解放
	powerUpLines.clear();

	//衝撃波解放
	for (auto itrShockWave = shockWaves.begin(); itrShockWave != shockWaves.end(); itrShockWave++)
	{
		safe_delete(*itrShockWave);
	}
	//衝撃波のリスト解放
	shockWaves.clear();

	//吸収円解放
	for (auto itrAbsorptionCircle = absorptionCircles.begin(); itrAbsorptionCircle != absorptionCircles.end(); itrAbsorptionCircle++) 
	{
		safe_delete(*itrAbsorptionCircle);
	}
	//吸収円のリスト解放
	absorptionCircles.clear();

	//画面枠解放
	safe_delete(frame);

	//制限時間解放
	safe_delete(timeLimit);

	//エフェクトの解放
	safe_delete(effects);

	//背景の解放
	buckGround->AllDelete();
	safe_delete(buckGround);
}

void GameScene::Initialize(Camera* camera)
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
	razorModel = Model::CreateFromOBJ("razor");//レーザーのモデル
	pBullModel = Model::CreateFromOBJ("playerbullet");//プレイヤーの弾のモデル
	enemy01Model = Model::CreateFromOBJ("enemy");//敵01(ガルタ)のモデル
	enemyPoint01Model = Model::CreateFromOBJ("garutaspown");//敵01(ガルタ)の出現位置のモデル
	enemy02Model = Model::CreateFromOBJ("garutata");//敵02(ガルタタ)のモデル
	enemyPoint02Model = Model::CreateFromOBJ("garutataspown");//敵02(ガルタタ)の出現位置のモデル
	eBullModel = Model::CreateFromOBJ("enemybullet");//敵の弾のモデル
	deadEnemyModel = Model::CreateFromOBJ("desenemy");//死んだ敵のモデル
	initialCircleCoreModel = Model::CreateFromOBJ("initialcirclecore");;//固定敵のコアのモデル
	initialCircleSquareModel = Model::CreateFromOBJ("initialcirclesquare");//固定敵の外枠のモデル
	hexagonModel = Model::CreateFromOBJ("hexagon");//六角形のモデル
	happyModel = Model::CreateFromOBJ("happy");//タバコモデル
	portaModel = Model::CreateFromOBJ("porta");//ポルタのモデル
	charoModel = Model::CreateFromOBJ("charo");//チャロのモデル
	tuffModel = Model::CreateFromOBJ("tuff");//タッフのモデル
	frameModel = Model::CreateFromOBJ("frame");//フレームのモデル
	waveModel = Model::CreateFromOBJ("wave");//衝撃波のモデル
	coreModel = Model::CreateFromOBJ("core");//コアのモデル
	RBbuttonModel = Model::CreateFromOBJ("RB");//RBボタンのモデル


	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/title.png");
	Sprite::LoadTexture(2, L"Resources/number.png");
	Sprite::LoadTexture(3, L"Resources/result.png");
	Sprite::LoadTexture(4, L"Resources/SCORE.png");
	Sprite::LoadTexture(5, L"Resources/pressButton.png");
	Sprite::LoadTexture(6, L"Resources/white1x1.png");
	Sprite::LoadTexture(7, L"Resources/gaugeIn.png");
	Sprite::LoadTexture(8, L"Resources/gaugeOut.png");
	//デバッグテキスト生成
	DebugText::GetInstance()->Initialize(0);


	//プレイヤーウエポンのモデルをセット
	Player::SetWeaponModel(pHead01Model, pHead02Model, pHead03Model);
	//プレイヤー生成
	player = Player::Create(pBodyModel);
	//弾生成
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(pBullModel);
	}
	//レーザーサイト生成
	laserSite = LaserSite::Create(razorModel);

	//エネルギーポイント生成
	energy = Energy::Create(8, 7);

	//コア生成
	core = Core::Create(coreModel);

	//ボス生成
	bossEnemy[0] = Tuff::Create(tuffModel);

	//敵の弾生成
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//画面枠生成
	frame = Frame::Create(frameModel);

	//制限時間生成
	timeLimit = TimeLimit::Create(2);

	//エフェクト初期化
	effects = new StageEffect();
	effects->Initialize();

	//背景の初期化
	buckGround->Create(hexagonModel);


	//サウンド用
	audio = new Audio();
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//プレイヤー更新
	player->Update();

	//LBボタン入力。ステップ状態ではない。エネルギーポイントが消費量以上ある場合ステップ状態にする
	if ((input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_LB))
		&& !(player->GetIsStep()) && energy->CheckPercent(20))
	{
		//プレイヤーをステップ状態にする
		player->SetStep();

		//エネルギーポイントを消費
		energy->UseTwentyPercent();
	}

	//レーザーサイト更新
	laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
	laserSite->Update();

	//プレイヤー弾溜め開始
	if (input->TriggerKey(DIK_SPACE) || Xinput->TriggerButton(XInputManager::PAD_RB))
	{
		//プレイヤーがステップ中でなければ溜め開始
		if (!player->GetIsStep())
		{
			//溜めを開始
			isBulletPowerUp = true;
			//溜めタイマーを初期化
			bulletPowerUpTimer = 0;
			//威力を初期化
			bulletPowerLevel = 0;
		}
	}
	//溜めている間
	if (isBulletPowerUp)
	{
		//威力溜め 発射
		PowerUpPlayerBullet();
	}

	//プレイヤー弾更新
	for (int i = 0; i < playerBulletNum; i++)
	{
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

			//線の始点と終点のコネクトサークルから衝撃波を出す
			for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
			{
				//線がコネクトサークルを使用していなかったら飛ばす
				if (!(*itrLine)->CheckUsePoints(*itrConnectCircle)) { continue; }

				//使用していたら衝撃波をコネクトサークルから発射する
				const XMFLOAT3 pos = (*itrConnectCircle)->GetPosition();
				const int power = playerBullet[i]->GetPower();
				shockWaves.push_back(ShockWave::Create(waveModel, pos, power));
			}
		}
	}


	if (input->TriggerKey(DIK_5) || Xinput->TriggerButton(XInputManager::PAD_BUCK))
	{
		//エネルギーポイントを増やす
		const int point = 10;
		energy->AddEnergyPoint(point);
	}
	//エネルギー更新
	energy->Update();

	//敵生成処理
	SpawnEnemyManager(isBossStage, wave);//敵生成カウント更新や敵更新

	//ガル族生成
	if (input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_RT))
	{
		//ガル族をスポーン
		SpawnGaruEnemy(0, wave);
	}

	//ガル族更新
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		//更新処理
		(*itrGaruEnemy)->Update();

		//ガル族が生きていなければ飛ばす ガル族がスポーン中だったら飛ばす ガル族が逃走中だったら飛ばす
		if (!(*itrGaruEnemy)->GetIsAlive() || (*itrGaruEnemy)->GetIsDuringSpawn() || (*itrGaruEnemy)->GetIsEscape()) { continue; }

		//弾発射フラグがtrueなら
		if ((*itrGaruEnemy)->GetIsBulletShot())
		{
			//弾発射
			GaruEnemyShotBullet(*itrGaruEnemy);
		}

		//プレイヤーとガル族の当たり判定
		//プレイヤーがダメージ状態でないなら判定をする
		if (!player->GetIsDamege())
		{
			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//ガル族とプレイヤーが衝突状態
			if (isCollision)
			{
				//プレイヤーはダメージを喰らう
				player->Damage();
				player->SetKnockback();
				//エネルギーポイントを失う
				energy->LoseEnergyPoint();

				isShake = true;
			}
		}

		//弾とガル族の当たり判定
		for (int i = 0; i < playerBulletNum; i++)
		{
			//弾が発射状態でなければ飛ばす
			if (!playerBullet[i]->GetIsAlive()) { continue; }

			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetScale().x;
			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//ガル族と弾が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//弾は死亡
			playerBullet[i]->Dead();

			//ガル族はダメージを喰らう
			int bulletPower = playerBullet[i]->GetPower();
			(*itrGaruEnemy)->Damage(bulletPower);

			//ダメージを喰らってもHPが残っていたら飛ばす
			if ((*itrGaruEnemy)->GetHP() > 0) { continue; }

			//ガル族のHPが0以下なのでノックバックを開始する
			float bulletAngle = playerBullet[i]->GetAngle();
			(*itrGaruEnemy)->SetKnockBack(bulletAngle, bulletPower);
		}

		//ピンとガル族の当たり判定
		for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
		{
			//ピンがダメージ状態なら飛ばす
			if ((*itrPin)->GetIsDamege()) { continue; }

			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x;
			XMFLOAT3 pinPos = (*itrPin)->GetPosition();
			float pinSize = (*itrPin)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, pinPos, pinSize);

			//ガル族とピンが衝突状態でないなら飛ばす
			if (!isCollision) { continue; }

			//ピンはダメージを喰らう
			int damage = 10;
			(*itrPin)->Damage(damage);
		}
	}

	//チャロポルタ生成
	if (input->TriggerKey(DIK_LSHIFT) || Xinput->TriggerButton(XInputManager::PAD_LT))
	{
		SpawnCharoPorta(0, wave);
	}

	//チャロ更新
	for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
	{
		//更新処理
		XMFLOAT3 tartgetPos = player->GetPosition();
		(*itrCharo)->Update(tartgetPos);

		//プレイヤーとチャロの当たり判定
		//衝突用に座標と半径の大きさを借りる
		XMFLOAT3 enemyPos = (*itrCharo)->GetPosition();
		float enemySize = (*itrCharo)->GetScale().x;
		XMFLOAT3 playerPos = player->GetPosition();
		float playerSize = player->GetScale().x;

		//衝突判定を計算
		bool isCollision = Collision::CheckCircle2Circle(
			enemyPos, enemySize, playerPos, playerSize);

		//プレイヤーとチャロが衝突状態
		if (isCollision)
		{
			//チャロも相打ちで死亡
			(*itrCharo)->Dead();

			//プレイヤーがダメージ状態でないなら判定をする
			if (!player->GetIsDamege())
			{
				//プレイヤーはダメージを喰らう
				player->Damage();
				player->SetKnockback();
				//エネルギーポイントを失う
				energy->LoseEnergyPoint();

				//画面をシェイクさせる
				isShake = true;
			}
		}

		//弾とチャロの当たり判定
		for (int i = 0; i < playerBulletNum; i++)
		{
			//弾が発射状態でなければ飛ばす
			if (!playerBullet[i]->GetIsAlive()) { continue; }

			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//チャロと弾が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//弾は死亡
			playerBullet[i]->Dead();

			//チャロはダメージを喰らう
			int bulletPower = playerBullet[i]->GetPower();
			(*itrCharo)->Damage(bulletPower);
		}

		//ピンとチャロの当たり判定
		for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
		{
			//ピンがダメージ状態なら飛ばす
			if ((*itrPin)->GetIsDamege()) { continue; }

			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 pinPos = (*itrPin)->GetPosition();
			float pinSize = (*itrPin)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, pinPos, pinSize);

			//チャロとピンが衝突状態でないなら飛ばす
			if (!isCollision) { continue; }

			//ピンはダメージを喰らう
			int damage = 10;
			(*itrPin)->Damage(damage);

			//チャロも相打ちで死亡
			(*itrCharo)->Dead();
		}
	}

	//ポルタ更新
	for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
	{
		//更新処理
		(*itrPorta)->Update();

		//プレイヤーとポルタの当たり判定
		//衝突用に座標と半径の大きさを借りる
		XMFLOAT3 enemyPos = (*itrPorta)->GetPosition();
		float enemySize = (*itrPorta)->GetScale().x;
		XMFLOAT3 playerPos = player->GetPosition();
		float playerSize = player->GetScale().x;

		//衝突判定を計算
		bool isCollision = Collision::CheckCircle2Circle(
			enemyPos, enemySize, playerPos, playerSize);

		//プレイヤーとポルタが衝突状態
		if (isCollision)
		{
			//ポルタも相打ちで死亡
			(*itrPorta)->Dead();

			//プレイヤーがダメージ状態でないなら判定をする
			if (!player->GetIsDamege())
			{
				//プレイヤーはダメージを喰らう
				player->Damage();
				player->SetKnockback();
				//エネルギーポイントを失う
				energy->LoseEnergyPoint();

				//画面をシェイクさせる
				isShake = true;
			}
		}

		//弾とポルタの当たり判定
		for (int i = 0; i < playerBulletNum; i++)
		{
			//弾が発射状態でなければ飛ばす
			if (!playerBullet[i]->GetIsAlive()) { continue; }

			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//ポルタと弾が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//弾は死亡
			playerBullet[i]->Dead();

			//ポルタはダメージを喰らう
			int bulletPower = playerBullet[i]->GetPower();
			(*itrPorta)->Damage(bulletPower);
		}

		//ピンとポルタの当たり判定
		for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
		{
			//ピンがダメージ状態なら飛ばす
			if ((*itrPin)->GetIsDamege()) { continue; }

			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 pinPos = (*itrPin)->GetPosition();
			float pinSize = (*itrPin)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, pinPos, pinSize);

			//ポルタとピンが衝突状態でないなら飛ばす
			if (!isCollision) { continue; }

			//ピンはダメージを喰らう
			int damage = 10;
			(*itrPin)->Damage(damage);

			//ポルタも相打ちで死亡
			(*itrPorta)->Dead();
		}
	}

	//ボス戦開始
	if (!isBossStage && input->TriggerKey(DIK_0))
	{
		BossStageStart();
	}

	//ボス戦中
	if (isBossStage)
	{
		//ボス更新
		XMFLOAT3 targetPos = player->GetPosition();
		bossEnemy[moveBossNumber]->Update(targetPos);

		//ボスがスポーン中以外だったら弾発射と当たり判定を行う
		if (!bossEnemy[moveBossNumber]->GetIsDuringSpawn())
		{
			//弾発射フラグがtrueなら
			if (bossEnemy[moveBossNumber]->GetIsBulletShot())
			{
				//弾発射
				BossEnemyShotBullet(moveBossNumber);
			}

			//衝撃フラグがtrueなら
			if (bossEnemy[moveBossNumber]->TriggerImpact())
			{
				//画面をシェイクさせる
				isShake = true;

				//ガル族を降らせる
				BossImpactFallEnemy();
			}

			//プレイヤーとボスの当たり判定
			//プレイヤーがダメージ状態でないなら判定をする
			if (!player->GetIsDamege())
			{
				//衝突用に座標と半径の大きさを借りる
				XMFLOAT3 bossPos = bossEnemy[moveBossNumber]->GetPosition();
				float bossSize = bossEnemy[moveBossNumber]->GetScale().x;
				XMFLOAT3 playerPos = player->GetPosition();
				float playerSize = player->GetScale().x;

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					bossPos, bossSize, playerPos, playerSize);

				//プレイヤーとボスが衝突状態
				if (isCollision)
				{
					//プレイヤーはダメージを喰らう
					player->Damage();
					player->SetKnockback();
					//エネルギーポイントを失う
					energy->LoseEnergyPoint();

					//画面をシェイクさせる
					isShake = true;
				}
			}

			//弾とボスの当たり判定
			for (int i = 0; i < playerBulletNum; i++)
			{
				//弾が発射状態でなければ飛ばす
				if (!playerBullet[i]->GetIsAlive()) { continue; }

				//衝突用に座標と半径の大きさを借りる
				XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
				float bulletSize = playerBullet[i]->GetScale().x;
				XMFLOAT3 bossPos = bossEnemy[moveBossNumber]->GetPosition();
				float bossSize = bossEnemy[moveBossNumber]->GetScale().x;

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					bulletPos, bulletSize, bossPos, bossSize);

				//ボスと弾が衝突状態でなければ飛ばす
				if (!isCollision) { continue; }

				//弾は死亡
				playerBullet[i]->Dead();

				//ボスはダメージを喰らう
				int bulletPower = playerBullet[i]->GetPower();
				bossEnemy[moveBossNumber]->Damage(bulletPower);

				//ダメージを喰らってもHPが残っていたら飛ばす
				if (bossEnemy[moveBossNumber]->GetHP() > 0) { continue; }

				//チャロのHPが0以下なので死亡
				bossEnemy[moveBossNumber]->Dead();
			}
		}

		//ボスが死亡したら
		if (!bossEnemy[moveBossNumber]->GetIsAlive())
		{
			//死亡してサイズを変更状態にする
			bossEnemy[moveBossNumber]->SetDeadChangeScale();

			//ボス戦終了
			isBossStage = false;
		}
	}

	//敵の弾更新
	for (int i = 0; i < enemyBulletNum; i++)
	{
		//弾が生きていなければ飛ばす
		if (!enemyBullet[i]->GetIsAlive()) { continue; }

		//更新処理
		enemyBullet[i]->Update();

		//プレイヤーと敵の弾の当たり判定
		//プレイヤーがダメージ状態でないなら
		if (!player->GetIsDamege())
		{
			//プレイヤーと敵の弾の当たり判定
			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
			float bulletSize = enemyBullet[i]->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, playerPos, playerSize);

			//プレイヤーと敵の弾が衝突状態
			if (isCollision)
			{
				//敵の弾は死亡
				enemyBullet[i]->Dead();
				//プレイヤーはダメージを喰らう
				player->Damage();
				//エネルギーポイントを失う
				energy->LoseEnergyPoint();
			}
		}

		//ピンと敵の弾の当たり判定
		for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
		{
			//ピンがダメージ状態なら飛ばす
			if ((*itrPin)->GetIsDamege()) { continue; }

			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
			float bulletSize = enemyBullet[i]->GetScale().x;
			XMFLOAT3 pinPos = (*itrPin)->GetPosition();
			float pinSize = (*itrPin)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, pinPos, pinSize);

			//敵の弾とピンが衝突状態でないなら飛ばす
			if (!isCollision) { continue; }

			//ピンはダメージを喰らう
			int damage = 10;
			(*itrPin)->Damage(damage);

			//敵の弾も死亡
			enemyBullet[i]->Dead();
		}
	}

	//ピン生成を開始する
	if (input->TriggerKey(DIK_A) || Xinput->TriggerButton(XInputManager::PAD_A))
	{
		CreatePinStart();
	}
	//ピン作成状態なら作成を続ける
	if (isCreatePin)
	{
		CreatePin();
	}

	//ピン更新
	for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
	{
		//更新処理
		(*itrPin)->Update();
	}

	//固定敵更新
	for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
	{
		(*itrFixedEnemy)->Update();
	}

	//コネクトサークル更新
	for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
	{
		//更新
		(*itrConnectCircle)->Update();

		//サイズ変更状態でない場合は飛ばす
		if (!(*itrConnectCircle)->GetIsChangeRadius()) { continue; }

		//衝突を判定してパワーアップ線を作成
		for (auto itrConnectCircle2 = connectCircles.begin(); itrConnectCircle2 != connectCircles.end(); itrConnectCircle2++)
		{
			CreatePowerUpLine(*itrConnectCircle, *itrConnectCircle2);
		}
	}

	//パワーアップ線更新
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		//更新処理
		(*itrLine)->Update(camera);

		//ステップ中のプレイヤーとの当たり判定
		if (!player->GetIsStep()) { continue; }

		//衝突用に座標と半径、線の始点と終点を借りる
		XMFLOAT3 playerPos = player->GetPosition();
		float playerRadius = player->GetScale().x;
		XMFLOAT3 lineStartPoint = (*itrLine)->GetStartPoint();
		XMFLOAT3 lineEndPoint = (*itrLine)->GetEndPoint();

		//衝突判定を計算
		bool isCollision = Collision::CheckCircle2Line(
			playerPos, playerRadius, lineStartPoint, lineEndPoint);

		//プレイヤーと線が衝突状態でなければ飛ばす
		if (!isCollision) { continue; }

		//既に衝突したことがあるか確認(衝突中吸収円を出し続けてしまうため)
		if (player->IsKnowLine((*itrLine))) { continue; }

		//吸収円を生成する
		XMFLOAT3 pos = player->GetPosition();
		absorptionCircles.push_back(AbsorptionCircle::Create(
			circleModel, pos));
	}

	if (input->TriggerKey(DIK_9)) 
	{
		//吸収円を生成する
		XMFLOAT3 pos = player->GetPosition();
		absorptionCircles.push_back(AbsorptionCircle::Create(
			circleModel, pos));
	}

	//衝撃波更新
	for (auto itrShockWave = shockWaves.begin(); itrShockWave != shockWaves.end(); itrShockWave++)
	{
		//更新処理
		(*itrShockWave)->Update();

		//当たり判定用変数
		XMFLOAT3 wavePos = (*itrShockWave)->GetPosition();
		float waveSize = (*itrShockWave)->GetRadius();

		//衝撃波とガル族の当たり判定
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//ガル族が生きていなければ飛ばす ガル族がスポーン中だったら飛ばす ガル族が逃走中だったら飛ばす
			if (!(*itrGaruEnemy)->GetIsAlive() || (*itrGaruEnemy)->GetIsDuringSpawn() || (*itrGaruEnemy)->GetIsEscape()) { continue; }

			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, enemyPos, enemySize);

			//衝撃波とガル族が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//既に衝突したことがあるか確認(衝突中ダメージを食らい続けてしまうため)
			if ((*itrShockWave)->IsKnowGaruEnemy((*itrGaruEnemy))) { continue; }

			//ガル族にダメージを喰らわせる
			int damagePower = (*itrShockWave)->GetPower();
			(*itrGaruEnemy)->Damage(damagePower);
		}
		//衝撃波とチャロの当たり判定
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			XMFLOAT3 enemyPos = (*itrCharo)->GetPosition();
			float enemySize = (*itrCharo)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, enemyPos, enemySize);

			//衝撃波とチャロが衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//既に衝突したことがあるか確認(衝突中ダメージを食らい続けてしまうため)
			if ((*itrShockWave)->IsKnowCharo((*itrCharo))) { continue; }

			//チャロにダメージを喰らわせる
			int damagePower = (*itrShockWave)->GetPower();
			(*itrCharo)->Damage(damagePower);
		}
		//衝撃波とポルタの当たり判定
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			XMFLOAT3 enemyPos = (*itrPorta)->GetPosition();
			float enemySize = (*itrPorta)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, enemyPos, enemySize);

			//衝撃波とポルタが衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//既に衝突したことがあるか確認(衝突中ダメージを食らい続けてしまうため)
			if ((*itrShockWave)->IsKnowPorta((*itrPorta))) { continue; }

			//ポルタにダメージを喰らわせる
			int damagePower = (*itrShockWave)->GetPower();
			(*itrPorta)->Damage(damagePower);
		}
		//衝撃波と敵の弾の当たり判定
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//弾が生きていなければ飛ばす
			if (!enemyBullet[i]->GetIsAlive()) { continue; }

			XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
			float bulletSize = enemyBullet[i]->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, bulletPos, bulletSize);

			//衝撃波と敵の弾が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//敵の弾を消す
			enemyBullet[i]->Dead();
		}
	}

	//吸収円更新
	for (auto itrAbsorptionCircle = absorptionCircles.begin(); itrAbsorptionCircle != absorptionCircles.end(); itrAbsorptionCircle++)
	{
		//更新処理
		(*itrAbsorptionCircle)->Update();

		//当たり判定用変数
		XMFLOAT3 circlePos = (*itrAbsorptionCircle)->GetPosition();
		float circleSize = (*itrAbsorptionCircle)->GetScale().x;

		//吸収円とガル族の当たり判定
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//ガル族が生きていなければ飛ばす ガル族がスポーン中だったら飛ばす ガル族が逃走中だったら飛ばす
			if (!(*itrGaruEnemy)->GetIsAlive() || (*itrGaruEnemy)->GetIsDuringSpawn() || (*itrGaruEnemy)->GetIsEscape()) { continue; }

			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				circlePos, circleSize, enemyPos, enemySize);

			//吸収円とガル族が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//ガル族を死亡させる
			(*itrGaruEnemy)->Dead();

			//エネルギーポイントを増やす
			const int point = 2;
			energy->AddEnergyPoint(point);
		}
		//吸収円とチャロの当たり判定
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			XMFLOAT3 enemyPos = (*itrCharo)->GetPosition();
			float enemySize = (*itrCharo)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				circlePos, circleSize, enemyPos, enemySize);

			//吸収円とチャロが衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//チャロを死亡させる
			(*itrCharo)->Dead();

			//エネルギーポイントを増やす
			const int point = 2;
			energy->AddEnergyPoint(point);
		}
		//吸収円とポルタの当たり判定
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			XMFLOAT3 enemyPos = (*itrPorta)->GetPosition();
			float enemySize = (*itrPorta)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				circlePos, circleSize, enemyPos, enemySize);

			//吸収円とポルタが衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//ポルタを死亡させる
			(*itrPorta)->Dead();

			//エネルギーポイントを増やす
			const int point = 2;
			energy->AddEnergyPoint(point);
		}
		//吸収円と敵の弾の当たり判定
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//弾が生きていなければ飛ばす
			if (!enemyBullet[i]->GetIsAlive()) { continue; }

			XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
			float bulletSize = enemyBullet[i]->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				circlePos, circleSize, bulletPos, bulletSize);

			//吸収円と敵の弾が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//敵の弾を消す
			enemyBullet[i]->Dead();

			//エネルギーポイントを増やす
			const int point = 2;
			energy->AddEnergyPoint(point);
		}
	}

	//ガル族削除
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrGaruEnemy)->GetIsDelete())
		{
			//エネルギーポイントを増やす
			const int point = 1;
			energy->AddEnergyPoint(point);

			//要素を削除、リストから除外する
			safe_delete(*itrGaruEnemy);
			itrGaruEnemy = garuEnemys.erase(itrGaruEnemy);
			continue;
		}
		//for分を回す
		itrGaruEnemy++;
	}

	//チャロ削除
	for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrCharo)->GetIsDelete())
		{
			//エネルギーポイントを増やす
			const int point = 1;
			energy->AddEnergyPoint(point);

			//要素を削除、リストから除外する
			safe_delete(*itrCharo);
			itrCharo = charoEnemys.erase(itrCharo);
			continue;
		}
		//for分を回す
		itrCharo++;
	}

	//ポルタ削除
	for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrPorta)->GetIsDelete())
		{
			//エネルギーポイントを増やす
			const int point = 1;
			energy->AddEnergyPoint(point);

			//要素を削除、リストから除外する
			safe_delete(*itrPorta);
			itrPorta = portaEnemys.erase(itrPorta);
			continue;
		}
		//for分を回す
		itrPorta++;
	}

	//ピン削除
	for (auto itrPin = pins.begin(); itrPin != pins.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrPin)->GetIsDelete())
		{
			//コネクトサークルが削除するピンを使用しているか確認
			for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
			{
				//使用していたらコネクトサークルを削除状態にセット
				if ((*itrConnectCircle)->CheckUsePin(*itrPin))
				{
					(*itrConnectCircle)->SetDelete();
				}
			}

			//要素を削除、リストから除外する
			safe_delete(*itrPin);
			itrPin = pins.erase(itrPin);
			continue;
		}
		//for分を回す
		itrPin++;
	}

	//コネクトサークル削除
	for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrConnectCircle)->GetIsDelete())
		{
			//パワーアップ線が削除するコネクトサークルを使用しているか確認
			for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
			{
				//使用していたら線を削除状態にセット
				if ((*itrLine)->CheckUsePoints(*itrConnectCircle))
				{
					(*itrLine)->SetDelete();
				}
			}

			//要素を削除、リストから除外する
			safe_delete(*itrConnectCircle);
			itrConnectCircle = connectCircles.erase(itrConnectCircle);
			continue;
		}
		//for分を回す
		itrConnectCircle++;
	}

	//パワーアップ線削除
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrLine)->GetIsDelete())
		{
			//要素を削除、リストから除外する
			safe_delete(*itrLine);
			itrLine = powerUpLines.erase(itrLine);
			continue;
		}
		//for分を回す
		itrLine++;
	}

	//衝撃波削除
	for (auto itrShockWave = shockWaves.begin(); itrShockWave != shockWaves.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrShockWave)->GetIsDelete())
		{
			//要素を削除、リストから除外する
			safe_delete(*itrShockWave);
			itrShockWave = shockWaves.erase(itrShockWave);
			continue;
		}
		//for分を回す
		itrShockWave++;
	}

	//吸収円削除
	for (auto itrAbsorptionCircle = absorptionCircles.begin(); itrAbsorptionCircle != absorptionCircles.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrAbsorptionCircle)->GetIsDelete())
		{
			//要素を削除、リストから除外する
			safe_delete(*itrAbsorptionCircle);
			itrAbsorptionCircle = absorptionCircles.erase(itrAbsorptionCircle);
			continue;
		}
		//for分を回す
		itrAbsorptionCircle++;
	}

	if (timeLimit->GetTime() <= 0)
	{
		if (powerUpLines.size() == 0)
		{
			DebugText::GetInstance()->Print("NotClear", 100, 600);
		}
		else
		{
			DebugText::GetInstance()->Print("Clear", 100, 600);
		}
	}

	if (bulletPowerLevel == 0) { DebugText::GetInstance()->Print("BulletPowerLevel 0", 100, 650); }
	else if (bulletPowerLevel == 1) { DebugText::GetInstance()->Print("BulletPowerLevel 1", 100, 650); }
	else if (bulletPowerLevel == 2) { DebugText::GetInstance()->Print("BulletPowerLevel 2", 100, 650); }
	else if (bulletPowerLevel == 3) { DebugText::GetInstance()->Print("BulletPowerLevel 3", 100, 650); }
	else if (bulletPowerLevel == 4) { DebugText::GetInstance()->Print("BulletPowerLevel 4", 100, 650); }
	else if (bulletPowerLevel == 5) { DebugText::GetInstance()->Print("BulletPowerLevel 5", 100, 650); }

	if (absorptionCircles.size() == 0) { DebugText::GetInstance()->Print("absorptionCircles 0", 100, 600); }
	else if (absorptionCircles.size() == 1) { DebugText::GetInstance()->Print("absorptionCircles 1", 100, 600); }
	else if (absorptionCircles.size() == 2) { DebugText::GetInstance()->Print("absorptionCircles 2", 100, 600); }
	else if (absorptionCircles.size() == 3) { DebugText::GetInstance()->Print("absorptionCircles 3", 100, 600); }
	else if (absorptionCircles.size() == 4) { DebugText::GetInstance()->Print("absorptionCircles 4", 100, 600); }
	else if (absorptionCircles.size() == 5) { DebugText::GetInstance()->Print("absorptionCircles 5", 100, 600); }

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 1000, 100);
	DebugText::GetInstance()->Print("RSTICK:ChangeAngle", 1000, 150);
	DebugText::GetInstance()->Print("LB:Step", 1000, 200);
	DebugText::GetInstance()->Print("RB:BulletShot", 1000, 250);
	DebugText::GetInstance()->Print("A :SetPin", 1000, 300);

	//制限時間更新
	timeLimit->Update();

	//エフェクトの更新
	effects->Update(camera);

	//背景更新
	buckGround->Update();

	//カメラ更新
	CameraUpdate(camera);

	//画面枠更新
	frame->Update();

	input = nullptr;
}

void GameScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	{
		//オブジェクト描画
		Object3d::PreDraw(cmdList);

		//画面枠描画
		frame->Draw();

		//プレイヤー描画
		player->Draw();

		//レーザーサイト描画
		laserSite->Draw();

		//衝撃波描画
		for (auto itrShockWave = shockWaves.begin(); itrShockWave != shockWaves.end(); itrShockWave++)
		{
			(*itrShockWave)->Draw();
		}

		//ピン描画
		for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
		{
			(*itrPin)->Draw();
		}

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

		//ボス戦中
		if (isBossStage)
		{
			//ボス更描画
			bossEnemy[moveBossNumber]->Draw();
		}

		//ガル族描画
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			(*itrGaruEnemy)->Draw();
		}
		//チャロ描画
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			(*itrCharo)->Draw();
		}
		//ポルタ描画
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			(*itrPorta)->Draw();
		}
		//固定敵描画
		for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
		{
			(*itrFixedEnemy)->Draw();
		}

		Object3d::PostDraw();

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

		//コネクトサークル描画
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			(*itrConnectCircle)->Draw();
		}

		//吸収円描画
		for (auto itrAbsorptionCircle = absorptionCircles.begin(); itrAbsorptionCircle != absorptionCircles.end(); itrAbsorptionCircle++)
		{
			(*itrAbsorptionCircle)->Draw();
		}

		//背景
		buckGround->Draw();

		Object3d::PostDraw();


		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//エネルギーポイント描画
		energy->Draw();

		//制限時間描画
		timeLimit->Draw();

		//デバッグテキスト描画
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}

	//全シーン共通の描画
	{
		//エフェクトの描画
		effects->Draw(cmdList);
	}
}

void GameScene::ResetGame()
{
	//プレイヤー初期化
	player->Reset();

	//レーザーをプレイヤーに追従させる
	laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
	laserSite->Update();

	//コア初期化
	core->Reset();

	//ボス戦中ではない
	isBossStage = false;

	//カメラ距離初期化
	cameraPos = { 0, 0, -100 };
	//カメラ距離イージング開始初期化
	cameraDisEaseStart = 0;
	//カメラ距離イージング終了初期化
	cameraDisEaseEnd = 0;
	//カメラ距離を変更中ではない
	isChangecameraDis = false;
	//カメラ距離イージングタイマー初期化
	cameraDisEaseTimer = 0;
	//画面シェイクしない
	isShake = false;
	//画面シェイク時間初期化
	ShakeTime = 0;

	//画面枠初期化
	frame->Reset();
	//枠のライン変更に合わせてオブジェクトの様々な境界線も変更
	XMFLOAT2 frameLine = frame->GetFrameLine();
	Player::SetMoveRange({ frameLine.x - 5, frameLine.y - 5 });
	PlayerBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
	EnemyBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
	Porta::SetReflectionLine({ frameLine.x - 3, frameLine.y - 2 });
	BossEnemy::SetFrameLine({ frameLine.x - 3, frameLine.y - 2 });


	//スポーンパターン
	spawnTimer = 0;//スポーンタイマー初期化
}

void GameScene::PowerUpPlayerBullet()
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//プレイヤーが被弾したら溜め状態を無効にする
	if (player->GetIsDamege())
	{
		//溜めを終了
		isBulletPowerUp = false;
		//溜めタイマーを初期化
		bulletPowerUpTimer = 0;

		return;
	}

	//ボタン入力を終了したら発射
	if (!(input->PushKey(DIK_SPACE) || Xinput->PushButton(XInputManager::PAD_RB)))
	{
		//溜めを終了
		isBulletPowerUp = false;
		//溜めタイマーを初期化
		bulletPowerUpTimer = 0;
		//弾を発射
		ShotPlayerBullet(bulletPowerLevel);
		//弾の威力をあげていれば
		if (bulletPowerLevel >= 1)
		{
			//エネルギーポイントを消費する
			for (int i = 0; i < bulletPowerLevel; i++)
			{
				energy->UseTwentyPercent();
			}
		}

		return;
	}

	if (input->PushKey(DIK_SPACE) || Xinput->PushButton(XInputManager::PAD_RB))
	{
		//威力アップに必要なエネルギーポイントを持っていたら溜め可能
		const int percent = (bulletPowerLevel + 1) * 20;
		if (energy->CheckPercent(percent))
		{
			//溜めタイマーを更新
			bulletPowerUpTimer++;
		}

		//一定時間溜めたら弾の威力をアップさせる
		const int levelUpTime = 60;
		if (bulletPowerUpTimer >= levelUpTime)
		{
			bulletPowerLevel++;

			//溜めタイマーを初期化
			bulletPowerUpTimer = 0;
		}
	}
}

void GameScene::ShotPlayerBullet(const int bulletPowerLevel)
{
	//プレイヤーウエポンの座標と角度を弾も持つ
	XMFLOAT3 pos = player->GetWeaponPosition();
	XMFLOAT3 rota = player->GetWeaponRotation();

	//弾の発射をプレイヤーウエポンの真上に設定
	float angle = DirectX::XMConvertToRadians(rota.z + 90);
	pos.x += 8.0f * cosf(angle);
	pos.y += 8.0f * sinf(angle);

	//弾の威力を設定
	int power = 0;
	if (bulletPowerLevel == 0) { power = 10; }
	else if (bulletPowerLevel == 1) { power = 20; }
	else if (bulletPowerLevel == 2) { power = 30; }
	else if (bulletPowerLevel == 3) { power = 40; }
	else if (bulletPowerLevel == 4) { power = 50; }
	else if (bulletPowerLevel == 5) { power = 60; }

	//弾発射
	for (int i = 0; i < playerBulletNum; i++)
	{
		//発射されていたら飛ばす
		if (playerBullet[i]->GetIsAlive()) { continue; }

		//弾発射
		playerBullet[i]->BulletStart(pos, rota, power);

		//1つ発射したら抜ける(一気に全ての弾を撃ってしまうため)
		return;
	}
}

void GameScene::SpawnEnemyManager(bool isBossStage, int wave)
{
	//ボス戦以外の時はカウントを進める。ボス戦になったら数値をリセットする
	if (!isBossStage)
	{
		spawnTimer++;//敵生成カウント更新
	}
	else
	{
		spawnTimer = 0;//敵生成カウントをリセット
	}

	//WAVE毎に出現間隔調整
	if ((wave % 3) == 1)
	{
		spawnInterval = 300;
	}
	else if ((wave % 3) == 2)
	{
		spawnInterval = 450;
	}
	else if ((wave % 3) == 0)
	{
		spawnInterval = 600;
	}

	//spawnIntervalフレーム毎に敵生成
	if (((spawnTimer + ((spawnInterval / 3) * 2)) % spawnInterval) == 0)
	{
		spawnPattern = ((spawnTimer + ((spawnInterval / 3) * 2)) / spawnInterval);//スポーンパターン計算

		//初回スポーン時にセット決定
		if (spawnPattern == 1)
		{
			spawnSet = 1;
			//spawnSet = (int)(rand() % 10) + 1;//スポーンセット計算
		}

		SpawnEnemyGroup(spawnPattern, spawnSet, (wave % 3));//敵生成
	}
}

void GameScene::SpawnEnemyGroup(int spawnPattern, int spawnSet, int wave)
{
	//生成時にスポーン座標と移動後の座標を決める(ガルタとガルタタ)
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	//生成時に初期座標と移動方向を決める(ポルタとチャロ)
	XMFLOAT3 startPos = {};
	float angle = 0;

	//ウェーブ、パターン、セットから出現パターンを確定
	if (wave == 1)
	{
		if (spawnPattern == 1)
		{
			if (spawnSet == 1)
			{
				//上にガルタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = -20;
				stayPos.y = 15;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = 20;
				stayPos.y = 15;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 2)
		{
			if (spawnSet == 1)
			{
				//中央にガルタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = -15;
				stayPos.y = -8;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = 15;
				stayPos.y = -8;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 3)
		{
			if (spawnSet == 1)
			{
				//左のガルタタ1体、右にガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = -40;
				stayPos.y = -5;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = 40;
				stayPos.y = -5;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 4)
		{
			if (spawnSet == 1)
			{
				//上からポルタ4体
				startPos = { 10, 65, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -10, 65, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 30, 65, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -30, 65, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 5)
		{
			if (spawnSet == 1)
			{
				//下にガルタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = -40;
				stayPos.y = -30;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = 40;
				stayPos.y = -30;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 6)
		{
			if (spawnSet == 1)
			{
				//右上にガルタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = 60;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = 60;
				stayPos.y = 20;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 7)
		{
			if (spawnSet == 1)
			{
				//右下にガルタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = 60;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = 60;
				stayPos.y = -20;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 8)
		{
			if (spawnSet == 1)
			{
				//上からポルタ4体
				startPos = { 115, 10, 0 }; angle = 120;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 115, 30, 0 }; angle = 120;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 115, -10, 0 }; angle = 60;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 115, -30, 0 }; angle = 60;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 9)
		{
			if (spawnSet == 1)
			{
				//左下にガルタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = -60;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = -60;
				stayPos.y = -20;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 10)
		{
			if (spawnSet == 1)
			{
				//左上にガルタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = -60;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = -60;
				stayPos.y = 20;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 11)
		{
			BossStageStart();
		}
	}
	else if (wave == 2)
	{
		if (spawnPattern == 1)
		{
			if (spawnSet == 1)
			{
				//右にガルタ2体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 80;
				stayPos.y = 50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 80;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 70;
				stayPos.y = 0;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//右にガルタ1体、ガルタタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 110;
				stayPos.y = 30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 110;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 90;
				stayPos.y = 0;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 2)
		{
			if (spawnSet == 1)
			{
				//左にガルタ2体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -80;
				stayPos.y = 50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -80;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -70;
				stayPos.y = 0;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//左にガルタ1体、ガルタタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -110;
				stayPos.y = 30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -110;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -90;
				stayPos.y = 0;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 3)
		{
			if (spawnSet == 1)
			{
				//左右にガルタタ1体、下にガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -90;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 90;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 0;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//左右にガルタタ1体、下にガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -100;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 100;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 0;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 4)
		{
			if (spawnSet == 1)
			{
				//左からポルタ6体、チャロ1体
				startPos = { -165, 20, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, 40, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, 60, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, -20, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, -40, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, -60, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, 0, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 5)
		{
			if (spawnSet == 1)
			{
				//左にガルタ2体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -120;
				stayPos.y = 0;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -110;
				stayPos.y = 20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -100;
				stayPos.y = 40;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//左にガルタ2体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -140;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -130;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -120;
				stayPos.y = -10;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 6)
		{
			if (spawnSet == 1)
			{
				//右にガルタ2体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 120;
				stayPos.y = 0;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 110;
				stayPos.y = 20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 100;
				stayPos.y = 40;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//右にガルタ2体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 140;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 130;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 120;
				stayPos.y = -10;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 7)
		{
			if (spawnSet == 1)
			{
				//上にガルタ2体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 20;
				stayPos.y = 65;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -20;
				stayPos.y = 65;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 0;
				stayPos.y = 55;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//上にガルタ2体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 30;
				stayPos.y = 35;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -30;
				stayPos.y = 35;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 0;
				stayPos.y = 25;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 8)
		{
			if (spawnSet == 1)
			{
				//上からポルタ6体、チャロ2体
				startPos = { 30, 100, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 50, 100, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 70, 100, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -30, 100, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -50, 100, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -70, 100, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, 0, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
				startPos = { 165, 0, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 9)
		{
			if (spawnSet == 1)
			{
				//左上にガルタ4体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -135;
				stayPos.y = 60;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -105;
				stayPos.y = 60;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -120;
				stayPos.y = 50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -135;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				/*
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -105;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
				*/
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 10)
		{
			if (spawnSet == 1)
			{
				//右上にガルタ4体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 135;
				stayPos.y = 60;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 105;
				stayPos.y = 60;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 120;
				stayPos.y = 50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 135;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				/*
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 105;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
				*/
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 11)
		{
			BossStageStart();
		}
	}
	else if (wave == 0)
	{
		if (spawnPattern == 1)
		{
			if (spawnSet == 1)
			{
				//上下にガルタ4体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 30;
				stayPos.y = 85;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -30;
				stayPos.y = 85;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 10;
				stayPos.y = 95;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -10;
				stayPos.y = 95;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 0;
				stayPos.y = 75;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//上下にガルタ4体、ガルタタ1体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 30;
				stayPos.y = -85;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -30;
				stayPos.y = -85;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 10;
				stayPos.y = -95;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -10;
				stayPos.y = -95;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 0;
				stayPos.y = -75;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 2)
		{
			if (spawnSet == 1)
			{
				//左にガルタ5体、ガルタタ3体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -150;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -150;
				stayPos.y = 20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -150;
				stayPos.y = 0;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -150;
				stayPos.y = -20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -150;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -130;
				stayPos.y = 30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -130;
				stayPos.y = 0;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -130;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 3)
		{
			if (spawnSet == 1)
			{
				//右にガルタ5体、ガルタタ3体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 150;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 150;
				stayPos.y = 20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 150;
				stayPos.y = 0;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 150;
				stayPos.y = -20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 150;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 130;
				stayPos.y = 30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 130;
				stayPos.y = 0;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 130;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 4)
		{
			if (spawnSet == 1)
			{
				//上からポルタ10体、チャロ2体
				startPos = { 90, 130, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 110, 130, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -90, 130, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -110, 130, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 30, 130, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 50, 130, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 70, 130, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -30, 130, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -50, 130, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -70, 130, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -70, -130, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
				startPos = { 70, -130, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 5)
		{
			if (spawnSet == 1)
			{
				//左にガルタ6体、ガルタタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -140;
				stayPos.y = 50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -170;
				stayPos.y = 70;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -170;
				stayPos.y = 30;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -140;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -170;
				stayPos.y = -70;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -170;
				stayPos.y = -30;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -155;
				stayPos.y = 50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -155;
				stayPos.y = -50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 6)
		{
			if (spawnSet == 1)
			{
				//右にガルタ6体、ガルタタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 140;
				stayPos.y = 50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 170;
				stayPos.y = 70;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 170;
				stayPos.y = 30;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 140;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 170;
				stayPos.y = -70;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 170;
				stayPos.y = -30;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 155;
				stayPos.y = 50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 155;
				stayPos.y = -50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 7)
		{
			if (spawnSet == 1)
			{
				//下にガルタ6体、ガルタタ4体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 50;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 70;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 90;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 60;
				stayPos.y = -60;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 80;
				stayPos.y = -60;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -50;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -70;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -90;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -60;
				stayPos.y = -60;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -80;
				stayPos.y = -60;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 8)
		{
			if (spawnSet == 1)
			{
				//上からチャロ2体
				startPos = { 50, 130, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
				startPos = { -50, 130, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
				//左からポルタ6体
				startPos = { -205, 40, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -205, 60, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -205, 80, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -205, -40, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -205, -60, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -205, -80, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				//右からポルタ6体
				startPos = { 205, 40, 0 }; angle = 120;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 205, 60, 0 }; angle = 120;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 205, 80, 0 }; angle = 120;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 205, -40, 0 }; angle = 60;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 205, -60, 0 }; angle = 60;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 205, -80, 0 }; angle = 60;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 9)
		{
			if (spawnSet == 1)
			{
				//上にガルタタ6体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 80;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 100;
				stayPos.y = 90;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 120;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 70;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 50;
				stayPos.y = 90;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 30;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 10)
		{
			if (spawnSet == 1)
			{
				//上にガルタタ6体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -80;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -100;
				stayPos.y = 90;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -120;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -70;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -50;
				stayPos.y = 90;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -30;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 11)
		{
			BossStageStart();
		}
	}
}

void GameScene::SpawnGaruEnemy(int spawnPattern, int wave)
{
	//生成時にスポーン座標と移動後の座標を決める
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	if (spawnPattern == 0)
	{
		//ランダム生成
		spawnPos.x = (float)(rand() % 400 - 200);
		spawnPos.y = 100;
		stayPos.x = (float)(rand() % 400 - 200);
		stayPos.y = (float)(rand() % 240 - 120);

		//20%の確率でガルタタ　80%の確率でガルタを生成
		int enemyKindRand = rand() % 5;
		if (enemyKindRand == 0)
		{
			garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
		}
		else
		{
			garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
		}
	}
}

void GameScene::GaruEnemyShotBullet(GaruEnemy* garuEnemy)
{
	//発射位置は敵の中心
	XMFLOAT3 startPos = garuEnemy->GetPosition();

	//所属するグループがガルタの場合、プレイヤー狙い弾を発射
	if (garuEnemy->GetGroup() == GaruEnemy::GaruGroup::Garuta)
	{
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//弾が発射されていたら飛ばす
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//標的の座標（プレイヤーの座標）をセット
			XMFLOAT3 targetPos = player->GetPosition();

			//ターゲットに向けて弾発射
			enemyBullet[i]->AimBulletStart(startPos, targetPos);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}
	}
	//所属するグループがガルタタの場合、4方向に弾を飛ばす
	else if (garuEnemy->GetGroup() == GaruEnemy::GaruGroup::Garutata)
	{
		//左上の弾発射
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//左上に角度を設定
			float angle = 45;

			//左上に弾発射
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}
		//左下の弾発射
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//左下に角度を設定
			float angle = 135;

			//左下に弾発射
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}
		//右下の弾発射
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//右下に角度を設定
			float angle = 225;

			//右下に弾発射
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}
		//右上の弾発射
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//右上に角度を設定
			float angle = 315;

			//右上に弾発射
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}
	}
}

void GameScene::SpawnCharoPorta(int spawnPattern, int wave)
{
	//生成時に初期座標と移動方向を決める
	XMFLOAT3 startPos = {};
	float angle = 0;

	if (spawnPattern == 0)
	{
		//4パターンのランダムで初期座標と移動方向をセット
		int posAngleRand = rand() % 4;
		if (posAngleRand == 0) { startPos = { 0, -65, 0 }; angle = 30; }
		else if (posAngleRand == 1) { startPos = { 115, 0, 0 }; angle = 120; }
		else if (posAngleRand == 2) { startPos = { 0, 65, 0 }; angle = 210; }
		else if (posAngleRand == 3) { startPos = { -115, 0, 0 }; angle = 300; }

		//20%の確率でハゲタタ　80%の確率でハゲタを生成
		int enemyKindRand = rand() % 5;
		if (enemyKindRand == 0)
		{
			charoEnemys.push_back(Charo::Create(charoModel, startPos));
		}
		else
		{
			portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
		}
	}
}

void GameScene::BossStageStart()
{
	//どのボスを動かすか
	if (wave >= 1) { moveBossNumber = BossEnemy::BossName::Tuff; }

	//ボスを初期化
	bossEnemy[moveBossNumber]->Reset();

	//ボス戦開始
	isBossStage = true;
}

void GameScene::BossEnemyShotBullet(int moveBossNumber)
{
	//発射位置は敵の中心
	XMFLOAT3 startPos = bossEnemy[moveBossNumber]->GetPosition();

	//ボスがタッフの場合
	if (moveBossNumber == BossEnemy::BossName::Tuff)
	{
		//3WAY弾発射シーンの場合
		if (bossEnemy[moveBossNumber]->GetAction() == Tuff::MovementPattern::ThreeWayBullet)
		{
			//弾の弾速を速くする
			float bulletSpeed = 1.0f;
			//標的をプレイヤーに設定
			XMFLOAT3 targetPos = player->GetPosition();
			//弾発射1
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//標的に向かって発射
				enemyBullet[i]->AimBulletStart(startPos, targetPos, bulletSpeed);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射2
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//発射角度を設定する（標的に向かって一直線）
				float radian = atan2f(targetPos.y - startPos.y, targetPos.x - startPos.x);
				//オブジェクトの角度を設定
				float degree = DirectX::XMConvertToDegrees(radian);
				//右方向を0に設定するために90ずらす
				degree -= 90;

				//3WAYの左の弾
				degree += 15;

				//標的から少し左にずらして弾発射
				enemyBullet[i]->StraightBulletStart(startPos, degree, bulletSpeed);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射3
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//発射角度を設定する（標的に向かって一直線）
				float radian = atan2f(targetPos.y - startPos.y, targetPos.x - startPos.x);
				//オブジェクトの角度を設定
				float degree = DirectX::XMConvertToDegrees(radian);
				//右方向を0に設定するために90ずらす
				degree -= 90;

				//3WAYの右の弾
				degree -= 15;

				//標的から少し右にずらして弾発射
				enemyBullet[i]->StraightBulletStart(startPos, degree, bulletSpeed);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
		}
		//5WAY弾発射シーンの場合
		else if (bossEnemy[moveBossNumber]->GetAction() == Tuff::MovementPattern::FiveWayBullet)
		{
			//弾発射1
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//左上に角度を設定
				float angle = 0;

				//左上に弾発射
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射2
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//左下に角度を設定
				float angle = 72;

				//左下に弾発射
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射3
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//右下に角度を設定
				float angle = 144;

				//右下に弾発射
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射4
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//左上に角度を設定
				float angle = 216;

				//左上に弾発射
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射5
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//左下に角度を設定
				float angle = 288;

				//左下に弾発射
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
		}
	}
}

void GameScene::BossImpactFallEnemy()
{
	//生成時にスポーン座標と移動後の座標を決める
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	//生成時に初期座標と移動方向を決める
	XMFLOAT3 startPos = {};
	float angle = 0;

	//ランダム生成(テキトーにガルタを2体降らせる)
	for (int i = 0; i < 2; i++)
	{
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = (float)(rand() % 180 - 90);
		stayPos.y = (float)(rand() % 110 - 55);
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
	}

	//ランダム生成(テキトーにガルタタを1体降らせる)
	spawnPos.x = (float)(rand() % 200 - 100);
	spawnPos.y = 100;
	stayPos.x = (float)(rand() % 180 - 90);
	stayPos.y = (float)(rand() % 110 - 55);
	garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

	//ランダム生成(テキトーにポルタを1体降らせる)
	//4パターンのランダムでポルタの初期座標と移動方向をセット
	int startPosRand = rand() % 3;

	if (startPosRand == 0) { startPos = { -70, 65, 0 }; angle = 210; }
	else if (startPosRand == 1) { startPos = { -50, 65, 0 }; angle = 210; }
	else if (startPosRand == 2) { startPos = { -30, 65, 0 }; angle = 210; }
	else if (startPosRand == 3) { startPos = { -10, 65, 0 }; angle = 210; }
	portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));

	//ランダム生成(テキトーにポルタを1体降らせる)
	//4パターンのランダムでポルタの初期座標と移動方向をセット
	int startPosRand2 = rand() % 3;

	if (startPosRand2 == 0) { startPos = { 70, 65, 0 }; angle = 150; }
	else if (startPosRand2 == 1) { startPos = { 50, 65, 0 }; angle = 150; }
	else if (startPosRand2 == 2) { startPos = { 30, 65, 0 }; angle = 150; }
	else if (startPosRand2 == 3) { startPos = { 10, 65, 0 }; angle = 150; }
	portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));

	/*
	//ランダム生成(テキトーに5体降らせる)
	for (int i = 0; i < 5; i++)
	{
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = (float)(rand() % 200 - 100);
		stayPos.y = (float)(rand() % 120 - 60);

		//20%の確率でガルタタ　80%の確率でガルタを生成
		int enemyKindRand = rand() % 5;
		if (enemyKindRand == 0)
		{
			garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
		}
		else
		{
			garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
		}
	}
	*/
}

void GameScene::CreatePinStart()
{
	//エネルギーポイントが最大値の20%以下なら関数を抜ける
	if (!energy->CheckPercent(20)) { return; }

	//プレイヤーと既存ピンが衝突していたら関数を抜ける
	for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
	{
		//衝突用に座標と半径の大きさを借りる
		XMFLOAT3 playerPos = player->GetPosition();
		float playerSize = player->GetScale().x;
		XMFLOAT3 pinPos = (*itrPin)->GetPosition();
		float pinSize = (*itrPin)->GetScale().x;

		//衝突判定を計算
		bool isCollision = Collision::CheckCircle2Circle(
			playerPos, playerSize, pinPos, pinSize);

		//プレイヤーとピンが衝突状態なら抜ける
		if (isCollision) { return; }
	}

	//ピン作成状態にする
	isCreatePin = true;
	//プレイヤーの行動を完全に止める
	player->SetIsStop(true);
	//ピン作成タイマーを初期化
	createPinTimer = 0;
}

void GameScene::CreatePin()
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//ボタン入力を途中で終了。または被弾したら
	//足りなくなったらピン作成を終了する
	if (!(input->PushKey(DIK_A) || Xinput->PushButton(XInputManager::PAD_A)) || player->GetIsDamege())
	{
		//ピン作成状態を終了
		isCreatePin = false;
		//プレイヤーの行動制限を解除
		player->SetIsStop(false);
		//ピン作成タイマーを初期化
		createPinTimer = 0;

		return;
	}
	else if (input->PushKey(DIK_A) || Xinput->PushButton(XInputManager::PAD_A))
	{
		//ピン作成タイマーを更新
		createPinTimer++;

		//ピン作成タイマーが指定の時間まで進んだら
		const int createEndTime = 100;
		if (createPinTimer >= createEndTime)
		{
			//新しいピンを作る
			pins.push_back(Pin::Create(happyModel, player->GetPosition()));
			//新しいピンから出るコネクトサークルを生成
			auto itrPin = pins.end();
			itrPin--;
			connectCircles.push_back(PinCircle::Create(circleModel, *itrPin));

			//ピン作成状態を終了
			isCreatePin = false;
			//プレイヤーの行動制限を解除
			player->SetIsStop(false);
			//ピン作成タイマーを初期化
			createPinTimer = 0;
			//エネルギーポイントを20%消費
			energy->UseTwentyPercent();
		}
	}
}

void GameScene::CreatePowerUpLine(ConnectCircle* startPoint, ConnectCircle* endPoint)
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

	//線が引かれたときのエフェクト
	StageEffect::SetConnectLine(startPoint->GetPosition(), endPoint->GetPosition());
}

void GameScene::CameraUpdate(Camera* camera)
{
	//カメラ距離変更
	if (isChangecameraDis)
	{
		//カメラ距離変更を行う時間
		const int changeTime = 100;

		//カメラ距離変更タイマー更新
		cameraDisEaseTimer++;

		//イージング計算用の時間
		float easeTimer = (float)cameraDisEaseTimer / changeTime;

		//イージングでサイズ変更
		float newDistance = Easing::InQuint(cameraDisEaseStart, cameraDisEaseEnd, easeTimer);

		//カメラ距離を更新
		cameraPos.z = newDistance;

		//タイマーが指定した時間になったら
		if (cameraDisEaseTimer >= changeTime)
		{
			//サイズ変更状態終了
			isChangecameraDis = false;
		}
	}

	//シェイク
	if (isShake)
	{
		camera->StartCameraShake(5);
		ShakeTime++;

		//シェイク開始から一定時間経ったら
		if (ShakeTime > 10) {
			isShake = false;
			ShakeTime = 0;

			//シェイク終了
			camera->EndCameraShake();
		}
	}
	//カメラ距離をセット
	camera->TpsCamera(cameraPos);
	//カメラ更新
	camera->Update();
}

void GameScene::SetChangeCameraDistance(float distance)
{
	//変更前のカメラ距離をセット
	cameraDisEaseStart = cameraPos.z;
	//変更後のカメラ距離をセット
	cameraDisEaseEnd = distance;
	//カメラ距離変更タイマーを初期化
	cameraDisEaseTimer = 0;
	//サイズを変更中にする
	isChangecameraDis = true;
}