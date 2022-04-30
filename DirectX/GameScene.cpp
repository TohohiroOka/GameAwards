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

	//ガル族解放
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		safe_delete(*itrGaruEnemy);
	}
	//ガル族のリスト解放
	garuEnemys.clear();

	//敵の弾解放
	for (int i = 0; i < enemyBulletNum; i++)
	{
		safe_delete(enemyBullet[i]);
	}

	//画面枠解放
	safe_delete(frame);

	//エフェクトの解放
	safe_delete(effects);

	//背景の解放
	buckGround->AllDelete();
	safe_delete(buckGround);

	//コンボ解放
	safe_delete(combo);

	//制限時間解放
	safe_delete(timeLimit);
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
	initialCircleSquareModel = Model::CreateFromOBJ("initialcirclesquare");//固定敵の外枠のモデル
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
	Sprite::LoadTexture(4, L"Resources/result.png");
	Sprite::LoadTexture(5, L"Resources/pressButton.png");
	Sprite::LoadTexture(6, L"Resources/title.png");
	//デバッグテキスト生成
	DebugText::GetInstance()->Initialize(0);


	//プレイヤー生成
	player = Player::Create(pBodyModel);
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
	//敵の弾生成
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//画面枠生成
	frame = Frame::Create(frameModel);
	XMFLOAT2 frameline = frame->GetFrameLine();
	Player::SetMoveRange(frameline);
	PlayerBullet::SetDeadPos(frameline);

	//エフェクト初期化
	effects = new StageEffect();
	effects->Initialize();

	//背景の初期化
	buckGround->Create(hexagonModel);

	//コンボ生成
	combo = Combo::Create(2, 3);

	//制限時間生成
	timeLimit = TimeLimit::Create(2);

	//サウンド用
	audio = new Audio();
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//画面枠更新
	frame->Update();

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

	//ガル族生成
	if (input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_RT))
	{
		//ガル族をスポーン
		SpawnGaruEnemy();
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

		//枠に当たったか判定を取る
		if ((*itrGaruEnemy)->IsCollisionFrame(frame->GetFrameLine()))
		{
			//死亡
			(*itrGaruEnemy)->Dead();

			//枠にもダメージを与える
			int damagePower = 10;
			frame->Damage(damagePower);

			//コンボを増やす
			combo->AddCombo();
		}

		//衝突用に座標と半径の大きさを借りる
		XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
		float enemySize = (*itrGaruEnemy)->GetScale().x;

		//プレイヤーとガル族の当たり判定
		//プレイヤーがダメージ状態でないなら
		if (!player->GetIsDamege())
		{
			//衝突用に座標と半径の大きさを借りる
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

				//敵も死亡
				(*itrGaruEnemy)->SetDelete();

				//コンボ終了
				combo->LostCombo();

				isShake = true;
			}
		}

		//衝撃波とガル族の当たり判定
		for (int i = 0; i < shockWaveNum; i++)
		{
			//衝撃波が発射状態
			if (shockWave[i]->GetIsAlive())
			{
				//当たり判定用変数
				XMFLOAT3 wavePos = shockWave[i]->GetPosition();
				float waveSize = shockWave[i]->GetRadius();

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					wavePos, waveSize, enemyPos, enemySize);

				//衝撃波とガル族が衝突状態
				if (isCollision)
				{
					//既に衝突したことがあるか確認(衝突中ダメージを食らい続けてしまうため)
					if (!shockWave[i]->IsKnowGaruEnemy((*itrGaruEnemy)))
					{
						//ガル族をノックバックで飛ばす
						float angle = atan2f(enemyPos.y - wavePos.y, enemyPos.x - wavePos.x);
						int powerLevel = shockWave[i]->GetPowerLevel();
						(*itrGaruEnemy)->SetKnockBack(angle, powerLevel);

						//コンボを増やす
						combo->AddCombo();
					}
				}
			}
		}
	}

	//敵の弾更新
	for (int i = 0; i < enemyBulletNum; i++)
	{
		//弾が生きていなければ飛ばす
		if (!enemyBullet[i]->GetIsAlive()) { continue; }

		//更新処理
		enemyBullet[i]->Update();

		//衝突用に座標と半径の大きさを借りる
		XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
		float bulletSize = enemyBullet[i]->GetScale().x;
		//プレイヤーと敵の弾の当たり判定
		//プレイヤーがダメージ状態でないなら
		if (!player->GetIsDamege())
		{
			//プレイヤーと敵の弾の当たり判定
			//衝突用に座標と半径の大きさを借りる
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
			}
		}

		//衝撃波と敵の弾の当たり判定
		for (int j = 0; j < shockWaveNum; j++)
		{
			//衝撃波が発射状態
			if (shockWave[j]->GetIsAlive())
			{
				//当たり判定用変数
				XMFLOAT3 wavePos = shockWave[j]->GetPosition();
				float waveSize = shockWave[j]->GetRadius();

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					wavePos, waveSize, bulletPos, bulletSize);

				//衝撃波と敵の弾が衝突状態
				if (isCollision)
				{
					//敵の弾を消す
					enemyBullet[i]->Dead();
				}
			}
		}
	}

	//ガル族削除
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrGaruEnemy)->GetIsDelete())
		{
			//要素を削除、リストから除外する
			safe_delete(*itrGaruEnemy);
			itrGaruEnemy = garuEnemys.erase(itrGaruEnemy);
			continue;
		}
		//for分を回す
		itrGaruEnemy++;
	}


	if (frame->GetHP() == 30) { DebugText::GetInstance()->Print("WALL HP:30", 100, 500); }
	else if (frame->GetHP() == 20) { DebugText::GetInstance()->Print("WALL HP:20", 100, 500); }
	else if (frame->GetHP() == 10) { DebugText::GetInstance()->Print("WALL HP:10", 100, 500); }
	else if (frame->GetHP() <= 0) { DebugText::GetInstance()->Print("WALL BREAK", 100, 500); }

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 100, 300);
	DebugText::GetInstance()->Print("RB:ShockWave", 100, 350);
	DebugText::GetInstance()->Print("RT:SpawnEnemy", 100, 400);

	//エフェクトの更新
	effects->Update(camera);

	//背景更新
	buckGround->Update();

	//コンボ更新
	if (Xinput->TriggerButton(XInputManager::PAD_START)) {
		combo->AddCombo();
	}
	combo->Update();

	//制限時間更新
	timeLimit->Update();

	//カメラ更新
	CameraUpdate(camera);

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

		//敵の弾描画
		for (int i = 0; i < enemyBulletNum; i++)
		{
			enemyBullet[i]->Draw();
		}
		//ガル族描画
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			(*itrGaruEnemy)->Draw();
		}

		//背景
		buckGround->Draw();

		Object3d::PostDraw();


		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//コンボ描画
		combo->Draw();

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

	//画面枠初期化
	frame->Reset();
	//枠のライン変更に合わせてオブジェクトの様々な境界線も変更
	XMFLOAT2 frameLine = frame->GetFrameLine();
	Player::SetMoveRange(frameLine);
	EnemyBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
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

void GameScene::SpawnGaruEnemy()
{
	//生成時にスポーン座標と移動後の座標を決める
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	//ランダム生成
	spawnPos.x = (float)(rand() % 300 - 150);
	spawnPos.y = 100;
	stayPos.x = (float)(rand() % 300 - 150);
	stayPos.y = (float)(rand() % 180 - 90);

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