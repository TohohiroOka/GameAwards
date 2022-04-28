#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

#include "Garuta.h"
#include "Garutata.h"
#include "Tuff.h"

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

	//固定敵解放
	for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
	{
		safe_delete(*itrFixedEnemy);
	}
	//固定敵のリスト解放
	fixedEnemys.clear();


	//画面枠解放
	safe_delete(frame);

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

	//衝撃波生成
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i] = ShockWave::Create(waveModel);
	}

	//ボス生成
	bossEnemy[0] = Tuff::Create(tuffModel);

	//敵の弾生成
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//画面枠生成
	frame = Frame::Create(frameModel);
	XMFLOAT2 frameline = frame->GetFrameLine();
	Player::SetMoveRange(frameline);


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

	//画面枠更新
	frame->Update();

	//プレイヤー更新
	player->Update();

	//衝撃波発射
	if (player->GetIsShockWaveStart())
	{
		ShockWaveStart(10);
	}
	//衝撃波更新
	for (int i = 0; i < shockWaveNum; i++)
	{
		//更新処理
		shockWave[i]->Update();

		//衝撃波が発射状態でなければ飛ばす
		if (!shockWave[i]->GetIsAlive()) { continue; }

		//当たり判定用変数
		XMFLOAT3 wavePos = shockWave[i]->GetPosition();
		float waveSize = shockWave[i]->GetRadius();

		//衝撃波とガル族の当たり判定
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//ガル族が生きていなければ飛ばす ガル族がスポーン中だったら飛ばす ガル族が逃走中だったら飛ばす
			if (!(*itrGaruEnemy)->GetIsAlive() || (*itrGaruEnemy)->GetIsDuringSpawn() || (*itrGaruEnemy)->GetIsEscape()) { continue; }

			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x - 0.2f;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, enemyPos, enemySize);

			//衝撃波とガル族が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//既に衝突したことがあるか確認(衝突中ダメージを食らい続けてしまうため)
			if (shockWave[i]->IsKnowGaruEnemy((*itrGaruEnemy))) { continue; }

			//ガル族をノックバックで飛ばす
			int power = shockWave[i]->GetPower();
			(*itrGaruEnemy)->SetKnockBack(wavePos, power);
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
			if (shockWave[i]->IsKnowCharo((*itrCharo))) { continue; }

			//チャロにダメージを喰らわせる
			int damagePower = shockWave[i]->GetPower();
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
			if (shockWave[i]->IsKnowPorta((*itrPorta))) { continue; }

			//ポルタにダメージを喰らわせる
			int damagePower = shockWave[i]->GetPower();
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

		//枠に当たったか判定を取る
		if ((*itrGaruEnemy)->IsCollisionFrame(frame->GetFrameLine()))
		{
			//死亡
			(*itrGaruEnemy)->Dead();

			//枠にもダメージを与える
			frame->Damage(10);
		}

		//プレイヤーとガル族の当たり判定
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
			if (player->GetIsTackle())
			{
				player->SetKnockback();

				//ガル族をノックバックで飛ばす
				XMFLOAT3 playerPos = player->GetPosition();
				int power = 10;
				(*itrGaruEnemy)->SetKnockBack(playerPos, power);
			}
			else
			{
				//プレイヤーがダメージ状態でないなら
				if (!player->GetIsDamege())
				{
					//プレイヤーはダメージを喰らう
					player->Damage();
					player->SetKnockback();

					isShake = true;
				}
			}
		}
	}

	//チャロポルタ生成
	//if (input->TriggerKey(DIK_LSHIFT) || Xinput->TriggerButton(XInputManager::PAD_LT))
	/*{
		SpawnCharoPorta(0, wave);
	}*/

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

				//画面をシェイクさせる
				isShake = true;
			}
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

				//画面をシェイクさせる
				isShake = true;
			}
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

					//画面をシェイクさせる
					isShake = true;
				}
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
			}
		}
	}

	//固定敵更新
	for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
	{
		(*itrFixedEnemy)->Update();
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

	//チャロ削除
	for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrCharo)->GetIsDelete())
		{
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
			//要素を削除、リストから除外する
			safe_delete(*itrPorta);
			itrPorta = portaEnemys.erase(itrPorta);
			continue;
		}
		//for分を回す
		itrPorta++;
	}


	if (frame->GetHP() == 30) { DebugText::GetInstance()->Print("WALL HP:30", 100, 500); }
	else if (frame->GetHP() == 20) { DebugText::GetInstance()->Print("WALL HP:20", 100, 500); }
	else if (frame->GetHP() == 10) { DebugText::GetInstance()->Print("WALL HP:10", 100, 500); }
	else if (frame->GetHP() <= 0) { DebugText::GetInstance()->Print("WALL BREAK", 100, 500); }

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 1000, 100);
	DebugText::GetInstance()->Print("LB:Tackle", 1000, 150);
	DebugText::GetInstance()->Print("RB:ShockWave", 1000, 200);
	DebugText::GetInstance()->Print("RT:SpawnEnemy", 1000, 250);

	//エフェクトの更新
	effects->Update(camera);

	//背景更新
	buckGround->Update();

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

		//背景
		buckGround->Draw();

		Object3d::PostDraw();


		//スプライト前面描画
		Sprite::PreDraw(cmdList);

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
	Player::SetMoveRange(frameLine);
	EnemyBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
	Porta::SetReflectionLine({ frameLine.x - 3, frameLine.y - 2 });
	BossEnemy::SetFrameLine({ frameLine.x - 3, frameLine.y - 2 });


	//スポーンパターン
	spawnTimer = 0;//スポーンタイマー初期化
}

void GameScene::ShockWaveStart(const int shockWavePower)
{
	//プレイヤーの座標から発射する
	XMFLOAT3 pos = player->GetPosition();

	//発射する衝撃波を設定
	for (int i = 0; i < shockWaveNum; i++)
	{
		//発射されていたら飛ばす
		if (shockWave[i]->GetIsAlive()) { continue; }

		//衝撃波発射
		shockWave[i]->WaveStart(pos, shockWavePower);

		//画面シェイク
		isShake = true;

		//1つ発射したらループを抜ける(一気に全ての衝撃波を発射してしまうため)
		break;
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