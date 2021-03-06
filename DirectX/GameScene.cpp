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

const float radian = XM_PI / 180.0f;//WA

GameScene::~GameScene()
{
	//newIuWFNgΜjό
	//safe_delete(audio);
	safe_delete(light);

	//fπϊ
	safe_delete(titleLogoModel);
	safe_delete(playerModel);
	safe_delete(waveModel);
	safe_delete(healingZoneModel);
	safe_delete(XButtonModel);
	safe_delete(chaserModel);
	safe_delete(divisionModel);
	safe_delete(releaserModel);


	//vC[πϊ
	safe_delete(player);
	//Υgπϊ
	safe_delete(shockWave);

	//Gπϊ
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		safe_delete(*itrEnemy);
	}
	//GΜXgπϊ
	enemys.clear();

	//Ηπϊ
	safe_delete(wall);

	//GtFNgΜπϊ
	safe_delete(effects);

	//wiΜπϊ
	safe_delete(backGround);

	//V[JΪpΓ]πϊ
	safe_delete(blackout);

	//Q[ΰΎπϊ
	safe_delete(explanation);

	//UIπΝ€gπϊ
	safe_delete(UIFrame);

	//ΗjσXRAπϊ
	safe_delete(breakScore);
	//bVπϊ
	safe_delete(rush);
	//§ΐΤπϊ
	safe_delete(timeLimitGauge);
	//ΥgpQ[Wπϊ
	safe_delete(shockWaveGauge);
	//ReadyGoπϊ
	safe_delete(readyGo);
	//|[YV[UIπϊ
	safe_delete(pauseUI);
	//Finishπϊ
	safe_delete(finish);
	//UgV[UIπϊ
	safe_delete(resultUI);
}

