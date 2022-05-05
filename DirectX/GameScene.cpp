#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

#include "Straighter.h"
#include "Division.h"
#include "Releaser.h"
#include "Chaser.h"

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
	safe_delete(RBbuttonModel);

	//プレイヤー解放
	safe_delete(player);
	//衝撃波解放
	for (int i = 0; i < shockWaveNum; i++)
	{
		safe_delete(shockWave[i]);
	}
	//プレイヤー弾解放
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}
	//着弾地点解放
	safe_delete(landingPoint);

	//敵解放
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		safe_delete(*itrEnemy);
	}
	//敵のリスト解放
	enemys.clear();

	//壁解放
	safe_delete(wall);

	//エフェクトの解放
	safe_delete(effects);

	//背景の解放
	buckGround->AllDelete();
	safe_delete(buckGround);

	//コンボ解放
	safe_delete(combo);
	//制限時間解放
	safe_delete(timeLimit);
	//スコア解放
	safe_delete(breakScore);
	//巨大衝撃波用ゲージ解放
	safe_delete(shockWaveGauge);
	//制限時間回復用ゲージ解放
	safe_delete(timeLimitGauge);
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
	enemy01Model = Model::CreateFromOBJ("enemy");//敵01(ガルタ)のモデル
	enemyPoint01Model = Model::CreateFromOBJ("garutaspown");//敵01(ガルタ)の出現位置のモデル
	enemy02Model = Model::CreateFromOBJ("garutata");//敵02(ガルタタ)のモデル
	enemyPoint02Model = Model::CreateFromOBJ("garutataspown");//敵02(ガルタタ)の出現位置のモデル
	eBullModel = Model::CreateFromOBJ("enemybullet");//敵の弾のモデル
	deadEnemyModel = Model::CreateFromOBJ("desenemy");//死んだ敵のモデル
	initialCircleCoreModel = Model::CreateFromOBJ("initialcirclecore");;//固定敵のコアのモデル
	initialCircleSquareModel = Model::CreateFromOBJ("initialcirclesquare");//固定敵の外壁のモデル
	hexagonModel = Model::CreateFromOBJ("hexagon");//六角形のモデル
	happyModel = Model::CreateFromOBJ("happy");//タバコモデル
	portaModel = Model::CreateFromOBJ("porta");//ポルタのモデル
	charoModel = Model::CreateFromOBJ("charo");//チャロのモデル
	tuffModel = Model::CreateFromOBJ("tuff");//タッフのモデル
	frameModel = Model::CreateFromOBJ("frame");//フレームのモデル
	waveModel = Model::CreateFromOBJ("wave");//衝撃波のモデル
	RBbuttonModel = Model::CreateFromOBJ("RB");//RBボタンのモデル


	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/white1x1.png");
	Sprite::LoadTexture(2, L"Resources/number.png");
	Sprite::LoadTexture(3, L"Resources/combo.png");
	Sprite::LoadTexture(4, L"Resources/break.png");
	Sprite::LoadTexture(5, L"Resources/gaugeIn.png");
	Sprite::LoadTexture(6, L"Resources/gaugeOut.png");
	//デバッグテキスト生成
	DebugText::GetInstance()->Initialize(0);


	//プレイヤー生成
	player = Player::Create(pBodyModel, waveModel);
	//衝撃波生成
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i] = ShockWave::Create(waveModel);
	}
	//プレイヤー弾生成
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(eBullModel);
	}
	//着弾地点生成
	landingPoint = LandingPoint::Create(waveModel);

	//壁生成
	wall = Wall::Create(frameModel);
	XMFLOAT2 wallline = wall->GetWallLine();
	Player::SetMoveRange(wallline);
	PlayerBullet::SetDeadPos(wallline);
	LandingPoint::SetMoveRange(wallline);

	//エフェクト初期化
	effects = new StageEffect();
	effects->Initialize();

	//背景の初期化
	buckGround->Create(hexagonModel);

	//コンボ生成
	combo = Combo::Create(2, 3);
	//制限時間生成
	timeLimit = TimeLimit::Create(2);
	//スコア生成
	breakScore = BreakScore::Create(2, 4);
	//巨大衝撃波用ゲージ生成
	shockWaveGauge = BigShockWaveGauge::Create(6, 5);
	//制限時間回復用ゲージ生成
	timeLimitGauge = TimeLimitGauge::Create(6, 5);

	//サウンド用
	audio = new Audio();
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//プレイヤー更新
	player->Update();
	//一定間隔でプレイヤーから衝撃波発射
	if (player->AutoShockWaveStart(combo->GetCombo()))
	{
		PlayerShockWaveStart(player->GetPosition());
	}
	//ポイ捨て開始
	if (player->GetIsLitteringStart())
	{
		//プレイヤー弾発射
		ShotPlayerBullet();
	}

	//プレイヤー弾更新
	for (int i = 0; i < playerBulletNum; i++)
	{
		//更新処理
		playerBullet[i]->Update();

		//ポイ捨てから出る衝撃波発射
		if (playerBullet[i]->GetIsShockWaveStart())
		{
			LitteringShockWaveStart(playerBullet[i]->GetPosition());
		}
	}

	//着弾地点更新
	landingPoint->Update(player->GetPosition(), player->GetRotation());


	//巨大衝撃波発射
	if (input->TriggerKey(DIK_Z) || Xinput->TriggerButton(XInputManager::PAD_A))
	{
		//巨大衝撃波を発射
		BigShockWaveStart(player->GetPosition());
	}
	//衝撃波更新
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i]->Update();
	}

	//敵生成
	if (input->TriggerKey(DIK_1) || Xinput->TriggerButton(XInputManager::PAD_RT))
	{
		//直進敵スポーン
		SpawnStraighter();
	}
	if (input->TriggerKey(DIK_2) || Xinput->TriggerButton(XInputManager::PAD_LT))
	{
		//分裂敵スポーン
		SpawnDivision();
	}
	if (input->TriggerKey(DIK_3) || Xinput->TriggerButton(XInputManager::PAD_LB))
	{
		//放出敵スポーン
		SpawnReleaser();
	}
	if (input->TriggerKey(DIK_4) || Xinput->TriggerButton(XInputManager::PAD_RIGHT))
	{
		//追従敵スポーン
		SpawnChaser();
	}
	if (input->TriggerKey(DIK_5))
	{
		timeLimit->Recovery(5);
	}
	if (input->TriggerKey(DIK_6))
	{
		timeLimitGauge->AddPoint(10);
	}



	//敵更新
	Chaser::SetTargetPos(player->GetPosition());
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		//更新処理
		(*itrEnemy)->Update();

		//敵が生きていなければ飛ばす
		if (!(*itrEnemy)->GetIsAlive()) { continue; }

		//敵生成フラグがtrueのとき敵を生成する
		if ((*itrEnemy)->GetIsCreateEnemy())
		{
			SpawnEnemyToEnemy(*itrEnemy);
		}

		//壁がある場合
		if (wall->GetIsAlive())
		{
			//ノックバック中の敵のみ壁との判定を取る
			if ((*itrEnemy)->GetIsKnockBack())
			{
				//壁に当たったか判定を取る
				if ((*itrEnemy)->IsCollisionWall(wall->GetWallLine()))
				{
					//死亡
					(*itrEnemy)->Dead();

					//壁にもダメージを与える
					int damagePower = (*itrEnemy)->GetPower();
					wall->Damage(damagePower);

					//コンボを増やす
					combo->AddCombo();
					//制限時間回復ゲージを増やす
					timeLimitGauge->AddPoint(combo->GetCombo());
				}
			}
		}
		//壁がない場合
		else
		{
			//画面外に出たら削除する
			XMFLOAT3 pos = (*itrEnemy)->GetPosition();
			XMFLOAT2 wallLine = wall->GetWallLine();
			if (pos.x <= -wallLine.x || pos.x >= wallLine.x ||
				pos.y <= -wallLine.y || pos.y >= wallLine.y)
			{
				(*itrEnemy)->SetDelete();
			}
		}

		//衝突用に座標と半径の大きさを借りる
		XMFLOAT3 enemyPos = (*itrEnemy)->GetPosition();
		float enemySize = (*itrEnemy)->GetScale().x;

		//プレイヤーと敵の当たり判定
		//プレイヤーがダメージ状態でないなら
		if (!player->GetIsDamege())
		{
			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//敵とプレイヤーが衝突状態
			if (isCollision)
			{
				//プレイヤーはダメージを喰らう
				player->Damage();
				player->SetKnockback();

				//敵も死亡
				(*itrEnemy)->SetDelete();

				//コンボ終了
				combo->LostCombo();

				isShake = true;
			}
		}

		//衝撃波と敵の当たり判定
		for (int i = 0; i < shockWaveNum; i++)
		{
			//衝撃波が発射状態でなければ飛ばす
			if (!shockWave[i]->GetIsAlive()) { continue; }

			//当たり判定用変数
			XMFLOAT3 wavePos = shockWave[i]->GetPosition();
			float waveSize = shockWave[i]->GetRadius();

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, enemyPos, enemySize);

			//衝撃波と敵が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//既に衝突したことがあるか確認(衝突中ダメージを食らい続けてしまうため)
			if (shockWave[i]->IsKnowEnemy((*itrEnemy))) { continue; }

			//敵をノックバックで飛ばす
			float angle = atan2f(enemyPos.y - wavePos.y, enemyPos.x - wavePos.x);
			int powerLevel = shockWave[i]->GetPowerLevel();
			int shockWaveGroup = shockWave[i]->GetGroup();
			(*itrEnemy)->SetKnockBack(angle, powerLevel, shockWaveGroup);

			//コンボを増やす
			combo->AddCombo();
			//制限時間回復ゲージを増やす
			timeLimitGauge->AddPoint(combo->GetCombo());
		}
	}

	//敵削除
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrEnemy)->GetIsDelete())
		{
			//要素を削除、リストから除外する
			safe_delete(*itrEnemy);
			itrEnemy = enemys.erase(itrEnemy);
			continue;
		}
		//for分を回す
		itrEnemy++;
	}

	//壁更新
	wall->Update();
	if (wall->GetTriggerBreak())
	{
		breakScore->AddScore();
	}

	{
		if (wall->GetHP() == 20) { DebugText::GetInstance()->Print("WALL HP:20", 100, 600); }
		else if (wall->GetHP() == 19) { DebugText::GetInstance()->Print("WALL HP:19", 100, 600); }
		else if (wall->GetHP() == 18) { DebugText::GetInstance()->Print("WALL HP:18", 100, 600); }
		else if (wall->GetHP() == 17) { DebugText::GetInstance()->Print("WALL HP:17", 100, 600); }
		else if (wall->GetHP() == 16) { DebugText::GetInstance()->Print("WALL HP:16", 100, 600); }
		else if (wall->GetHP() == 15) { DebugText::GetInstance()->Print("WALL HP:15", 100, 600); }
		else if (wall->GetHP() == 14) { DebugText::GetInstance()->Print("WALL HP:14", 100, 600); }
		else if (wall->GetHP() == 13) { DebugText::GetInstance()->Print("WALL HP:13", 100, 600); }
		else if (wall->GetHP() == 12) { DebugText::GetInstance()->Print("WALL HP:12", 100, 600); }
		else if (wall->GetHP() == 11) { DebugText::GetInstance()->Print("WALL HP:11", 100, 600); }
		else if (wall->GetHP() == 10) { DebugText::GetInstance()->Print("WALL HP:10", 100, 600); }
		else if (wall->GetHP() == 9) { DebugText::GetInstance()->Print("WALL HP:9", 100, 600); }
		else if (wall->GetHP() == 8) { DebugText::GetInstance()->Print("WALL HP:8", 100, 600); }
		else if (wall->GetHP() == 7) { DebugText::GetInstance()->Print("WALL HP:7", 100, 600); }
		else if (wall->GetHP() == 6) { DebugText::GetInstance()->Print("WALL HP:6", 100, 600); }
		else if (wall->GetHP() == 5) { DebugText::GetInstance()->Print("WALL HP:5", 100, 600); }
		else if (wall->GetHP() == 4) { DebugText::GetInstance()->Print("WALL HP:4", 100, 600); }
		else if (wall->GetHP() == 3) { DebugText::GetInstance()->Print("WALL HP:3", 100, 600); }
		else if (wall->GetHP() == 2) { DebugText::GetInstance()->Print("WALL HP:2", 100, 600); }
		else if (wall->GetHP() == 1) { DebugText::GetInstance()->Print("WALL HP:1", 100, 600); }
		else if (wall->GetHP() <= 0) { DebugText::GetInstance()->Print("WALL BREAK", 100, 600); }
	}

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 100, 250);
	DebugText::GetInstance()->Print("RB:ShockWave", 100, 300);
	DebugText::GetInstance()->Print("RT:SpawnStraighter", 100, 350);
	DebugText::GetInstance()->Print("LT:SpawnDivision", 100, 400);
	DebugText::GetInstance()->Print("LB:SpawnReleaser", 100, 450);
	DebugText::GetInstance()->Print("RIGHT:SpawnChaser", 100, 500);


	//エフェクトの更新
	effects->Update(camera);

	//背景更新
	buckGround->Update();

	//コンボ更新
	if (Xinput->TriggerButton(XInputManager::PAD_START)) {
		combo->AddCombo();
		//制限時間回復ゲージを増やす
		timeLimitGauge->AddPoint(combo->GetCombo());
	}
	combo->Update();
	//制限時間回復用ゲージ更新
	timeLimitGauge->Update();
	//制限時間回復ゲージが最大になったら
	if (timeLimitGauge->GetIsGaugeMax())
	{
		//制限時間5秒回復
		timeLimit->Recovery(5);
		//ポイントを消費
		timeLimitGauge->UsePoint();
	}

	//制限時間更新
	timeLimit->Update();
	//スコア更新
	breakScore->Update();
	//巨大衝撃波用ゲージ更新
	shockWaveGauge->Update(combo->GetCombo());

	//カメラ更新
	CameraUpdate(camera);

	input = nullptr;
}

void GameScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	{
		//オブジェクト描画
		Object3d::PreDraw(cmdList);

		//壁描画
		wall->Draw();

		//プレイヤー描画
		player->Draw();

		//衝撃波描画
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Draw();
		}

		//プレイヤー弾更新
		for (int i = 0; i < playerBulletNum; i++)
		{
			//更新処理
			playerBullet[i]->Draw();
		}
		//着弾地点描画
		landingPoint->Draw();

		//敵描画
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		//背景
		buckGround->Draw();

		Object3d::PostDraw();


		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//コンボ描画
		combo->Draw();
		//制限時間回復用ゲージ描画
		timeLimitGauge->Draw();
		//制限時間描画
		timeLimit->Draw();
		//スコア描画
		breakScore->Draw();
		//巨大衝撃波用ゲージ描画
		shockWaveGauge->Draw();

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

	//カメラ距離初期化
	cameraPos = { 0, 0, -200 };
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

	//壁のライン変更に合わせてオブジェクトの様々な境界線も変更
	XMFLOAT2 wallLine = wall->GetWallLine();
	Player::SetMoveRange(wallLine);
}

void GameScene::PlayerShockWaveStart(XMFLOAT3 pos)
{
	//発射されていたら抜ける
	if (shockWave[0]->GetIsAlive()) { return; }

	//衝撃波発射
	shockWave[0]->PlayerWaveStart(pos);

	//画面シェイク
	isShake = true;
}

