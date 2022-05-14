#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "GameCollision.h"

#include "Straighter.h"
#include "Division.h"
#include "Releaser.h"
#include "Chaser.h"
#include "TitleLogo.h"

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
	safe_delete(titleLogoModel);
	safe_delete(circleModel);
	safe_delete(playerModel);

	safe_delete(straighterModel1);
	safe_delete(straighterModel2);
	safe_delete(straighterModel3);
	safe_delete(straighterModel4);

	safe_delete(divisionModel1);
	safe_delete(divisionModel2);
	safe_delete(divisionModel3);
	safe_delete(divisionModel4);

	safe_delete(releaserModel1);
	safe_delete(releaserModel2);
	safe_delete(releaserModel3);
	safe_delete(releaserModel4);

	safe_delete(chaserModel1);
	safe_delete(chaserModel2);
	safe_delete(chaserModel3);
	safe_delete(chaserModel4);

	safe_delete(eBullModel);
	safe_delete(hexagonModel);
	safe_delete(portaModel);
	safe_delete(charoModel);
	safe_delete(frameModel);
	safe_delete(waveModel);

	//プレイヤー解放
	safe_delete(player);
	//衝撃波解放
	for (int i = 0; i < shockWaveNum; i++)
	{
		safe_delete(shockWave[i]);
	}

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
	//buckGround->AllDelete();
	//safe_delete(buckGround);
	safe_delete(backGround);

	//シーン遷移用暗転解放
	safe_delete(blackout);

	//タイトルシーンUI解放
	safe_delete(titleUI);

	//UIを囲う枠解放
	safe_delete(UIFrame);


	//制限時間解放
	safe_delete(timeLimitGauge);
	//巨大衝撃波用ゲージ解放
	safe_delete(shockWaveGauge);
	//ReadyGo解放
	safe_delete(readyGo);
	//ポーズシーンUI解放
	safe_delete(pauseUI);
	//Finish解放
	safe_delete(finish);
	//リザルトシーンUI解放
	safe_delete(resultUI);
}