void GameScene::Initialize(Camera* camera)
{
	// nullptr`FbN
	assert(camera);

	//Cg
	light = LightGroup::Create();
	light->DefaultLightSetting();
	Object3d::SetLightGroup(light);

	titleLogoModel = Model::CreateFromOBJ("titleLogo");//^CgSΜf
	playerModel = Model::CreateFromOBJ("player");//vC[Μf
	waveModel = Model::CreateFromOBJ("wave");//ΥgΜf
	healingZoneModel = Model::CreateFromOBJ("wavehealcore");//Q[Wρn_Μf
	XButtonModel = Model::CreateFromOBJ("Xbutton");//X{^Μf
	chaserModel = Model::CreateFromOBJ("enemy1_4");//ΗΥGΜf
	divisionModel = Model::CreateFromOBJ("enemy2_3");//ͺτGΜf
	releaserModel = Model::CreateFromOBJ("enemy3_2");//ϊoGΜf


	//XvCg€ΚeNX`Ηέέ
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
	Sprite::LoadTexture(23, L"Resources/rush.png");

	//fobOeLXgΆ¬
	DebugText::GetInstance()->Initialize(0);

	//vC[Ά¬
	player = Player::Create(playerModel);
	//ΥgΆ¬
	shockWave = ShockWave::Create(waveModel);

	//GΜfπZbg
	Chaser::SetModel(chaserModel);
	Division::SetModel(divisionModel);
	Releaser::SetModel(releaserModel);
	TitleLogo::SetModel(titleLogoModel);

	//ΗΆ¬
	wall = WallManager::Create();
	//ΗΜ½θ»θCπ^CgV[pΙέθ
	WallLineSet(true);

	//GtFNgϊ»
	effects = new StageEffect();
	effects->Initialize();

	//wiΆ¬
	backGround = BackGround::Create(16);

	//V[JΪpΓ]Ά¬
	blackout = Blackout::Create(1);

	//Q[ΰΎΆ¬
	explanation = Explanation::Create(20, 22);

	//UIπΝ€gΆ¬
	UIFrame = UIFrame::Create(17, 18);


	//ΗjσXRAΆ¬
	breakScore = BreakScore::Create(19, 2);
	//bVΆ¬
	rush = Rush::Create(23);
	//§ΐΤΆ¬
	timeLimitGauge = TimeLimitGauge::Create(13, 14, 15);
	//ΥgpQ[WΆ¬
	shockWaveGauge = ShockWaveGauge::Create(5);

	//ReadyGoΆ¬
	readyGo = ReadyGo::Create(7, 8);
	//|[YV[UIΆ¬
	pauseUI = PauseUI::Create(1, 4, 3, 12, 21);
	//FinishΆ¬
	finish = Finish::Create(9);
	//UgV[UIΆ¬
	resultUI = ResultUI::Create(1, 10, 19, 2, 11, 12, 21);

	//TEhΜΗέέ
	Audio* audio = Audio::GetInstance();
	sound[0] = audio->SoundLoadWave("Resources/sound/select.wav");//J[\Ϊ?Ή
	sound[1] = audio->SoundLoadWave("Resources/sound/enter.wav");//IπθΉ
	sound[2] = audio->SoundLoadWave("Resources/sound/playerHit.wav");//vC[νeΉ
	sound[3] = audio->SoundLoadWave("Resources/sound/shockWave.wav");//vC[ΥgΉ
	sound[4] = audio->SoundLoadWave("Resources/sound/enemyHit.wav");//Gl~[ΑςΡΉ
	sound[5] = audio->SoundLoadWave("Resources/sound/enemyDead.wav");//Gl~[ΗΥΛΉ
	sound[6] = audio->SoundLoadWave("Resources/sound/wallBreak.wav");//ΗΟvl0Ή
	sound[7] = audio->SoundLoadWave("Resources/sound/bigShock.wav");//εΥgΉ
	sound[8] = audio->SoundLoadWave("Resources/sound/go.wav");//GOΉ
	sound[9] = audio->SoundLoadWave("Resources/sound/finish.wav");//FINISHΉ
	sound[10] = audio->SoundLoadWave("Resources/sound/wallSet.wav");//ΗΆ¬Ή
	sound[11] = audio->SoundLoadWave("Resources/sound/diffusion.wav");//gUΉ
	sound[12] = audio->SoundLoadWave("Resources/sound/charge.wav");//KEZ­άΑ½Ζ«ΜΉ
	sound[13] = audio->SoundLoadWave("Resources/sound/result.wav");//UgΜS\¦ΜΉ
	sound[14] = audio->SoundLoadWave("Resources/sound/chase.wav");//Ηφ·ιGΜΪ?Ή
	sound[15] = audio->SoundLoadWave("Resources/sound/enemyHit2.wav");//Gl~[­ΑςΡΉ
	sound[16] = audio->SoundLoadWave("Resources/sound/bgm.wav");//BGM
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();
	Audio* audio = Audio::GetInstance();

	//^Cgυ
	if (scene == SceneName::TitleStayScene)
	{
		//Γ]π
		if (blackout->GetIsBlackReturn())
		{
			//V[JΪpΓ]XV
			blackout->Update();
		}
		else
		{
			//oͺIΉ΅½ηΙs­
			if (wall->GetIsAlive())
			{
				scene = SceneName::TitleScene;

				//vC[πX|[σΤΙZbg
				player->SetSpawn();
			}

			//ΗXV
			wall->Update();
		}
	}

	//^CgV[
	else if (scene == SceneName::TitleScene)
	{
		//vC[XV
		player->Update();

		//X|[ͺIΉ΅½η
		if (player->GetTriggerSpawnEnd())
		{
			//^CgSΆ¬
			enemys.push_back(TitleLogo::Create());
		}

		//Υg­Λ
		if (player->GetIsShockWaveStart())
		{
			ShockWaveStart(player->GetPosition(), shockWaveGauge->GetGaugeLevel());
		}
		//ΥgXV
		shockWave->Update();

		//GXV
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//XV
			(*itrEnemy)->Update();

			//GͺΆ«Δ’Θ―κΞςΞ·
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//ΗΖGΜ½θ»θπζι
			if (GameCollision::CheckWallToEnemy(wall, (*itrEnemy)))
			{
				//TEhΜΔΆ
				SoundManager(sound[5], false, false);
				//U?I
				Xinput->StartVibration(XInputManager::STRENGTH::MEDIUM, 10);
			}

			//ΥgΖGΜ½θ»θ
			if (GameCollision::CheckShockWaveToEnemy(shockWave, (*itrEnemy)))
			{
				//TEhΜΔΆ
				SoundManager(sound[4], false, false);
			}
		}
		//^CgSΜX|[ͺ?Ή΅½η
		if (TitleLogo::GetTriggerSpawnEnd())
		{
			//vC[μΒ\
			player->SetIsFreeMove(true);
			//Q[ΰΎπζΚΰΙΪ?³Ήι
			explanation->SetMoveInScreen();
			//ΥgQ[WΜXVπJn
			shockWaveGauge->SetIsUpdate(true);
			//^CgphpΙΨθΦ¦
			wall->SetBreakCount(3);
		}

		//ΗXV
		wall->Update();
		//Ηͺσκ½ηΜV[Φ
		if (wall->GetTriggerBreak())
		{
			scene = SceneName::ReadyGoScene;

			//TEhΜΔΆ
			SoundManager(sound[5], false, false);

			//vC[πQ[JnΜΐWΙΪ?σΤΙZbg
			player->SetGameStartPos();
			//Q[ΰΎπζΚOΙΪ?
			explanation->SetMoveOutScreen();
			//ΥgQ[WπZbg·ι
			shockWaveGauge->SetGaugeReset();

			//^CgSπν
			for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
			{
				(*itrEnemy)->SetDelete();
			}
		}

		//Q[ΰΎXV
		explanation->Update();

		//ΥgpQ[WXV
		if (CheckPlayerToHealingZone(player))
		{
			shockWaveGauge->IncreasePoint();
			//ρGtFNg
			StageEffect::SetHeal(player->GetPosition());
		}
		else
		{
			shockWaveGauge->DecreasePoint();
		}
		shockWaveGauge->Update();
	}

	//ReadyGoV[
	else if (scene == SceneName::ReadyGoScene)
	{
		//vC[XV
		player->Update();
		//ΥgXV
		shockWave->Update();

		//vC[ͺQ[JnΜΐWΙB΅½η
		if (player->GetTriggerMoveStartPosEnd())
		{
			//UIπζΚγΙΪ?³Ήι
			UIFrame->SetMoveGamePos();
			timeLimitGauge->SetMoveGamePos();
		}
		//UIXV
		UIFrame->Update();
		timeLimitGauge->Update();
		shockWaveGauge->Update();

		//Q[ΰΎXV
		explanation->Update();

		//ΗΜXV
		wall->Update();

		//ΗͺΆ«Δ’½η
		if (wall->GetIsAlive())
		{
			//ReadyGoXvCgXV
			readyGo->Update();
			//ReadyGoΜGoάΕ\¦ͺIνΑ½ηΜV[Φ
			if (readyGo->GetIsReadyGoEnd())
			{
				//^CgphpΙΨθΦ¦
				wall->SetBreakCount(0);

				scene = SceneName::GamePlayScene;

				//ΗΜ½θ»θCπQ[V[pΙέθ
				WallLineSet(false);

				//vC[Μs?§ΐππ­
				player->SetIsFreeMove(true);
				//§ΐΤΜJEg_EπJn·ι
				timeLimitGauge->SetIsCountDown(true);
				//X^[g{^π\¦·ι
				UIFrame->SetIsDrawStart(true);
			}
		}
	}

	//Q[V[
	else if (scene == SceneName::GamePlayScene)
	{
		//BGMΜΔΆ
		if (!isBGM)
		{
			//TEhΜΔΆ
			SoundManager(sound[16], true, false);
		}

		//vC[XV
		player->Update();
		//Υg­Λ
		if (player->GetIsShockWaveStart())
		{
			ShockWaveStart(player->GetPosition(), shockWaveGauge->GetGaugeLevel());
		}
		//ΥgXV
		shockWave->Update();

		//Ηͺ ικ
		if (wall->GetIsAlive())
		{
			//GΆ¬
			SpawnEnemyManager(breakScore->GetBreakScore(), timeLimitGauge->GetTimer());
		}

		//GXV
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//XV
			(*itrEnemy)->Update();

			//GͺΆ«Δ’Θ―κΞςΞ·
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//GΆ¬tOͺtrueΜΖ«GπΆ¬·ι
			if ((*itrEnemy)->GetIsCreateEnemy())
			{
				SpawnEnemyToEnemy(*itrEnemy);
			}

			//ΗΖGΜ½θ»θπζι
			if (GameCollision::CheckWallToEnemy(wall, (*itrEnemy)))
			{
				//X|[^C}[πΕεάΕγ°ι
				spawnTimer = 300;
				//TEhΜΔΆ
				SoundManager(sound[5], false, false);
				//U?I
				Xinput->StartVibration(XInputManager::STRENGTH::MEDIUM, 10);
			}
			//ΝΝO»θ
			XMFLOAT3 pos = (*itrEnemy)->GetPosition();
			if (pos.x <= outsideMinPosition.x || pos.x >= outsideMaxPosition.x ||
				pos.y <= outsideMinPosition.y || pos.y >= outsideMaxPosition.y)
			{
				(*itrEnemy)->SetDelete();
			}

			//vC[ΖGΜ½θ»θ
			if (GameCollision::CheckPlayerToEnemy(player, (*itrEnemy)))
			{
				//TEhΜΔΆ
				SoundManager(sound[2], false, false);
				//ζΚVFCN
				isShake = true;
				//U?I
				Xinput->StartVibration(XInputManager::STRENGTH::SMALL, 10);
			}

			//ΥgΖGΜ½θ»θ
			if (GameCollision::CheckShockWaveToEnemy(shockWave, (*itrEnemy)))
			{
				//TEhΜΔΆ
				SoundManager(sound[4], false, false);
			}
		}

		//ΗXV
		wall->Update();
		//Ηͺσκ½uΤ
		if (wall->GetTriggerBreak())
		{
			//Gπν
			for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
			{
				(*itrEnemy)->SetDelete();

				//GͺΑ¦ιΜGtFNg
				StageEffect::SetDeleteEnemey((*itrEnemy)->GetPosition());
			}

			//σ΅½XRAΑZ
			breakScore->AddScore();
			//^C~bgͺLΡι
			timeLimitGauge->Recovery(7);
		}

		//UIt[XV
		UIFrame->Update();

		//ΗjσXRAXV
		breakScore->Update();
		//§ΐΤXV
		timeLimitGauge->Update();
		//§ΐΤͺ0ΙΘΑ½ηΜV[Φ
		if (timeLimitGauge->GetIsCountDownEnd())
		{
			scene = SceneName::FinishScene;

			//Gπν
			for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
			{
				(*itrEnemy)->SetDelete();

				//GͺΑ¦ιΜGtFNg
				StageEffect::SetDeleteEnemey((*itrEnemy)->GetPosition());
			}

			//TEhΜβ~
			isBGM = false;
			SoundManager(sound[16], true, true);

			//vC[π©RΙμΕ«Θ­·ι
			player->SetIsFreeMove(false);

			//UgV[pΙΗjσπn΅Δ¨­
			resultUI->SetBreakWallNum(breakScore->GetBreakScore());
		}

		//ΥgpQ[WXV
		if (CheckPlayerToHealingZone(player))
		{
			shockWaveGauge->IncreasePoint();
			//ρGtFNg
			StageEffect::SetHeal(player->GetPosition());
		}
		else
		{
			shockWaveGauge->DecreasePoint();
		}
		shockWaveGauge->Update();

		//bVXV
		if (isRushStart)
		{
			rush->SetMoveInScreen();

			//gK[ΘΜΕtOπ~λ΅Δ¨­
			isRushStart = false;
		}
		else if (isRushEnd)
		{
			rush->SetMoveOutScreen();

			//gK[ΘΜΕtOπ~λ΅Δ¨­
			isRushEnd = false;
		}
		rush->Update();

		//wθ΅½{^π·Ζ|[YV[Φ
		if (input->TriggerKey(DIK_1) || Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_START))
		{
			//TEhΜΔΆ
			SoundManager(sound[1], false, false);

			scene = SceneName::PauseScene;

			//|[YV[πZbg
			pauseUI->Reset();

			//X^[g{^π\¦΅Θ’
			UIFrame->SetIsDrawStart(false);
		}
	}

	//|[YV[
	else if (scene == SceneName::PauseScene)
	{
		//UIt[XV
		UIFrame->Update();

		//|[YV[UIXV
		pauseUI->Update();
		//mθ΅Δ’Θ’Ζ«Awθ΅½{^π·ΖV[ΨθΦ¦Iππmθ³Ήι
		if (!pauseUI->GetIsSelect() && (input->TriggerKey(DIK_1) ||
			Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_A)))
		{
			//TEhΜΔΆ
			SoundManager(sound[1], false, false);

			//mθ³Ήι
			pauseUI->SetSelect();

			//Q[Ιίι
			if (pauseUI->GetIsBackGame())
			{
				scene = SceneName::GamePlayScene;

				//X^[g{^π\¦·ι
				UIFrame->SetIsDrawStart(true);
			}
			//^CgV[Ιίι
			else
			{
				//TEhΜβ~
				isBGM = false;
				SoundManager(sound[16], true, true);

				//Γ]Jn
				blackout->SetBlackout();
			}
		}

		//V[JΪpΓ]XV
		blackout->Update();
		//Γ]ΕζΚͺ^ΑΓΙΘΑ½η^CgV[Ιίι
		if (blackout->GetIsAllBlack())
		{
			//^CgV[Ϊspϊ»
			ResetTitleScene();

			//Γ]ππσΤΙZbg
			blackout->SetBlackoutReturn();
		}
	}

	//FinishV[
	else if (scene == SceneName::FinishScene)
	{
		//vC[XV
		player->Update();
		//ΥgXV
		shockWave->Update();

		//GXV
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//XV
			(*itrEnemy)->Update();

			//GͺΆ«Δ’Θ―κΞςΞ·
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//GΆ¬tOͺtrueΜΖ«GπΆ¬·ι
			if ((*itrEnemy)->GetIsCreateEnemy())
			{
				SpawnEnemyToEnemy(*itrEnemy);
			}

			//ΗΖGΜ½θ»θπζι
			GameCollision::CheckWallToEnemy(wall, (*itrEnemy));
		}

		//ΗXV
		wall->Update();
		//UIXV
		UIFrame->Update();
		timeLimitGauge->Update();
		shockWaveGauge->Update();
		rush->Update();

		//FinishXV
		finish->Update();
		//Finishπ\¦΅I¦½ηΜV[Φ
		if (finish->GetIsFinishSpriteMoveEnd())
		{
			scene = SceneName::ResultScene;

			//GπUgV[pΜ?«ΙΟ¦ι
			BaseEnemy::SetIsResultMove(true);

			//ΗπUgV[pΙζΚOΙ?©·
			wall->SetOutScreen();

			//UIπζΚOΙΪ?³Ήι
			UIFrame->SetMoveResultPos();
			timeLimitGauge->SetMoveResultPos();

			//bVͺζΚΰΙ’ικζΚOΙΪ?³Ήι
			if (rush->GetIsInScreen())
			{
				rush->SetMoveOutScreen();
			}
		}
	}

	//UgV[
	else if (scene == SceneName::ResultScene)
	{
		//vC[XV
		player->Update();
		//GXV
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//XV
			(*itrEnemy)->Update();

			//GͺΆ«Δ’Θ―κΞςΞ·
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//ΗͺΘ’ΜΕAζΚOΙo½ην·ι
			XMFLOAT3 pos = (*itrEnemy)->GetPosition();
			//ΝΝO»θ
			if (pos.x <= outsideMinPosition.x || pos.x >= outsideMaxPosition.x ||
				pos.y <= outsideMinPosition.y || pos.y >= outsideMaxPosition.y)
			{
				(*itrEnemy)->SetDelete();
			}
		}

		//ΗXV
		wall->Update();
		//UIXV
		UIFrame->Update();
		timeLimitGauge->Update();
		shockWaveGauge->Update();
		rush->Update();

		//UgV[UIXV
		resultUI->Update();
		//SΔΜ`ζͺIνΑ½η
		if (resultUI->GetIsDrawAll())
		{
			//mθ΅Δ’Θ’Ζ«Awθ΅½{^π·ΖV[ΨθΦ¦Iππmθ³Ήι
			if (!resultUI->GetIsSelect() && (input->TriggerKey(DIK_Z) ||
				Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_A)))
			{
				//TEhΜΔΆ
				SoundManager(sound[1], false, false);

				//mθ³Ήι
				resultUI->SetSelect();

				//gC
				if (resultUI->GetIsRetry())
				{
					ResetGame();
				}
				//^CgV[Ιίι
				else
				{
					//Γ]Jn
					blackout->SetBlackout();
				}
			}
		}
		//V[JΪpΓ]XV
		blackout->Update();
		//Γ]ΕζΚͺ^ΑΓΙΘΑ½η^CgV[Ιίι
		if (blackout->GetIsAllBlack())
		{
			//^CgV[Ϊspϊ»
			ResetTitleScene();

			//Γ]ππσΤΙZbg
			blackout->SetBlackoutReturn();
		}
	}

	//Gν
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//νtOͺtrueΘην
		if ((*itrEnemy)->GetIsDelete())
		{
			//vfπνAXg©ηO·ι
			safe_delete(*itrEnemy);
			itrEnemy = enemys.erase(itrEnemy);
			continue;
		}
		//forͺπρ·
		itrEnemy++;
	}

	//GtFNgΜXV
	effects->Update(camera);
	//wiXV
	backGround->Update();
	//UIπΝ€gXV
	UIFrame->Update();
	//JXV
	CameraUpdate(camera);

	input = nullptr;
}

void GameScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//^CgV[
	if (scene == SceneName::TitleStayScene || scene == SceneName::TitleScene)
	{
		//XvCgwΚ`ζ
		Sprite::PreDraw(cmdList);

		//wi`ζ
		backGround->Draw();
		//ΥgpQ[W`ζ
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//IuWFNg`ζ
		Object3d::PreDraw(cmdList);

		//Η`ζ
		wall->Draw();

		//vC[`ζ
		player->Draw();

		//Υg`ζ
		shockWave->Draw();

		//G`ζ
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();

		//XvCgOΚ`ζ
		Sprite::PreDraw(cmdList);

		//UIπΝ€g`ζ
		UIFrame->Draw();

		//Q[ΰΎ`ζ
		explanation->Draw();

		//V[JΪpΓ]`ζ
		blackout->Draw();

		Sprite::PostDraw();
	}
	//ReadyGoV[
	else if (scene == SceneName::ReadyGoScene)
	{
		//XvCgwΚ`ζ
		Sprite::PreDraw(cmdList);

		//wi`ζ
		backGround->Draw();
		//ΥgpQ[W`ζ
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//IuWFNg`ζ
		Object3d::PreDraw(cmdList);

		//Η`ζ
		wall->Draw();

		//vC[`ζ
		player->Draw();

		Object3d::PostDraw();

		//XvCgOΚ`ζ
		Sprite::PreDraw(cmdList);

		//UIπΝ€g`ζ
		UIFrame->Draw();

		//ReadyGo`ζ
		readyGo->Draw();
		//§ΐΤ`ζ
		timeLimitGauge->Draw();

		//Q[ΰΎ`ζ
		explanation->Draw();

		Sprite::PostDraw();
	}
	//Q[V[
	else if (scene == SceneName::GamePlayScene)
	{
		//XvCgwΚ`ζ
		Sprite::PreDraw(cmdList);

		//wi`ζ
		backGround->Draw();
		//ΥgpQ[W`ζ
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//IuWFNg`ζ
		Object3d::PreDraw(cmdList);

		//Η`ζ
		wall->Draw();

		//vC[`ζ
		player->Draw();

		//Υg`ζ
		shockWave->Draw();


		//G`ζ
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();


		//XvCgOΚ`ζ
		Sprite::PreDraw(cmdList);

		//ΗjσXRA`ζ
		breakScore->Draw();

		//UIπΝ€g`ζ
		UIFrame->Draw();

		//§ΐΤ`ζ
		timeLimitGauge->Draw();

		//bV`ζ
		rush->Draw();

		Sprite::PostDraw();
	}
	//|[YV[
	else if (scene == SceneName::PauseScene)
	{
		//XvCgwΚ`ζ
		Sprite::PreDraw(cmdList);

		//wi`ζ
		backGround->Draw();
		//ΥgpQ[W`ζ
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//IuWFNg`ζ
		Object3d::PreDraw(cmdList);

		//Η`ζ
		wall->Draw();

		//vC[`ζ
		player->Draw();

		//Υg`ζ
		shockWave->Draw();


		//G`ζ
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();

		//XvCgOΚ`ζ
		Sprite::PreDraw(cmdList);

		//UIπΝ€g`ζ
		UIFrame->Draw();

		//§ΐΤ`ζ
		timeLimitGauge->Draw();

		//bV`ζ
		rush->Draw();

		//|[YV[UI`ζ
		pauseUI->Draw();

		//V[JΪpΓ]`ζ
		blackout->Draw();

		Sprite::PostDraw();
	}
	//FinishV[
	else if (scene == SceneName::FinishScene)
	{
		//XvCgwΚ`ζ
		Sprite::PreDraw(cmdList);

		//wi`ζ
		backGround->Draw();
		//ΥgpQ[W`ζ
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//IuWFNg`ζ
		Object3d::PreDraw(cmdList);

		//Η`ζ
		wall->Draw();

		//vC[`ζ
		player->Draw();

		//Υg`ζ
		shockWave->Draw();


		//G`ζ
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();

		//XvCgOΚ`ζ
		Sprite::PreDraw(cmdList);

		//UIπΝ€g`ζ
		UIFrame->Draw();

		//§ΐΤ`ζ
		timeLimitGauge->Draw();

		//bV`ζ
		rush->Draw();

		//Finish`ζ
		finish->Draw();

		Sprite::PostDraw();
	}
	//UgV[
	else if (scene == SceneName::ResultScene)
	{
		//XvCgwΚ`ζ
		Sprite::PreDraw(cmdList);

		//wi`ζ
		backGround->Draw();
		//ΥgpQ[W`ζ
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//IuWFNg`ζ
		Object3d::PreDraw(cmdList);

		//Η`ζ
		wall->Draw();

		//vC[`ζ
		player->Draw();

		//G`ζ
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();

		//XvCgOΚ`ζ
		Sprite::PreDraw(cmdList);

		//UIπΝ€g`ζ
		UIFrame->Draw();
		//§ΐΤ`ζ
		timeLimitGauge->Draw();

		//bV`ζ
		rush->Draw();

		//UgV[UI`ζ
		resultUI->Draw();

		//V[JΪpΓ]`ζ
		blackout->Draw();

		Sprite::PostDraw();
	}

	//SV[€ΚΜ`ζ
	{
		//GtFNgΜ`ζ
		effects->Draw(cmdList);
	}
}