void GameScene::LitteringShockWaveStart(XMFLOAT3 pos)
{
	for (int i = 1; i <= 3; i++)
	{
		//発射されていたら飛ばす
		if (shockWave[i]->GetIsAlive()) { continue; }

		//衝撃波発射
		shockWave[i]->LitteringWaveStart(pos);

		//画面シェイク
		isShake = true;

		//一発発射したら抜ける
		break;
	}
}

void GameScene::BigShockWaveStart(XMFLOAT3 pos)
{
	//発射されていたら抜ける
	if (shockWave[4]->GetIsAlive()) { return; }

	//コンボ数が足りない場合は抜ける
	if (combo->GetCombo() <= 5) { return; }
	//コンボ数に応じて巨大衝撃波の威力を変更
	int shockWavePowerLevel = 0;
	if (combo->GetCombo() <= 10) { shockWavePowerLevel = 1; }
	else if (combo->GetCombo() <= 15) { shockWavePowerLevel = 2; }
	else { shockWavePowerLevel = 3; }

	//巨大衝撃波発射
	shockWave[4]->BigWaveStart(pos, shockWavePowerLevel);

	//巨大衝撃波を発射した場合コンボを終了する
	combo->LostCombo();

	//画面シェイク
	isShake = true;
}

void GameScene::ShotPlayerBullet()
{
	//ポイ捨て衝撃波が全て消えるまで弾をうてないようにする
	if (shockWave[1]->GetIsAlive() || shockWave[2]->GetIsAlive() || shockWave[3]->GetIsAlive()) { return; }

	//プレイヤーウエポンの座標と角度を弾も持つ
	XMFLOAT3 pos = player->GetPosition();
	float angle = player->GetRotation().z;

	//左側の弾発射
	for (int i = 0; i < playerBulletNum; i++)
	{
		//発射されていたら飛ばす
		if (playerBullet[i]->GetIsAlive()) { continue; }

		//弾発射
		playerBullet[i]->BulletStart(pos, angle - 30);

		//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
		break;
	}

	//真ん中の弾発射
	for (int i = 0; i < playerBulletNum; i++)
	{
		//発射されていたら飛ばす
		if (playerBullet[i]->GetIsAlive()) { continue; }

		//弾発射
		playerBullet[i]->BulletStart(pos, angle);

		//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
		break;
	}

	//右側の弾発射
	for (int i = 0; i < playerBulletNum; i++)
	{
		//発射されていたら飛ばす
		if (playerBullet[i]->GetIsAlive()) { continue; }

		//弾発射
		playerBullet[i]->BulletStart(pos, angle + 30);

		//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
		break;
	}
}