void GameScene::Initialize(Camera* camera)
{
	// nullptrチェック
	assert(camera);

	//ライト
	light = LightGroup::Create();
	light->DefaultLightSetting();
	Object3d::SetLightGroup(light);

	titleLogoModel = Model::CreateFromOBJ("titleLogo");//タイトルロゴのモデル
	circleModel = Model::CreateFromOBJ("circle");//タバコのモデル
	playerModel = Model::CreateFromOBJ("player");//プレイヤーのモデル

	straighterModel1 = Model::CreateFromOBJ("enemy1_1");//直進敵のモデル1
	straighterModel2 = Model::CreateFromOBJ("enemy1_2");//直進敵のモデル2
	straighterModel3 = Model::CreateFromOBJ("enemy1_3");//直進敵のモデル3
	straighterModel4 = Model::CreateFromOBJ("enemy1_4");//直進敵のモデル4

	divisionModel1 = Model::CreateFromOBJ("enemy2_1");//分裂敵のモデル1
	divisionModel2 = Model::CreateFromOBJ("enemy2_2");//分裂敵のモデル2
	divisionModel3 = Model::CreateFromOBJ("enemy2_3");//分裂敵のモデル3
	divisionModel4 = Model::CreateFromOBJ("enemy2_4");//分裂敵のモデル4

	releaserModel1 = Model::CreateFromOBJ("enemy3_1");//放出敵のモデル1
	releaserModel2 = Model::CreateFromOBJ("enemy3_2");//放出敵のモデル2
	releaserModel3 = Model::CreateFromOBJ("enemy3_3");//放出敵のモデル3
	releaserModel4 = Model::CreateFromOBJ("enemy3_4");//放出敵のモデル4

	chaserModel1 = Model::CreateFromOBJ("enemy4_1");//追跡敵のモデル1
	chaserModel2 = Model::CreateFromOBJ("enemy4_2");//追跡敵のモデル2
	chaserModel3 = Model::CreateFromOBJ("enemy4_3");//追跡敵のモデル3
	chaserModel4 = Model::CreateFromOBJ("enemy4_4");//追跡敵のモデル4

	eBullModel = Model::CreateFromOBJ("enemybullet");//敵の弾のモデル
	hexagonModel = Model::CreateFromOBJ("hexagon");//六角形のモデル
	portaModel = Model::CreateFromOBJ("porta");//ポルタのモデル
	charoModel = Model::CreateFromOBJ("charo");//チャロのモデル
	frameModel = Model::CreateFromOBJ("frame");//フレームのモデル
	waveModel = Model::CreateFromOBJ("wave");//衝撃波のモデル


	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/white1x1.png");
	Sprite::LoadTexture(2, L"Resources/num.png");
	Sprite::LoadTexture(3, L"Resources/combo.png");
	Sprite::LoadTexture(4, L"Resources/break.png");
	Sprite::LoadTexture(5, L"Resources/gauge.png");
	Sprite::LoadTexture(6, L"Resources/gaugeframe.png");
	Sprite::LoadTexture(7, L"Resources/ready.png");
	Sprite::LoadTexture(8, L"Resources/go.png");
	Sprite::LoadTexture(9, L"Resources/finish.png");
	Sprite::LoadTexture(10, L"Resources/result.png");
	Sprite::LoadTexture(11, L"Resources/retry.png");
	Sprite::LoadTexture(12, L"Resources/pressA.png");
	Sprite::LoadTexture(13, L"Resources/time.png");
	Sprite::LoadTexture(14, L"Resources/timeframe.png");
	Sprite::LoadTexture(15, L"Resources/timegauge.png");
	Sprite::LoadTexture(16, L"Resources/background.png");
	Sprite::LoadTexture(17, L"Resources/blackframe.png");
	Sprite::LoadTexture(18, L"Resources/continueMenu.png");

	//デバッグテキスト生成
	DebugText::GetInstance()->Initialize(0);


	//プレイヤー生成
	player = Player::Create(playerModel);
	//衝撃波生成
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i] = ShockWave::Create(waveModel);
	}


	//敵のモデルをセット
	Straighter::SetModel(straighterModel1);
	Division::SetModel(divisionModel1);
	Releaser::SetModel(releaserModel1);
	Chaser::SetModel(chaserModel1);
	/*Straighter::SetModel(portaModel, portaModel, portaModel, portaModel);
	Division::SetModel(portaModel, portaModel, portaModel, portaModel);
	Releaser::SetModel(portaModel, portaModel, portaModel, portaModel);
	Chaser::SetModel(charoModel, charoModel, charoModel, charoModel);*/
	TitleLogo::SetModel(titleLogoModel);

	//壁生成
	wall = WallManager::Create();

	//移動範囲設定
	Player::SetMoveRange(minWallLinePosition, maxWallLinePosition);
	XMFLOAT2 enemyWallLineMin = minWallLinePosition;
	enemyWallLineMin.y -= 2;
	XMFLOAT2 enemyWallLineMax = maxWallLinePosition;
	enemyWallLineMax.y += 2;
	BaseEnemy::SetWallLine(enemyWallLineMin, enemyWallLineMax);

	//エフェクト初期化
	effects = new StageEffect();
	effects->Initialize();

	//背景生成
	backGround = BackGround::Create(16);

	//シーン遷移用暗転生成
	blackout = Blackout::Create(1);

	//タイトルシーンUI生成
	titleUI = TitleUI::Create(portaModel);

	//UIを囲う枠生成
	UIFrame = UIFrame::Create(17);


	//制限時間生成
	timeLimitGauge = TimeLimitGauge::Create(13, 14, 15);
	//巨大衝撃波用ゲージ生成
	shockWaveGauge = BigShockWaveGauge::Create(6, 5);

	//ReadyGo生成
	readyGo = ReadyGo::Create(7, 8);
	//ポーズシーンUI生成
	pauseUI = PauseUI::Create(1, 4, 18);
	//Finish生成
	finish = Finish::Create(9);
	//リザルトシーンUI生成
	resultUI = ResultUI::Create(1, 10, 4, 2, 11, 12);

	//サウンド用
	audio = new Audio();
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//タイトル準備
	if (scene == SceneName::TitleStayScene)
	{
		//暗転解除中
		if (blackout->GetIsBlackReturn())
		{
			//シーン遷移用暗転更新
			blackout->Update();
		}
		else
		{
			//演出が終了したら次に行く
			if (wall->GetIsAlive())
			{
				scene = SceneName::TitleScene;

				//プレイヤーをスポーン状態にセット
				player->SetSpawn();
			}

			//壁更新
			wall->Update();
		}
	}

	//タイトルシーン
	else if (scene == SceneName::TitleScene)
	{
		//プレイヤー更新
		player->Update();

		//スポーンが終了したら
		if (player->GetTriggerSpawnEnd())
		{
			//タイトルロゴ生成
			enemys.push_back(TitleLogo::Create());
		}

		//ポイ捨て開始
		if (player->GetIsShockWaveStart())
		{
			//プレイヤー衝撃波発射
			PlayerShockWaveStart(player->GetPosition());
		}

		//衝撃波更新
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Update();
		}

		//敵更新
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//更新処理
			(*itrEnemy)->Update();

			//削除状態になったら次のシーンへ
			if ((*itrEnemy)->GetIsDelete())
			{
				scene = SceneName::ReadyGoScene;

				//プレイヤーをゲーム開始時の座標に移動状態にセット
				player->SetGameStartPos();
			}

			//敵が生きていなければ飛ばす
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//壁と敵の当たり判定を取る
			GameCollision::CheckWallToEnemy(wall, (*itrEnemy));

			//プレイヤーと敵の当たり判定
			GameCollision::CheckPlayerToEnemy(player, (*itrEnemy));
			//衝撃波と敵の当たり判定
			for (int i = 0; i < shockWaveNum; i++)
			{
				GameCollision::CheckShockWaveToEnemy(shockWave[i], (*itrEnemy));
			}
		}
		//タイトルロゴのスポーンが完了したら
		if (TitleLogo::GetTriggerSpawnEnd())
		{
			//プレイヤー操作可能
			player->SetIsFreeMove(true);
			//UIを描画する
			titleUI->SetIsDraw(true);
		}

		//壁更新
		wall->Update();
		//タイトルシーン用UI更新
		titleUI->Update(player->GetPosition());
	}

	//ReadyGoシーン
	else if (scene == SceneName::ReadyGoScene)
	{
		//プレイヤー更新
		player->Update();
		//衝撃波更新
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Update();
		}

		//プレイヤーがゲーム開始時の座標に到達したら
		if (player->GetTriggerMoveStartPosEnd())
		{
			//UIを画面上部に移動させる
			timeLimitGauge->SetMoveGamePos();
			shockWaveGauge->SetMoveGamePos();
		}
		//UI更新
		timeLimitGauge->Update();
		shockWaveGauge->Update();

		//壁の更新
		wall->Update();

		//壁が生きていたら
		if (wall->GetIsAlive())
		{
			//タイトルシーンで破壊した壁のみ何もせず修復状態にする
			wall->GetTriggerBreak();

			//ReadyGoスプライト更新
			readyGo->Update();
			//ReadyGoのGoまで表示が終わったら次のシーンへ
			if (readyGo->GetIsReadyGoEnd())
			{
				scene = SceneName::GamePlayScene;

				//プレイヤーの行動制限を解く
				player->SetIsFreeMove(true);
				//制限時間のカウントダウンを開始する
				timeLimitGauge->SetIsCountDown(true);
			}
		}
	}

	//ゲームシーン
	else if (scene == SceneName::GamePlayScene)
	{
		//プレイヤー更新
		player->Update();
		//プレイヤー衝撃波発射
		if (player->GetIsShockWaveStart())
		{
			//プレイヤー衝撃波発射
			PlayerShockWaveStart(player->GetPosition());
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

		//敵生成
		SpawnEnemyManager(breakScore);

		//敵更新
		BaseEnemy::SetTargetPos(player->GetPosition());
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
				//壁と敵の当たり判定を取る
				if (GameCollision::CheckWallToEnemy(wall, (*itrEnemy)))
				{
					//エフェクトを出す
					wall->SetHitEffect((*itrEnemy)->GetPosition());
				}
			}
			//壁がない場合
			else
			{
				//画面外に出たら削除する
				XMFLOAT3 pos = (*itrEnemy)->GetPosition();
				if (pos.x <= minWallLinePosition.x || pos.x >= maxWallLinePosition.x ||
					pos.y <= minWallLinePosition.y || pos.y >= maxWallLinePosition.y)
				{
					(*itrEnemy)->SetDelete();
				}
			}

			//プレイヤーと敵の当たり判定
			if (GameCollision::CheckPlayerToEnemy(player, (*itrEnemy)))
			{
				//画面シェイク
				isShake = true;
			}

			//衝撃波と敵の当たり判定
			for (int i = 0; i < shockWaveNum; i++)
			{
				if (GameCollision::CheckShockWaveToEnemy(shockWave[i], (*itrEnemy)))
				{
					shockWaveGauge->AddPoint();
				}
			}
		}

		//壁更新
		wall->Update();
		if (wall->GetTriggerBreak())
		{
			//壊したスコア加算
			breakScore++;

			//タイムリミットが伸びる
			timeLimitGauge->Recovery(10);
		}

		/*{
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

			DebugText::GetInstance()->Print("LSTICK:PlayerMove", 100, 250);
			DebugText::GetInstance()->Print("RB:ShockWave", 100, 300);
			DebugText::GetInstance()->Print("RT:SpawnStraighter", 100, 350);
			DebugText::GetInstance()->Print("LT:SpawnDivision", 100, 400);
			DebugText::GetInstance()->Print("LB:SpawnReleaser", 100, 450);
			DebugText::GetInstance()->Print("RIGHT:SpawnChaser", 100, 500);
		}*/



		//制限時間更新
		timeLimitGauge->Update();
		//制限時間が0になったら次のシーンへ
		if (timeLimitGauge->GetIsCountDownEnd())
		{
			scene = SceneName::FinishScene;

			//プレイヤーを自由に操作できなくする
			player->SetIsFreeMove(false);

			//リザルトシーン用に壁破壊数を渡しておく
			resultUI->SetBreakWallNum(breakScore);
		}

		//巨大衝撃波用ゲージ更新
		shockWaveGauge->Update();

		//指定したボタンを押すとポーズシーンへ
		if (input->TriggerKey(DIK_1) || Xinput->TriggerButton(XInputManager::PAD_START))
		{
			scene = SceneName::PauseScene;

			//ポーズシーンをリセット
			pauseUI->Reset();
		}
	}

	//ポーズシーン
	else if (scene == SceneName::PauseScene)
	{
		//ポーズシーンUI更新
		pauseUI->Update();

		//確定していないとき、指定したボタンを押すとシーン切り替え選択を確定させる
		if (!pauseUI->GetIsSelect() && (input->TriggerKey(DIK_1) || Xinput->TriggerButton(XInputManager::PAD_A)))
		{
			//確定させる
			pauseUI->SetSelect();

			//ゲームに戻る
			if (pauseUI->GetIsBackGame())
			{
				scene = SceneName::GamePlayScene;
			}
			//タイトルシーンに戻る
			else
			{
				//暗転開始
				blackout->SetBlackout();
			}
		}

		//シーン遷移用暗転更新
		blackout->Update();
		//暗転で画面が真っ暗になったらタイトルシーンに戻る
		if (blackout->GetIsAllBlack())
		{
			//タイトルシーン移行用初期化
			ResetTitleScene();

			//暗転を解除状態にセット
			blackout->SetBlackoutReturn();
		}
	}

	//Finishシーン
	else if (scene == SceneName::FinishScene)
	{
		//プレイヤー更新
		player->Update();
		//衝撃波更新
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Update();
		}
		//敵更新
		BaseEnemy::SetTargetPos(player->GetPosition());
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
				//壁と敵の当たり判定を取る
				GameCollision::CheckWallToEnemy(wall, (*itrEnemy));
			}
			//壁がない場合
			else
			{
				//画面外に出たら削除する
				XMFLOAT3 pos = (*itrEnemy)->GetPosition();

				//範囲外判定
				if (pos.x <= outsideMinPosition.x || pos.x >= outsideMaxPosition.x ||
					pos.y <= outsideMinPosition.y || pos.y >= outsideMaxPosition.y)
				{
					(*itrEnemy)->SetDelete();
				}
			}
		}

		//壁更新
		wall->Update();
		//UI更新
		timeLimitGauge->Update();
		shockWaveGauge->Update();

		//Finish更新
		finish->Update();
		//Finishを表示し終えたら次のシーンへ
		if (finish->GetIsFinishSpriteMoveEnd())
		{
			scene = SceneName::ResultScene;

			//敵をリザルトシーン用の動きに変える
			BaseEnemy::SetIsResultMove(true);

			//壁をリザルトシーン用に画面外に動かす
			wall->SetChangeResult();

			//UIを画面外に移動させる
			timeLimitGauge->SetMoveResultPos();
			shockWaveGauge->SetMoveResultPos();
		}
	}

	//リザルトシーン
	else if (scene == SceneName::ResultScene)
	{
		//プレイヤー更新
		player->Update();
		//敵更新
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//更新処理
			(*itrEnemy)->Update();

			//敵が生きていなければ飛ばす
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//壁がないので、画面外に出たら削除する
			XMFLOAT3 pos = (*itrEnemy)->GetPosition();

			//範囲外判定
			if (pos.x <= outsideMinPosition.x || pos.x >= outsideMaxPosition.x ||
				pos.y <= outsideMinPosition.y || pos.y >= outsideMaxPosition.y)
			{
				(*itrEnemy)->SetDelete();
			}
		}

		//壁更新
		wall->Update();
		//UI更新
		timeLimitGauge->Update();
		shockWaveGauge->Update();

		//リザルトシーンUI更新
		resultUI->Update();
		//全ての描画が終わったら
		if (resultUI->GetIsDrawAll())
		{
			//確定していないとき、指定したボタンを押すとシーン切り替え選択を確定させる
			if (!resultUI->GetIsSelect() && (input->TriggerKey(DIK_Z) || Xinput->TriggerButton(XInputManager::PAD_A)))
			{
				//確定させる
				resultUI->SetSelect();

				//リトライ
				if (resultUI->GetIsRetry())
				{
					ResetGame();
				}
				//タイトルシーンに戻る
				else
				{
					//暗転開始
					blackout->SetBlackout();
				}
			}
		}
		//シーン遷移用暗転更新
		blackout->Update();
		//暗転で画面が真っ暗になったらタイトルシーンに戻る
		if (blackout->GetIsAllBlack())
		{
			//タイトルシーン移行用初期化
			ResetTitleScene();

			//暗転を解除状態にセット
			blackout->SetBlackoutReturn();
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

	//エフェクトの更新
	effects->Update(camera);
	//背景更新
	//buckGround->Update();
	backGround->Update();
	//UIを囲う枠更新
	UIFrame->Update();
	//カメラ更新
	CameraUpdate(camera);

	input = nullptr;
}

void GameScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//タイトルシーン
	if (scene == SceneName::TitleStayScene || scene == SceneName::TitleScene)
	{
		//スプライト背面描画
		Sprite::PreDraw(cmdList);

		//背景描画
		backGround->Draw();

		Sprite::PostDraw();

		//オブジェクト描画
		Object3d::PreDraw(cmdList);

		//タイトルシーン用UI描画
		titleUI->Draw();

		//壁描画
		wall->Draw();

		//プレイヤー描画
		player->Draw();

		//衝撃波描画
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Draw();
		}

		//敵描画
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();

		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//UIを囲う枠描画
		UIFrame->Draw();

		//シーン遷移用暗転描画
		blackout->Draw();

		Sprite::PostDraw();
	}
	//ReadyGoシーン
	else if (scene == SceneName::ReadyGoScene)
	{
		//スプライト背面描画
		Sprite::PreDraw(cmdList);

		//背景描画
		backGround->Draw();

		Sprite::PostDraw();

		//オブジェクト描画
		Object3d::PreDraw(cmdList);

		//壁描画
		wall->Draw();

		//プレイヤー描画
		player->Draw();

		Object3d::PostDraw();

		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//UIを囲う枠描画
		UIFrame->Draw();

		//ReadyGo描画
		readyGo->Draw();
		//制限時間描画
		timeLimitGauge->Draw();
		//巨大衝撃波用ゲージ描画
		shockWaveGauge->Draw();

		//デバッグテキスト描画
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//ゲームシーン
	else if (scene == SceneName::GamePlayScene)
	{
		//スプライト背面描画
		Sprite::PreDraw(cmdList);

		//背景描画
		backGround->Draw();

		Sprite::PostDraw();

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

		//敵描画
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();


		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//UIを囲う枠描画
		UIFrame->Draw();

		//制限時間描画
		timeLimitGauge->Draw();
		//巨大衝撃波用ゲージ描画
		shockWaveGauge->Draw();

		//デバッグテキスト描画
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//ポーズシーン
	else if (scene == SceneName::PauseScene)
	{
		//スプライト背面描画
		Sprite::PreDraw(cmdList);

		//背景描画
		backGround->Draw();

		Sprite::PostDraw();

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

		//敵描画
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();


		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//UIを囲う枠描画
		UIFrame->Draw();

		//制限時間描画
		timeLimitGauge->Draw();
		//巨大衝撃波用ゲージ描画
		shockWaveGauge->Draw();

		//ポーズシーンUI描画
		pauseUI->Draw();

		//シーン遷移用暗転描画
		blackout->Draw();

		//デバッグテキスト描画
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//Finishシーン
	else if (scene == SceneName::FinishScene)
	{
		//スプライト背面描画
		Sprite::PreDraw(cmdList);

		//背景描画
		backGround->Draw();

		Sprite::PostDraw();

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

		//敵描画
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();

		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//UIを囲う枠描画
		UIFrame->Draw();

		//制限時間描画
		timeLimitGauge->Draw();
		//巨大衝撃波用ゲージ描画
		shockWaveGauge->Draw();
		//Finish描画
		finish->Draw();

		//デバッグテキスト描画
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//リザルトシーン
	else if (scene == SceneName::ResultScene)
	{
		//スプライト背面描画
		Sprite::PreDraw(cmdList);

		//背景描画
		backGround->Draw();

		Sprite::PostDraw();

		//オブジェクト描画
		Object3d::PreDraw(cmdList);

		//壁描画
		wall->Draw();

		//プレイヤー描画
		player->Draw();

		//敵描画
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();

		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//UIを囲う枠描画
		UIFrame->Draw();
		//制限時間描画
		timeLimitGauge->Draw();
		//巨大衝撃波用ゲージ描画
		shockWaveGauge->Draw();

		//リザルトシーンUI描画
		resultUI->Draw();

		//シーン遷移用暗転描画
		blackout->Draw();

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

void GameScene::ResetTitleScene()
{
	//プレイヤー初期化
	player->ResetTitle();
	//衝撃波初期化
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i]->Reset();
	}
	//敵が残っていたら削除
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//削除フラグをtrueにして強制削除
		(*itrEnemy)->SetDelete();
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
	//敵を元の動きに戻す
	BaseEnemy::SetIsResultMove(false);

	//壁初期化
	wall->Reset();

	//UIを描画しない
	titleUI->SetIsDraw(false);
	//制限時間初期化
	timeLimitGauge->Reset();
	//壊したスコア初期化
	breakScore = 0;
	//巨大衝撃波用ゲージ初期化
	shockWaveGauge->Reset();

	//ReadyGo初期化
	readyGo->Reset();
	//Finish初期化
	finish->Reset();
	//リザルトシーンUI初期化
	resultUI->Reset();

	//シーンをタイトルシーンに移行
	scene = SceneName::TitleStayScene;

	//画面シェイクしない
	isShake = false;
	//画面シェイク時間初期化
	ShakeTime = 0;

	//スポーンするまでのカウント初期化
	spawnTimer = 0;
}

void GameScene::ResetGame()
{
	//プレイヤー初期化
	player->ResetGame();
	//衝撃波初期化
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i]->Reset();
	}
	//敵が残っていたら削除
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//削除フラグをtrueにして強制削除
		(*itrEnemy)->SetDelete();
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
	//敵を元の動きに戻す
	BaseEnemy::SetIsResultMove(false);

	//壁初期化
	wall->Reset();


	//制限時間初期化
	timeLimitGauge->Reset();
	//壊したスコア初期化
	breakScore = 0;
	//巨大衝撃波用ゲージ初期化
	shockWaveGauge->Reset();

	//ReadyGo初期化
	readyGo->Reset();
	//Finish初期化
	finish->Reset();
	//リザルトシーンUI初期化
	resultUI->Reset();

	//シーンをReadyGoシーンに移行
	scene = SceneName::ReadyGoScene;

	//画面シェイクしない
	isShake = false;
	//画面シェイク時間初期化
	ShakeTime = 0;

	//スポーンするまでのカウント初期化
	spawnTimer = 0;
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

void GameScene::BigShockWaveStart(XMFLOAT3 pos)
{
	//発射されていたら抜ける
	if (shockWave[1]->GetIsAlive()) { return; }

	//ゲージレベルが足りない場合は抜ける
	if (shockWaveGauge->GetGaugeLevel() == 0) { return; }
	//ゲージレベルに応じて巨大衝撃波の威力を変更
	int shockWavePowerLevel = 0;
	if (shockWaveGauge->GetGaugeLevel() == 1) { shockWavePowerLevel = 1; }
	else if (shockWaveGauge->GetGaugeLevel() == 2) { shockWavePowerLevel = 2; }
	else if (shockWaveGauge->GetGaugeLevel() == 3) { shockWavePowerLevel = 3; }
	else { return; }

	//巨大衝撃波発射
	shockWave[1]->BigWaveStart(pos, shockWavePowerLevel);

	//ゲージの0にする
	shockWaveGauge->UsePoint();

	//画面シェイク
	isShake = true;
}

void GameScene::SpawnStraighter()
{
	//生成時に初期座標と移動方向を決める
	XMFLOAT3 startPos = {};
	float angle = 0;

	//範囲
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
	XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

	//4パターンのランダムで初期座標と移動方向をセット
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; angle = 300; }

	//直進敵を生成
	enemys.push_back(Straighter::Create(startPos, angle));
}

void GameScene::SpawnDivision()
{
	//生成時に初期座標と移動方向を決める
	XMFLOAT3 startPos = {};
	float angle = 0;

	//範囲
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
	XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

	//4パターンのランダムで初期座標と移動方向をセット
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; angle = 300; }

	//分裂敵を生成
	enemys.push_back(Division::Create(startPos, angle));
}