void GameScene::ResetTitleScene()
{
	//vC[ϊ»
	player->ResetTitle();
	//Υgϊ»
	shockWave->Reset();

	//GͺcΑΔ’½ην
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//νtOπtrueΙ΅Δ­§ν
		(*itrEnemy)->SetDelete();
		if ((*itrEnemy)->GetIsDelete())
		{
			//vfπνAXg©ηO·ι
			safe_delete(*itrEnemy);
			itrEnemy = enemys.erase(itrEnemy);
			continue;
		}
		//forͺπρ·
		itrEnemy++;
	}
	//Gπ³Μ?«Ιί·
	BaseEnemy::SetIsResultMove(false);

	//Ηϊ»
	wall->Reset();

	//Q[ΰΎϊ»
	explanation->Reset();
	//UIt[ϊ»
	UIFrame->Reset();

	//ΗjσXRAϊ»
	breakScore->Reset();
	//bVϊ»
	rush->Reset();
	//§ΐΤϊ»
	timeLimitGauge->Reset();
	//ΥgpQ[Wϊ»
	shockWaveGauge->Reset();

	//ReadyGoϊ»
	readyGo->Reset();
	//Finishϊ»
	finish->Reset();
	//UgV[UIϊ»
	resultUI->Reset();

	//V[π^CgV[ΙΪs
	scene = SceneName::TitleStayScene;

	//ΗΜ½θ»θCπ^CgV[pΙί·
	WallLineSet(true);

	//ζΚVFCN΅Θ’
	isShake = false;
	//ζΚVFCNΤϊ»
	ShakeTime = 0;

	spawnInterval = 0;//X|[Τu
	spawnRate = 0;//X|[[g(κxΙX|[·ιGΜ)
	enemyType = 0;//GΜνή»Κp
	enemyDirection = 0;//GΜo»ϋό»Κp
	isRush = false;//bV©
	isRushStart = false;//bVJn©
	isRushEnd = false;//bVIΉ©
	spawnTimer = 80;
	rushTimer = 1800;//bVάΕΜJEg
}