void GameScene::SpawnStraighter()
{
	//生成時に初期座標と移動方向を決める
	XMFLOAT3 startPos = {};
	float angle = 0;

	XMFLOAT2 startLine = wall->GetWallLine();
	startLine.x += 5;
	startLine.y += 5;

	//4パターンのランダムで初期座標と移動方向をセット
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, -startLine.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLine.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLine.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { -startLine.x, 0, 0 }; angle = 300; }

	//直進敵を生成
	enemys.push_back(Straighter::Create(portaModel, startPos, angle));
}

void GameScene::SpawnDivision()
{
	//生成時に初期座標と移動方向を決める
	XMFLOAT3 startPos = {};
	float angle = 0;

	XMFLOAT2 startLine = wall->GetWallLine();
	startLine.x += 5;
	startLine.y += 5;

	//4パターンのランダムで初期座標と移動方向をセット
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, -startLine.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLine.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLine.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { -startLine.x, 0, 0 }; angle = 300; }

	//分裂敵を生成
	enemys.push_back(Division::Create(portaModel, startPos, angle));
}

void GameScene::SpawnReleaser()
{
	//生成時に初期座標と移動方向を決める
	XMFLOAT3 startPos = {};
	float angle = 0;

	XMFLOAT2 startLine = wall->GetWallLine();
	startLine.x += 5;
	startLine.y += 5;

	//4パターンのランダムで初期座標と移動方向をセット
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, -startLine.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLine.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLine.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { -startLine.x, 0, 0 }; angle = 300; }

	//停止地点をランダム生成
	XMFLOAT3 stayPos = {};
	stayPos.x = (float)(rand() % 200 - 100);
	stayPos.y = (float)(rand() % 120 - 60);

	//放出的を生成
	enemys.push_back(Releaser::Create(portaModel, startPos, stayPos));
}