void GameScene::SpawnReleaser()
{
	//生成時に初期座標と移動方向を決める
	XMFLOAT3 startPos = {};
	float angle = 0;

	//範囲
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
	XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

	//4パターンのランダムで初期座標と移動方向をセット
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; angle = 300; }

	//停止地点をランダム生成
	XMFLOAT3 stayPos = {};
	stayPos.x = (float)(rand() % 200 - 100);
	stayPos.y = (float)(rand() % 120 - 60);

	//放出的を生成
	enemys.push_back(Releaser::Create(startPos, stayPos));
}

void GameScene::SpawnChaser()
{
	//生成時に初期座標を決める
	XMFLOAT3 startPos = {};

	//範囲
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
	XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

	//4パターンのランダムで初期座標と移動方向をセット
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; }

	//追従敵を生成
	enemys.push_back(Chaser::Create(startPos));
}

void GameScene::SpawnEnemyToEnemy(BaseEnemy* enemy)
{
	//敵が分裂的だった場合
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Division)
	{
		//生成時に初期座標と移動方向を決める
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = enemy->GetMoveDegree();

		//3体生成する
		for (int i = 0; i < 3; i++)
		{
			angle += 120;

			//直進敵を生成する
			enemys.push_back(Straighter::Create(startPos, angle));
		}
	}

	//敵が放出的だった場合
	else if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Releaser)
	{
		//生成時に初期座標と移動方向を決める
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = (float)(rand() % 360);

		//直進敵を生成する
		enemys.push_back(Straighter::Create(startPos, angle));
	}
}