void GameScene::ResetGame()
{
	//vC[ϊ»
	player->ResetGame();
	//Υgϊ»
	shockWave->Reset();

	//GͺcΑΔ’½ην
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//νtOπtrueΙ΅Δ­§ν
		(*itrEnemy)->SetDelete();
		if ((*itrEnemy)->GetIsDelete())
		{
			//vfπνAXg©ηO·ι
			safe_delete(*itrEnemy);
			itrEnemy = enemys.erase(itrEnemy);
			continue;
		}
		//forͺπρ·
		itrEnemy++;
	}
	//Gπ³Μ?«Ιί·
	BaseEnemy::SetIsResultMove(false);

	//Ηϊ»
	wall->Reset(false);

	//UIt[ϊ»
	UIFrame->Reset();

	//ΗjσXRAϊ»
	breakScore->Reset();
	//bVϊ»
	rush->Reset();
	//§ΐΤϊ»
	timeLimitGauge->Reset();
	//ΥgpQ[Wϊ»
	shockWaveGauge->SetGaugeReset();

	//ReadyGoϊ»
	readyGo->Reset();
	//Finishϊ»
	finish->Reset();
	//UgV[UIϊ»
	resultUI->Reset();

	//V[πReadyGoV[ΙΪs
	scene = SceneName::ReadyGoScene;

	//ζΚVFCN΅Θ’
	isShake = false;
	//ζΚVFCNΤϊ»
	ShakeTime = 0;

	spawnInterval = 0;//X|[Τu
	spawnRate = 0;//X|[[g(κxΙX|[·ιGΜ)
	enemyType = 0;//GΜνή»Κp
	enemyDirection = 0;//GΜo»ϋό»Κp
	isRush = false;//bV©
	isRushStart = false;//bVJn©
	isRushEnd = false;//bVIΉ©
	spawnTimer = 80;
	rushTimer = 1800;//bVάΕΜJEg
}

