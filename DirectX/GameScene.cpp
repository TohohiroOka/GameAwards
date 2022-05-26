#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "GameCollision.h"

#include "Chaser.h"
#include "Division.h"
#include "Releaser.h"
#include "TitleLogo.h"

const float radian = XM_PI / 180.0f;//ラジアン

GameScene::~GameScene()
{
	//newオブジェクトの破棄
	//safe_delete(audio);
	safe_delete(light);

	//モデル解放
	safe_delete(titleLogoModel);
	safe_delete(playerModel);
	safe_delete(waveModel);
	safe_delete(healingZoneModel);
	safe_delete(XButtonModel);
	safe_delete(chaserModel);
	safe_delete(divisionModel);
	safe_delete(releaserModel);


	//プレイヤー解放
	safe_delete(player);
	//衝撃波解放
	safe_delete(shockWave);

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
	safe_delete(backGround);

	//シーン遷移用暗転解放
	safe_delete(blackout);

	//ゲーム説明解放
	safe_delete(explanation);

	//UIを囲う枠解放
	safe_delete(UIFrame);

	//壁破壊スコア解放
	safe_delete(breakScore);
	//制限時間解放
	safe_delete(timeLimitGauge);
	//衝撃波用ゲージ解放
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
	playerModel = Model::CreateFromOBJ("player");//プレイヤーのモデル
	waveModel = Model::CreateFromOBJ("wave");//衝撃波のモデル
	healingZoneModel = Model::CreateFromOBJ("wavehealcore");//ゲージ回復地点のモデル
	XButtonModel = Model::CreateFromOBJ("Xbutton");//Xボタンのモデル
	chaserModel = Model::CreateFromOBJ("enemy1_4");//追跡敵のモデル
	divisionModel = Model::CreateFromOBJ("enemy2_3");//分裂敵のモデル
	releaserModel = Model::CreateFromOBJ("enemy3_2");//放出敵のモデル


	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/white1x1.png");
	Sprite::LoadTexture(2, L"Resources/num.png");
	Sprite::LoadTexture(3, L"Resources/backgame.png");
	Sprite::LoadTexture(4, L"Resources/pause.png");
	Sprite::LoadTexture(5, L"Resources/gauge.png");
	Sprite::LoadTexture(6, L"Resources/gaugeframe.png");
	Sprite::LoadTexture(7, L"Resources/ready.png");
	Sprite::LoadTexture(8, L"Resources/go.png");
	Sprite::LoadTexture(9, L"Resources/finish.png");
	Sprite::LoadTexture(10, L"Resources/result.png");
	Sprite::LoadTexture(11, L"Resources/retry.png");
	Sprite::LoadTexture(12, L"Resources/backtitle.png");
	Sprite::LoadTexture(13, L"Resources/time.png");
	Sprite::LoadTexture(14, L"Resources/timeframe.png");
	Sprite::LoadTexture(15, L"Resources/timegauge.png");
	Sprite::LoadTexture(16, L"Resources/background.png");
	Sprite::LoadTexture(17, L"Resources/blackframe.png");
	Sprite::LoadTexture(18, L"Resources/start.png");
	Sprite::LoadTexture(19, L"Resources/break.png");
	Sprite::LoadTexture(20, L"Resources/starttext.png");
	Sprite::LoadTexture(21, L"Resources/AButton.png");
	Sprite::LoadTexture(22, L"Resources/XButton.png");

	//デバッグテキスト生成
	DebugText::GetInstance()->Initialize(0);


	//プレイヤー生成
	player = Player::Create(playerModel);
	//衝撃波生成
	shockWave = ShockWave::Create(waveModel);

	//敵のモデルをセット
	Chaser::SetModel(chaserModel);
	Division::SetModel(divisionModel);
	Releaser::SetModel(releaserModel);
	TitleLogo::SetModel(titleLogoModel);

	//壁生成
	wall = WallManager::Create();
	//壁の当たり判定ラインをタイトルシーン用に設定
	WallLineSet(true);

	//エフェクト初期化
	effects = new StageEffect();
	effects->Initialize();

	//背景生成
	backGround = BackGround::Create(16);

	//シーン遷移用暗転生成
	blackout = Blackout::Create(1);

	//ゲーム説明生成
	explanation = Explanation::Create(20, 22);

	//UIを囲う枠生成
	UIFrame = UIFrame::Create(17, 18);


	//壁破壊スコア生成
	breakScore = BreakScore::Create(19, 2);
	//制限時間生成
	timeLimitGauge = TimeLimitGauge::Create(13, 14, 15);
	//衝撃波用ゲージ生成
	shockWaveGauge = ShockWaveGauge::Create(6, 5);

	//ReadyGo生成
	readyGo = ReadyGo::Create(7, 8);
	//ポーズシーンUI生成
	pauseUI = PauseUI::Create(1, 4, 3, 12, 21);
	//Finish生成
	finish = Finish::Create(9);
	//リザルトシーンUI生成
	resultUI = ResultUI::Create(1, 10, 19, 2, 11, 12, 21);

	//サウンドの読み込み
	Audio* audio = Audio::GetInstance();
	sound[0] = audio->SoundLoadWave("Resources/sound/select.wav");//カーソル移動音
	sound[1] = audio->SoundLoadWave("Resources/sound/enter.wav");//選択肢決定音
	sound[2] = audio->SoundLoadWave("Resources/sound/playerHit.wav");//プレイヤー被弾音
	sound[3] = audio->SoundLoadWave("Resources/sound/shockWave.wav");//プレイヤー衝撃波音
	sound[4] = audio->SoundLoadWave("Resources/sound/enemyHit.wav");//エネミー吹っ飛び音
	sound[5] = audio->SoundLoadWave("Resources/sound/enemyDead.wav");//エネミー壁衝突音
	sound[6] = audio->SoundLoadWave("Resources/sound/wallBreak.wav");//壁耐久値0音
	sound[7] = audio->SoundLoadWave("Resources/sound/bigShock.wav");//巨大衝撃波音
	sound[8] = audio->SoundLoadWave("Resources/sound/go.wav");//GO音
	sound[9] = audio->SoundLoadWave("Resources/sound/finish.wav");//FINISH音
	sound[10] = audio->SoundLoadWave("Resources/sound/wallSet.wav");//壁生成音
	sound[11] = audio->SoundLoadWave("Resources/sound/diffusion.wav");//拡散音
	sound[12] = audio->SoundLoadWave("Resources/sound/charge.wav");//必殺技溜まったときの音
	sound[13] = audio->SoundLoadWave("Resources/sound/result.wav");//リザルトのロゴ表示の音
	sound[14] = audio->SoundLoadWave("Resources/sound/chase.wav");//追尾する敵の移動音
	sound[15] = audio->SoundLoadWave("Resources/sound/enemyHit2.wav");//エネミー強吹っ飛び音
	sound[16] = audio->SoundLoadWave("Resources/sound/bgm.wav");//BGM
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();
	Audio* audio = Audio::GetInstance();

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

		//衝撃波発射
		if (player->GetIsShockWaveStart())
		{
			int titleScenePowerLevel = 1;
			ShockWaveStart(player->GetPosition(), titleScenePowerLevel);
		}
		//衝撃波更新
		shockWave->Update();

		//敵更新
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//更新処理
			(*itrEnemy)->Update();

			//削除状態になったら次のシーンへ
			if ((*itrEnemy)->GetIsDelete())
			{
				//サウンドの再生
				SoundManager(sound[5], false, false);
				scene = SceneName::ReadyGoScene;

				//プレイヤーをゲーム開始時の座標に移動状態にセット
				player->SetGameStartPos();

				//ゲーム説明を画面外に移動
				explanation->SetMoveOutScreen();
			}

			//敵が生きていなければ飛ばす
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//壁と敵の当たり判定を取る
			GameCollision::CheckWallToEnemy(wall, (*itrEnemy));

			//衝撃波と敵の当たり判定
			if (GameCollision::CheckShockWaveToEnemy(shockWave, (*itrEnemy)))
			{
				//サウンドの再生
				SoundManager(sound[4], false, false);
			}

		}
		//タイトルロゴのスポーンが完了したら
		if (TitleLogo::GetTriggerSpawnEnd())
		{
			//プレイヤー操作可能
			player->SetIsFreeMove(true);
			//ゲーム説明を画面内に移動させる
			explanation->SetMoveInScreen();
		}

		//壁更新
		wall->Update();
		//ゲーム説明更新
		explanation->Update();
		//衝撃波用ゲージ更新
		shockWaveGauge->Update();
	}

	//ReadyGoシーン
	else if (scene == SceneName::ReadyGoScene)
	{
		//プレイヤー更新
		player->Update();
		//衝撃波更新
		shockWave->Update();

		//プレイヤーがゲーム開始時の座標に到達したら
		if (player->GetTriggerMoveStartPosEnd())
		{
			//UIを画面上部に移動させる
			UIFrame->SetMoveGamePos();
			timeLimitGauge->SetMoveGamePos();
			shockWaveGauge->SetMoveGamePos();
		}
		//UI更新
		UIFrame->Update();
		timeLimitGauge->Update();
		shockWaveGauge->Update();

		//ゲーム説明更新
		explanation->Update();

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

				//壁の当たり判定ラインをゲームシーン用に設定
				WallLineSet(false);

				//プレイヤーの行動制限を解く
				player->SetIsFreeMove(true);
				//制限時間のカウントダウンを開始する
				timeLimitGauge->SetIsCountDown(true);
				//スタートボタンを表示する
				UIFrame->SetIsDrawStart(true);
			}
		}
	}

	//ゲームシーン
	else if (scene == SceneName::GamePlayScene)
	{
		//BGMの再生
		if (!isBGM)
		{
			//サウンドの再生
			SoundManager(sound[16], true, false);
		}

		//プレイヤー更新
		player->Update();
		//衝撃波発射
		if (player->GetIsShockWaveStart())
		{
			ShockWaveStart(player->GetPosition(), shockWaveGauge->GetGaugeLevel());
		}
		//衝撃波更新
		shockWave->Update();

		//壁がある場合
		if (wall->GetIsAlive())
		{
			//敵生成
			SpawnEnemyManager(breakScore->GetBreakScore(), timeLimitGauge->GetTimer());
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

			//壁と敵の当たり判定を取る
			if (GameCollision::CheckWallToEnemy(wall, (*itrEnemy)))
			{
				//サウンドの再生
				SoundManager(sound[5], false, false);
				//エフェクトを出す
				StageEffect::SetSmash((*itrEnemy)->GetPosition(), (*itrEnemy)->GetDamagePower());
				//振動オン
				Xinput->StartVibration(XInputManager::STRENGTH::MEDIUM, 10);
			}
			//範囲外判定
			XMFLOAT3 pos = (*itrEnemy)->GetPosition();
			if (pos.x <= outsideMinPosition.x || pos.x >= outsideMaxPosition.x ||
				pos.y <= outsideMinPosition.y || pos.y >= outsideMaxPosition.y)
			{
				(*itrEnemy)->SetDelete();
			}

			//プレイヤーと敵の当たり判定
			if (GameCollision::CheckPlayerToEnemy(player, (*itrEnemy)))
			{
				//サウンドの再生
				SoundManager(sound[2], false, false);
				//画面シェイク
				isShake = true;
				//振動オン
				Xinput->StartVibration(XInputManager::STRENGTH::SMALL, 10);
			}

			//衝撃波と敵の当たり判定
			if (GameCollision::CheckShockWaveToEnemy(shockWave, (*itrEnemy)))
			{
				//サウンドの再生
				SoundManager(sound[4], false, false);
			}
		}

		//壁更新
		wall->Update();
		//壁が壊れた瞬間
		if (wall->GetTriggerBreak())
		{
			//敵を削除
			for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
			{
				(*itrEnemy)->SetDelete();

				//エフェクトを出す
				StageEffect::SetDeleteEnemey((*itrEnemy)->GetPosition());
			}

			//壊したスコア加算
			breakScore->AddScore();
			//タイムリミットが伸びる
			timeLimitGauge->Recovery(10);
		}

		//UIフレーム更新
		UIFrame->Update();

		//壁破壊スコア更新
		breakScore->Update();
		//制限時間更新
		timeLimitGauge->Update();
		//制限時間が0になったら次のシーンへ
		if (timeLimitGauge->GetIsCountDownEnd())
		{
			scene = SceneName::FinishScene;

			//敵を削除
			for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
			{
				(*itrEnemy)->SetDelete();

				//エフェクトを出す
				StageEffect::SetDeleteEnemey((*itrEnemy)->GetPosition());
			}

			//サウンドの停止
			isBGM = false;
			SoundManager(sound[16], true, true);

			//プレイヤーを自由に操作できなくする
			player->SetIsFreeMove(false);

			//リザルトシーン用に壁破壊数を渡しておく
			resultUI->SetBreakWallNum(breakScore->GetBreakScore());
		}

		//衝撃波用ゲージ更新
		if (CheckPlayerToHealingZone(player))
		{
			shockWaveGauge->IncreasePoint();
			//回復エフェクト
			StageEffect::SetHeal(player->GetPosition());
		}
		else
		{
			shockWaveGauge->DecreasePoint();
		}
		shockWaveGauge->Update();

		//指定したボタンを押すとポーズシーンへ
		if (input->TriggerKey(DIK_1) || Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_START))
		{
			//サウンドの再生
			SoundManager(sound[1], false, false);

			scene = SceneName::PauseScene;

			//ポーズシーンをリセット
			pauseUI->Reset();

			//スタートボタンを表示しない
			UIFrame->SetIsDrawStart(false);
		}


		std::string wallHP = std::to_string(wall->GetHP());
		DebugText::GetInstance()->Print("HP : " + wallHP, 200, 200);
	}

	//ポーズシーン
	else if (scene == SceneName::PauseScene)
	{
		//UIフレーム更新
		UIFrame->Update();

		//ポーズシーンUI更新
		pauseUI->Update();
		//確定していないとき、指定したボタンを押すとシーン切り替え選択を確定させる
		if (!pauseUI->GetIsSelect() && (input->TriggerKey(DIK_1) ||
			Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_A)))
		{
			//サウンドの再生
			SoundManager(sound[1], false, false);

			//確定させる
			pauseUI->SetSelect();

			//ゲームに戻る
			if (pauseUI->GetIsBackGame())
			{
				scene = SceneName::GamePlayScene;

				//スタートボタンを表示する
				UIFrame->SetIsDrawStart(true);
			}
			//タイトルシーンに戻る
			else
			{
				//サウンドの停止
				isBGM = false;
				SoundManager(sound[16], true, true);

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
		shockWave->Update();

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

			//壁と敵の当たり判定を取る
			GameCollision::CheckWallToEnemy(wall, (*itrEnemy));
		}

		//壁更新
		wall->Update();
		//UI更新
		UIFrame->Update();
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
			wall->SetOutScreen();

			//UIを画面外に移動させる
			UIFrame->SetMoveResultPos();
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
		UIFrame->Update();
		timeLimitGauge->Update();
		shockWaveGauge->Update();

		//リザルトシーンUI更新
		resultUI->Update();
		//全ての描画が終わったら
		if (resultUI->GetIsDrawAll())
		{
			//確定していないとき、指定したボタンを押すとシーン切り替え選択を確定させる
			if (!resultUI->GetIsSelect() && (input->TriggerKey(DIK_Z) ||
				Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_A)))
			{
				//サウンドの再生
				SoundManager(sound[1], false, false);

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

		//壁描画
		wall->Draw();

		//プレイヤー描画
		player->Draw();

		//衝撃波描画
		shockWave->Draw();

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

		//ゲーム説明描画
		explanation->Draw();

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
		//衝撃波用ゲージ描画
		shockWaveGauge->Draw();

		//ゲーム説明描画
		explanation->Draw();

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
		shockWave->Draw();


		//敵描画
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();


		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//壁破壊スコア描画
		breakScore->Draw();

		//UIを囲う枠描画
		UIFrame->Draw();

		//制限時間描画
		timeLimitGauge->Draw();
		//衝撃波用ゲージ描画
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
		shockWave->Draw();


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
		//衝撃波用ゲージ描画
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
		shockWave->Draw();


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
		//衝撃波用ゲージ描画
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
		//衝撃波用ゲージ描画
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
	shockWave->Reset();

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

	//ゲーム説明初期化
	explanation->Reset();
	//UIフレーム初期化
	UIFrame->Reset();

	//壁破壊スコア初期化
	breakScore->Reset();
	//制限時間初期化
	timeLimitGauge->Reset();
	//衝撃波用ゲージ初期化
	shockWaveGauge->Reset();

	//ReadyGo初期化
	readyGo->Reset();
	//Finish初期化
	finish->Reset();
	//リザルトシーンUI初期化
	resultUI->Reset();

	//シーンをタイトルシーンに移行
	scene = SceneName::TitleStayScene;

	//壁の当たり判定ラインをタイトルシーン用に戻す
	WallLineSet(true);

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
	shockWave->Reset();

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
	wall->Reset(false);

	//UIフレーム初期化
	UIFrame->Reset();

	//壁破壊スコア初期化
	breakScore->Reset();
	//制限時間初期化
	timeLimitGauge->Reset();
	//衝撃波用ゲージ初期化
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

void GameScene::ShockWaveStart(XMFLOAT3 pos, int powerLevel)
{
	//プレイヤーが自由に動けない（ダメージ状態）なら抜ける
	if (!player->GetIsFreeMove()) { return; }

	//発射されていたら抜ける
	if (shockWave->GetIsAlive()) { return; }

	//衝撃波発射
	shockWave->ShockWaveStart(pos, powerLevel);

	//画面シェイク
	isShake = true;

	//サウンドの再生
	SoundManager(sound[3], false, false);
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

void GameScene::SpawnEnemyToEnemy(BaseEnemy* enemy)
{
	Audio* audio = Audio::GetInstance();

	//敵が分裂的だった場合
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Division)
	{
		//生成時に初期座標と移動方向を決める
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = enemy->GetMoveDegree();

		//サウンドの再生
		SoundManager(sound[11], false, false);

		//3体生成する
		for (int i = 0; i < 3; i++)
		{
			angle += 120;

			//追従敵を生成する
			enemys.push_back(Chaser::Create(startPos, angle, true));
		}
	}

	//敵が放出的だった場合
	else if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Releaser)
	{
		//生成時に初期座標と移動方向を決める
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = (float)(rand() % 360);

		//サウンドの再生
		SoundManager(sound[11], false, false);

		//追従敵を生成する
		enemys.push_back(Chaser::Create(startPos, angle, true));
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

void GameScene::SpawnEnemyManager(int score, int time)
{
	//Timer更新
	spawnTimer++;

	//Interval更新
	spawnInterval = 90 - score;

	//Intervalは70以下にはならない
	if (spawnInterval <= 70)
	{
		spawnInterval = 70;
	}

	//Rate更新
	if (score <= 10)
	{
		spawnRate = 1;
	}
	else
	{
		spawnRate = 2;
	}

	//Rateは10以上にはならない
	if (spawnRate >= 10)
	{
		spawnRate = 10;
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
			int enemyTypeRand;
			if (score <= 2) { enemyTypeRand = rand() % 4; }
			else if (score <= 6) { enemyTypeRand = rand() % 7; }
			else { enemyTypeRand = rand() % 10; }

			if (enemyTypeRand <= 3) { enemyType = 0; }
			else if (enemyTypeRand <= 6) { enemyType = 1; }
			else { enemyType = 2; }

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
				startPos = { startLineMin.x, (float)(rand() % 140 - 70), 0 };

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
				startPos = { startLineMax.x, (float)(rand() % 140 - 70), 0 };

				//移動方向を決定(90±30)
				angle = (float)(rand() % 61) + 60;
			}

			//敵の生成
			if (enemyType == 0)
			{
				//敵1(追尾型)を生成
				enemys.push_back(Chaser::Create(startPos));
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
		}
	}
}

void GameScene::WallLineSet(const bool isTitle)
{
	//壁の生成範囲
	XMFLOAT2 minWallPosition = WallObject::GetWallMinPosition();
	XMFLOAT2 maxWallPosition = WallObject::GetWallMaxPosition();

	//タイトル画面のとき
	if (isTitle)
	{
		minWallPosition.y += 8.0f;
		maxWallPosition.y += 12.0f;
	}
	//ゲーム画面のとき
	else
	{
		maxWallPosition.y += 4.0f;
	}
	//壁生成範囲との差分
	const float wallPosDis = 18.0f;
	minWallLinePosition = { minWallPosition.x + wallPosDis, minWallPosition.y + wallPosDis };
	maxWallLinePosition = { maxWallPosition.x - wallPosDis, maxWallPosition.y - wallPosDis };
	//範囲外範囲
	const XMFLOAT2 outsideRange = { 25.0f, 45.0f };
	outsideMinPosition = { minWallLinePosition.x - outsideRange.x, minWallLinePosition.y - outsideRange.y };
	outsideMaxPosition = { maxWallLinePosition.x + outsideRange.x, maxWallLinePosition.y + outsideRange.y };

	//壁割り当て
	Player::SetMoveRange(minWallLinePosition, maxWallLinePosition);
	XMFLOAT2 enemyWallLineMin = minWallLinePosition;
	enemyWallLineMin.y -= 2;
	XMFLOAT2 enemyWallLineMax = maxWallLinePosition;
	enemyWallLineMax.y += 2;
	BaseEnemy::SetWallLine(enemyWallLineMin, enemyWallLineMax);
}

bool GameScene::CheckPlayerToHealingZone(Player* player)
{
	//衝突用に座標と半径の大きさを借りる
	XMFLOAT3 zonePos = { 0, -10, 0 };
	float zoneSize = 30.0f;
	XMFLOAT3 playerPos = player->GetPosition();
	float playerSize = player->GetScale().x;

	//衝突判定を計算
	bool isCollision = Collision::CheckCircle2Circle(
		zonePos, zoneSize, playerPos, playerSize);

	return isCollision;
}

void GameScene::SoundManager(int soundNum, bool isBGM, bool stop)
{
	Audio* audio = Audio::GetInstance();

	//SEの場合
	if (!isBGM)
	{
		//再生
		if (!stop)
		{
			audio->SoundPlayWava(soundNum, false);
		}
		//停止
		else
		{
			audio->StopSound(soundNum);
		}
	}
	//BGMの場合
	else
	{
		//再生
		if (!stop)
		{
			audio->SoundPlayWava(soundNum, true);
		}
		//停止
		else
		{
			audio->StopSound(soundNum);
		}
	}
}