void GameScene::SpawnChaser()
{
	//生成時に初期座標を決める
	XMFLOAT3 startPos = {};

	XMFLOAT2 startLine = wall->GetWallLine();
	startLine.x += 5;
	startLine.y += 5;

	//4パターンのランダムで初期座標と移動方向をセット
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, -startLine.y, 0 }; }
	else if (posAngleRand == 1) { startPos = { startLine.x, 0, 0 }; }
	else if (posAngleRand == 2) { startPos = { 0, startLine.y, 0 }; }
	else if (posAngleRand == 3) { startPos = { -startLine.x, 0, 0 }; }

	//追従敵を生成
	enemys.push_back(Chaser::Create(charoModel, startPos));
}

void GameScene::SpawnEnemyToEnemy(BaseEnemy* enemy)
{
	//敵が分裂的だった場合
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Division)
	{
		//生成時に初期座標と移動方向を決める
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = enemy->GetMoveDegree();
		int parantKnockBackPowerLevel = enemy->GetKnockBackPowerLevel();

		//3体生成する
		for (int i = 0; i < 3; i++)
		{
			angle += 120;

			//親のノックバックの強さを引き継いだ直進敵を生成する
			enemys.push_back(Straighter::Create(portaModel, startPos, angle, parantKnockBackPowerLevel));
		}
	}

	//敵が放出的だった場合
	else if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Releaser)
	{
		//生成時に初期座標と移動方向を決める
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = (float)(rand() % 360);
		int parantKnockBackPowerLevel = enemy->GetKnockBackPowerLevel();

		//親のノックバックの強さを引き継いだ直進敵を生成する
		enemys.push_back(Straighter::Create(portaModel, startPos, angle, parantKnockBackPowerLevel));
	}
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
		camera->StartCameraShake(3);
		ShakeTime++;

		//シェイク開始から一定時間経ったら
		if (ShakeTime > 20) {
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