void GameScene::ShockWaveStart(XMFLOAT3 pos, int powerLevel)
{
	//vC[ͺ©RΙ?―Θ’i_[WσΤjΘη²―ι
	if (!player->GetIsFreeMove()) { return; }

	//­Λ³κΔ’½η²―ι
	if (shockWave->GetIsAlive()) { return; }

	//Υg­Λ
	shockWave->ShockWaveStart(pos, powerLevel);

	//ζΚVFCN
	isShake = true;

	//TEhΜΔΆ
	SoundManager(sound[3], false, false);
}

void GameScene::SpawnChaser()
{
	//Ά¬ΙϊΐWπίι
	XMFLOAT3 startPos = {};

	//ΝΝ
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
	XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

	//4p^[Μ_ΕϊΐWΖΪ?ϋόπZbg
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; }

	//Η]GπΆ¬
	enemys.push_back(Chaser::Create(startPos));
}

void GameScene::SpawnDivision()
{
	//Ά¬ΙϊΐWΖΪ?ϋόπίι
	XMFLOAT3 startPos = {};
	float angle = 0;

	//ΝΝ
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
	XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

	//4p^[Μ_ΕϊΐWΖΪ?ϋόπZbg
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; angle = 300; }

	//ͺτGπΆ¬
	enemys.push_back(Division::Create(startPos, angle));
}

void GameScene::SpawnReleaser()
{
	//Ά¬ΙϊΐWΖΪ?ϋόπίι
	XMFLOAT3 startPos = {};
	float angle = 0;

	//ΝΝ
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
	XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

	//4p^[Μ_ΕϊΐWΖΪ?ϋόπZbg
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; angle = 300; }

	//β~n_π_Ά¬
	XMFLOAT3 stayPos = {};
	stayPos.x = (float)(rand() % 200 - 100);
	stayPos.y = (float)(rand() % 120 - 60);

	//ϊoIπΆ¬
	enemys.push_back(Releaser::Create(startPos, stayPos));
}