void GameScene::CameraUpdate(Camera* camera)
{
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

void GameScene::SpawnEnemyManager(int score)
{
	//Timer更新
	spawnTimer++;

	//Interval更新
	spawnInterval = 60;

	//Intervalは60以下にはならない
	if (spawnInterval <= 60)
	{
		spawnInterval = 60;
	}

	//Rate更新(1 + 破壊した壁の数 / 3)
	spawnRate = 1 + (score / 3);

	//Rateは5以上にはならない
	if (spawnRate >= 5)
	{
		spawnRate = 5;
	}

	//TimerがIntervalを超えたら敵を生成する
	if (spawnInterval <= spawnTimer)
	{
		//Timerを0に戻す
		spawnTimer = 0;

		//Rate体生成する
		for (int i = 0; i < spawnRate; i++)
		{
			//生成時に初期座標と移動方向を決定
			XMFLOAT3 startPos = {};
			float angle = 0;

			//範囲
			float range = 5.0f;
			XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
			XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

			//乱数で敵の種類を決定
			int enemyTypeRand = rand() % 10;
			if (enemyTypeRand <= 2) { enemyType = 0; }
			else if (enemyTypeRand <= 4) { enemyType = 1; }
			else if (enemyTypeRand <= 6) { enemyType = 2; }
			else { enemyType = 3; }

			//乱数で敵が出現する方向を決定
			int enemyDirectionRand = rand() % 4;
			if (enemyDirectionRand == 0) { enemyDirection = 0; }
			else if (enemyDirectionRand == 1) { enemyDirection = 1; }
			else if (enemyDirectionRand == 2) { enemyDirection = 2; }
			else { enemyDirection = 3; }

			//初期座標と移動方向を決定
			if (enemyDirection == 0)
			{
				//上側から出現
				//初期座標を決定
				startPos = { (float)(rand() % 300 - 150), startLineMax.y, 0 };

				//移動方向を決定(180±30)
				angle = (float)(rand() % 61) + 150;
			}
			else if (enemyDirection == 1)
			{
				//左側から出現
				//初期座標を決定
				startPos = { startLineMin.x, (float)(rand() % 120 - 60), 0 };

				//移動方向を決定(270±30)
				angle = (float)(rand() % 61) + 240;
			}
			else if (enemyDirection == 2)
			{
				//下側から出現
				//初期座標を決定
				startPos = { (float)(rand() % 300 - 150), startLineMin.y, 0 };

				//移動方向を決定(0±30)
				angle = (float)(rand() % 61) - 30;
				if (angle < 0)
				{
					angle += 360;
				}
			}
			else
			{
				//右側から出現
				//初期座標を決定
				startPos = { startLineMax.x, (float)(rand() % 120 - 60), 0 };

				//移動方向を決定(90±30)
				angle = (float)(rand() % 61) + 60;
			}

			//敵の生成
			if (enemyType == 0)
			{
				//敵1(直進型)を生成
				enemys.push_back(Straighter::Create(startPos, angle));
			}
			else if (enemyType == 1)
			{
				//敵2(拡散型)を生成
				enemys.push_back(Division::Create(startPos, angle));
			}
			else if (enemyType == 2)
			{
				//停止地点をランダム生成
				XMFLOAT3 stayPos = {};
				stayPos.x = (float)(rand() % 200 - 100);
				stayPos.y = (float)(rand() % 120 - 60);

				//敵3(放出型)を生成
				enemys.push_back(Releaser::Create(startPos, stayPos));
			}
			else
			{
				//敵4(追尾型)を生成
				enemys.push_back(Chaser::Create(startPos));
			}
		}
	}
}