void GameScene::SpawnEnemyToEnemy(BaseEnemy* enemy)
{
	Audio* audio = Audio::GetInstance();

	//GͺͺτIΎΑ½κ
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Division)
	{
		//Ά¬ΙϊΐWΖΪ?ϋόπίι
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = enemy->GetMoveDegree();

		//TEhΜΔΆ
		SoundManager(sound[11], false, false);

		//3ΜΆ¬·ι
		for (int i = 0; i < 3; i++)
		{
			angle += 120;

			//Η]GπΆ¬·ι
			enemys.push_back(Chaser::Create(startPos, angle, true));
		}
	}

	//GͺϊoIΎΑ½κ
	else if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Releaser)
	{
		//Ά¬ΙϊΐWΖΪ?ϋόπίι
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = (float)(rand() % 360);

		//TEhΜΔΆ
		SoundManager(sound[11], false, false);

		//Η]GπΆ¬·ι
		enemys.push_back(Chaser::Create(startPos, angle, true));
	}
}

void GameScene::CameraUpdate(Camera* camera)
{
	//VFCN
	if (isShake)
	{
		camera->StartCameraShake(3);
		ShakeTime++;

		//VFCNJn©ηκθΤoΑ½η
		if (ShakeTime > 20) {
			isShake = false;
			ShakeTime = 0;

			//VFCNIΉ
			camera->EndCameraShake();
		}
	}
	//J£πZbg
	camera->TpsCamera(cameraPos);
	//JXV
	camera->Update();
}

void GameScene::SpawnEnemyManager(int score, int time)
{
	Audio* audio = Audio::GetInstance();

	//RushΆαΘ’Ζ«
	if (!isRush)
	{
		//eνTimerXV
		if (6 <= score) { rushTimer++; }
		spawnTimer++;

		//RushTimerͺκθlΙB΅½η
		if (rushInterval <= rushTimer)
		{
			//TEhΜΔΆ
			SoundManager(sound[7], false, false);
			rushTimer = 0; isRush = true; isRushStart = true;
		}

		//BreakπΰΖΙIntervalXV
		if (score <= 5) { spawnInterval = 100; }
		else if (score <= 15) { spawnInterval = 80; }
		else if (score <= 22) { spawnInterval = 60; }
		else if (score <= 32) { spawnInterval = 70; }
		else if (score <= 42) { spawnInterval = 60; }
		else if (score <= 50) { spawnInterval = 70; }
		else if (score <= 70) { spawnInterval = 60; }
		else if (score <= 80) { spawnInterval = 100; }
		else if (score <= 90) { spawnInterval = 40; }
		else { spawnInterval = 60; }

		//BreakπΰΖΙRateXV
		spawnRate = 1;
	}

	//RushΜΖ«
	if (isRush)
	{
		//eνTimerXV
		rushTimer++;
		spawnTimer++;

		//RushTimerͺκθlΙB΅½η
		if (rushFinish <= rushTimer) { rushTimer = 0; isRush = false; isRushEnd = true; }

		//BreakπΰΖΙIntervalXV
		if (score <= 5) { spawnInterval = 50; }
		else if (score <= 15) { spawnInterval = 40; }
		else if (score <= 22) { spawnInterval = 30; }
		else if (score <= 32) { spawnInterval = 35; }
		else if (score <= 42) { spawnInterval = 30; }
		else if (score <= 50) { spawnInterval = 35; }
		else if (score <= 70) { spawnInterval = 30; }
		else if (score <= 80) { spawnInterval = 50; }
		else if (score <= 90) { spawnInterval = 20; }
		else { spawnInterval = 30; }

		//BreakπΰΖΙRateXV
		spawnRate = 2;
	}

	//TimerͺIntervalπ΄¦½ηGπΆ¬·ι
	if (spawnInterval <= spawnTimer)
	{
		//Timerπ0Ιί·
		spawnTimer = 0;

		//RateΜΆ¬·ι
		for (int i = 0; i < spawnRate; i++)
		{
			//Ά¬ΙϊΐWΖΪ?ϋόπθ
			XMFLOAT3 startPos = {};
			float angle = 0;

			//ΝΝ
			float range = 5.0f;
			XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
			XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

			//ΕGΜνήπθ
			int enemyTypeRand;
			if (score <= 8) { enemyTypeRand = rand() % 4; }
			else if (score <= 15) { enemyTypeRand = rand() % 5; }
			else if (score <= 22) { enemyTypeRand = rand() % 6; }
			else if (score <= 28) { enemyTypeRand = rand() % 7; }
			else if (score <= 32) { enemyTypeRand = (rand() % 6) + 1; }
			else if (score <= 37) { enemyTypeRand = rand() % 8; }
			else if (score <= 42) { enemyTypeRand = rand() % 9; }
			else if (score <= 46) { enemyTypeRand = rand() % 10; }
			else if (score <= 50) { enemyTypeRand = (rand() % 9) + 1; }
			else if (score <= 55) { enemyTypeRand = (rand() % 4) + 3; }
			else if (score <= 60) { enemyTypeRand = (rand() % 3) + 5; }
			else if (score <= 65) { enemyTypeRand = (rand() % 7) + 2; }
			else if (score <= 70) { enemyTypeRand = (rand() % 5) + 3; }
			else if (score <= 75) { enemyTypeRand = (rand() % 5) + 3; }
			else if (score <= 80) { enemyTypeRand = (rand() % 3) + 7; }
			else if (score <= 85) { enemyTypeRand = rand() % 10; }
			else if (score <= 90) { enemyTypeRand = (rand() % 8) + 2; }
			else { enemyTypeRand = rand() % 10; }

			if (enemyTypeRand <= 3) { enemyType = 0; }
			else if (enemyTypeRand <= 6) { enemyType = 1; }
			else { enemyType = 2; }

			//ΕGͺo»·ιϋόπθ
			int enemyDirectionRand = rand() % 4;
			if (enemyDirectionRand == 0) { enemyDirection = 0; }
			else if (enemyDirectionRand == 1) { enemyDirection = 1; }
			else if (enemyDirectionRand == 2) { enemyDirection = 2; }
			else { enemyDirection = 3; }

			//ϊΐWΖΪ?ϋόπθ
			if (enemyDirection == 0)
			{
				//γ€©ηo»
				//ϊΐWπθ
				startPos = { (float)(rand() % 300 - 150), startLineMax.y, 0 };

				//Ϊ?ϋόπθ(180}30)
				angle = (float)(rand() % 61) + 150;
			}
			else if (enemyDirection == 1)
			{
				//Ά€©ηo»
				//ϊΐWπθ
				startPos = { startLineMin.x, (float)(rand() % 140 - 70), 0 };

				//Ϊ?ϋόπθ(270}30)
				angle = (float)(rand() % 61) + 240;
			}
			else if (enemyDirection == 2)
			{
				//Ί€©ηo»
				//ϊΐWπθ
				startPos = { (float)(rand() % 300 - 150), startLineMin.y, 0 };

				//Ϊ?ϋόπθ(0}30)
				angle = (float)(rand() % 61) - 30;
				if (angle < 0)
				{
					angle += 360;
				}
			}
			else
			{
				//E€©ηo»
				//ϊΐWπθ
				startPos = { startLineMax.x, (float)(rand() % 140 - 70), 0 };

				//Ϊ?ϋόπθ(90}30)
				angle = (float)(rand() % 61) + 60;
			}

			//GΜΆ¬
			if (enemyType == 0)
			{
				//G1(Ηφ^)πΆ¬
				enemys.push_back(Chaser::Create(startPos));
			}
			else if (enemyType == 1)
			{
				//G2(gU^)πΆ¬
				enemys.push_back(Division::Create(startPos, angle));
			}
			else if (enemyType == 2)
			{
				//β~n_π_Ά¬
				XMFLOAT3 stayPos = {};
				stayPos.x = (float)(rand() % 200 - 100);
				stayPos.y = (float)(rand() % 120 - 60);

				//G3(ϊo^)πΆ¬
				enemys.push_back(Releaser::Create(startPos, stayPos));
			}
		}
	}
}

void GameScene::WallLineSet(const bool isTitle)
{
	//ΗΜΆ¬ΝΝ
	XMFLOAT2 minWallPosition = WallObject::GetWallMinPosition();
	XMFLOAT2 maxWallPosition = WallObject::GetWallMaxPosition();

	//^CgζΚΜΖ«
	if (isTitle)
	{
		minWallPosition.y += 8.0f;
		maxWallPosition.y += 12.0f;
	}
	//Q[ζΚΜΖ«
	else
	{
		maxWallPosition.y += 4.0f;
	}
	//ΗΆ¬ΝΝΖΜ·ͺ
	const float wallPosDis = 18.0f;
	minWallLinePosition = { minWallPosition.x + wallPosDis, minWallPosition.y + wallPosDis };
	maxWallLinePosition = { maxWallPosition.x - wallPosDis, maxWallPosition.y - wallPosDis };
	//ΝΝOΝΝ
	const XMFLOAT2 outsideRange = { 25.0f, 45.0f };
	outsideMinPosition = { minWallLinePosition.x - outsideRange.x, minWallLinePosition.y - outsideRange.y };
	outsideMaxPosition = { maxWallLinePosition.x + outsideRange.x, maxWallLinePosition.y + outsideRange.y };

	//ΗθΔ
	Player::SetMoveRange(minWallLinePosition, maxWallLinePosition);
	XMFLOAT2 enemyWallLineMin = minWallLinePosition;
	enemyWallLineMin.y -= 2;
	XMFLOAT2 enemyWallLineMax = maxWallLinePosition;
	enemyWallLineMax.y += 2;
	BaseEnemy::SetWallLine(enemyWallLineMin, enemyWallLineMax);
}

bool GameScene::CheckPlayerToHealingZone(Player* player)
{
	//ΥΛpΙΐWΖΌaΜε«³πΨθι
	XMFLOAT3 zonePos = { 1, -11, 0 };
	float zoneSize = 30.0f;
	XMFLOAT3 playerPos = player->GetPosition();
	float playerSize = player->GetScale().x;

	//ΥΛ»θπvZ
	bool isCollision = Collision::CheckCircle2Circle(
		zonePos, zoneSize, playerPos, playerSize);

	return isCollision;
}

void GameScene::SoundManager(int soundNum, bool isBGM, bool stop)
{
	Audio* audio = Audio::GetInstance();

	//SEΜκ
	if (!isBGM)
	{
		//ΔΆ
		if (!stop)
		{
			audio->SoundPlayWava(soundNum, false);
		}
		//β~
		else
		{
			audio->StopSound(soundNum);
		}
	}
	//BGMΜκ
	else
	{
		//ΔΆ
		if (!stop)
		{
			audio->SoundPlayWava(soundNum, true);
		}
		//β~
		else
		{
			audio->StopSound(soundNum);
		}
	}
}