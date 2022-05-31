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

const float radian = XM_PI / 180.0f;//���W�A��

GameScene::~GameScene()
{
	//new�I�u�W�F�N�g�̔j��
	//safe_delete(audio);
	safe_delete(light);

	//���f�����
	safe_delete(titleLogoModel);
	safe_delete(playerModel);
	safe_delete(waveModel);
	safe_delete(healingZoneModel);
	safe_delete(XButtonModel);
	safe_delete(chaserModel);
	safe_delete(divisionModel);
	safe_delete(releaserModel);


	//�v���C���[���
	safe_delete(player);
	//�Ռ��g���
	safe_delete(shockWave);

	//�G���
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		safe_delete(*itrEnemy);
	}
	//�G�̃��X�g���
	enemys.clear();

	//�ǉ��
	safe_delete(wall);

	//�G�t�F�N�g�̉��
	safe_delete(effects);

	//�w�i�̉��
	safe_delete(backGround);

	//�V�[���J�ڗp�Ó]���
	safe_delete(blackout);

	//�Q�[���������
	safe_delete(explanation);

	//UI���͂��g���
	safe_delete(UIFrame);

	//�ǔj��X�R�A���
	safe_delete(breakScore);
	//���b�V�����
	safe_delete(rush);
	//�������ԉ��
	safe_delete(timeLimitGauge);
	//�Ռ��g�p�Q�[�W���
	safe_delete(shockWaveGauge);
	//ReadyGo���
	safe_delete(readyGo);
	//�|�[�Y�V�[��UI���
	safe_delete(pauseUI);
	//Finish���
	safe_delete(finish);
	//���U���g�V�[��UI���
	safe_delete(resultUI);
}

void GameScene::Initialize(Camera* camera)
{
	// nullptr�`�F�b�N
	assert(camera);

	//���C�g
	light = LightGroup::Create();
	light->DefaultLightSetting();
	Object3d::SetLightGroup(light);

	titleLogoModel = Model::CreateFromOBJ("titleLogo");//�^�C�g�����S�̃��f��
	playerModel = Model::CreateFromOBJ("player");//�v���C���[�̃��f��
	waveModel = Model::CreateFromOBJ("wave");//�Ռ��g�̃��f��
	healingZoneModel = Model::CreateFromOBJ("wavehealcore");//�Q�[�W�񕜒n�_�̃��f��
	XButtonModel = Model::CreateFromOBJ("Xbutton");//X�{�^���̃��f��
	chaserModel = Model::CreateFromOBJ("enemy1_4");//�ǐՓG�̃��f��
	divisionModel = Model::CreateFromOBJ("enemy2_3");//�����G�̃��f��
	releaserModel = Model::CreateFromOBJ("enemy3_2");//���o�G�̃��f��


	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
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

	//�f�o�b�O�e�L�X�g����
	DebugText::GetInstance()->Initialize(0);


	//�v���C���[����
	player = Player::Create(playerModel);
	//�Ռ��g����
	shockWave = ShockWave::Create(waveModel);

	//�G�̃��f�����Z�b�g
	Chaser::SetModel(chaserModel);
	Division::SetModel(divisionModel);
	Releaser::SetModel(releaserModel);
	TitleLogo::SetModel(titleLogoModel);

	//�ǐ���
	wall = WallManager::Create();
	//�ǂ̓����蔻�胉�C�����^�C�g���V�[���p�ɐݒ�
	WallLineSet(true);

	//�G�t�F�N�g������
	effects = new StageEffect();
	effects->Initialize();

	//�w�i����
	backGround = BackGround::Create(16);

	//�V�[���J�ڗp�Ó]����
	blackout = Blackout::Create(1);

	//�Q�[����������
	explanation = Explanation::Create(20, 22);

	//UI���͂��g����
	UIFrame = UIFrame::Create(17, 18);


	//�ǔj��X�R�A����
	breakScore = BreakScore::Create(19, 2);
	//���b�V������
	rush = Rush::Create(23);
	//�������Ԑ���
	timeLimitGauge = TimeLimitGauge::Create(13, 14, 15);
	//�Ռ��g�p�Q�[�W����
	shockWaveGauge = ShockWaveGauge::Create(5);

	//ReadyGo����
	readyGo = ReadyGo::Create(7, 8);
	//�|�[�Y�V�[��UI����
	pauseUI = PauseUI::Create(1, 4, 3, 12, 21);
	//Finish����
	finish = Finish::Create(9);
	//���U���g�V�[��UI����
	resultUI = ResultUI::Create(1, 10, 19, 2, 11, 12, 21);

	//�T�E���h�̓ǂݍ���
	Audio* audio = Audio::GetInstance();
	sound[0] = audio->SoundLoadWave("Resources/sound/select.wav");//�J�[�\���ړ���
	sound[1] = audio->SoundLoadWave("Resources/sound/enter.wav");//�I�������艹
	sound[2] = audio->SoundLoadWave("Resources/sound/playerHit.wav");//�v���C���[��e��
	sound[3] = audio->SoundLoadWave("Resources/sound/shockWave.wav");//�v���C���[�Ռ��g��
	sound[4] = audio->SoundLoadWave("Resources/sound/enemyHit.wav");//�G�l�~�[������щ�
	sound[5] = audio->SoundLoadWave("Resources/sound/enemyDead.wav");//�G�l�~�[�ǏՓˉ�
	sound[6] = audio->SoundLoadWave("Resources/sound/wallBreak.wav");//�Ǒϋv�l0��
	sound[7] = audio->SoundLoadWave("Resources/sound/bigShock.wav");//����Ռ��g��
	sound[8] = audio->SoundLoadWave("Resources/sound/go.wav");//GO��
	sound[9] = audio->SoundLoadWave("Resources/sound/finish.wav");//FINISH��
	sound[10] = audio->SoundLoadWave("Resources/sound/wallSet.wav");//�ǐ�����
	sound[11] = audio->SoundLoadWave("Resources/sound/diffusion.wav");//�g�U��
	sound[12] = audio->SoundLoadWave("Resources/sound/charge.wav");//�K�E�Z���܂����Ƃ��̉�
	sound[13] = audio->SoundLoadWave("Resources/sound/result.wav");//���U���g�̃��S�\���̉�
	sound[14] = audio->SoundLoadWave("Resources/sound/chase.wav");//�ǔ�����G�̈ړ���
	sound[15] = audio->SoundLoadWave("Resources/sound/enemyHit2.wav");//�G�l�~�[��������щ�
	sound[16] = audio->SoundLoadWave("Resources/sound/bgm.wav");//BGM
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();
	Audio* audio = Audio::GetInstance();

	//�^�C�g������
	if (scene == SceneName::TitleStayScene)
	{
		//�Ó]������
		if (blackout->GetIsBlackReturn())
		{
			//�V�[���J�ڗp�Ó]�X�V
			blackout->Update();
		}
		else
		{
			//���o���I�������玟�ɍs��
			if (wall->GetIsAlive())
			{
				scene = SceneName::TitleScene;

				//�v���C���[���X�|�[����ԂɃZ�b�g
				player->SetSpawn();
			}

			//�ǍX�V
			wall->Update();
		}
	}

	//�^�C�g���V�[��
	else if (scene == SceneName::TitleScene)
	{
		//�v���C���[�X�V
		player->Update();

		//�X�|�[�����I��������
		if (player->GetTriggerSpawnEnd())
		{
			//�^�C�g�����S����
			enemys.push_back(TitleLogo::Create());
		}

		//�Ռ��g����
		if (player->GetIsShockWaveStart())
		{
			ShockWaveStart(player->GetPosition(), shockWaveGauge->GetGaugeLevel());
		}
		//�Ռ��g�X�V
		shockWave->Update();

		//�G�X�V
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//�X�V����
			(*itrEnemy)->Update();

			//�G�������Ă��Ȃ���Δ�΂�
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//�ǂƓG�̓����蔻������
			if (GameCollision::CheckWallToEnemy(wall, (*itrEnemy)))
			{
				//�T�E���h�̍Đ�
				SoundManager(sound[5], false, false);
				//�U���I��
				Xinput->StartVibration(XInputManager::STRENGTH::MEDIUM, 10);
			}

			//�Ռ��g�ƓG�̓����蔻��
			if (GameCollision::CheckShockWaveToEnemy(shockWave, (*itrEnemy)))
			{
				//�T�E���h�̍Đ�
				SoundManager(sound[4], false, false);
			}
		}
		//�^�C�g�����S�̃X�|�[��������������
		if (TitleLogo::GetTriggerSpawnEnd())
		{
			//�v���C���[����\
			player->SetIsFreeMove(true);
			//�Q�[����������ʓ��Ɉړ�������
			explanation->SetMoveInScreen();
			//�Ռ��g�Q�[�W�̍X�V���J�n
			shockWaveGauge->SetIsUpdate(true);
			//�^�C�g���php�ɐ؂�ւ�
			wall->SetBreakCount(3);
		}

		//�ǍX�V
		wall->Update();
		//�ǂ���ꂽ�玟�̃V�[����
		if (wall->GetTriggerBreak())
		{
			scene = SceneName::ReadyGoScene;

			//�T�E���h�̍Đ�
			SoundManager(sound[5], false, false);

			//�v���C���[���Q�[���J�n���̍��W�Ɉړ���ԂɃZ�b�g
			player->SetGameStartPos();
			//�Q�[����������ʊO�Ɉړ�
			explanation->SetMoveOutScreen();
			//�Ռ��g�Q�[�W�����Z�b�g����
			shockWaveGauge->SetGaugeReset();

			//�^�C�g�����S���폜
			for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
			{
				(*itrEnemy)->SetDelete();
			}
		}

		//�Q�[�������X�V
		explanation->Update();

		//�Ռ��g�p�Q�[�W�X�V
		if (CheckPlayerToHealingZone(player))
		{
			shockWaveGauge->IncreasePoint();
			//�񕜃G�t�F�N�g
			StageEffect::SetHeal(player->GetPosition());
		}
		else
		{
			shockWaveGauge->DecreasePoint();
		}
		shockWaveGauge->Update();
	}

	//ReadyGo�V�[��
	else if (scene == SceneName::ReadyGoScene)
	{
		//�v���C���[�X�V
		player->Update();
		//�Ռ��g�X�V
		shockWave->Update();

		//�v���C���[���Q�[���J�n���̍��W�ɓ��B������
		if (player->GetTriggerMoveStartPosEnd())
		{
			//UI����ʏ㕔�Ɉړ�������
			UIFrame->SetMoveGamePos();
			timeLimitGauge->SetMoveGamePos();
		}
		//UI�X�V
		UIFrame->Update();
		timeLimitGauge->Update();
		shockWaveGauge->Update();

		//�Q�[�������X�V
		explanation->Update();

		//�ǂ̍X�V
		wall->Update();

		//�ǂ������Ă�����
		if (wall->GetIsAlive())
		{
			//ReadyGo�X�v���C�g�X�V
			readyGo->Update();
			//ReadyGo��Go�܂ŕ\�����I������玟�̃V�[����
			if (readyGo->GetIsReadyGoEnd())
			{
				//�^�C�g���php�ɐ؂�ւ�
				wall->SetBreakCount(0);

				scene = SceneName::GamePlayScene;

				//�ǂ̓����蔻�胉�C�����Q�[���V�[���p�ɐݒ�
				WallLineSet(false);

				//�v���C���[�̍s������������
				player->SetIsFreeMove(true);
				//�������Ԃ̃J�E���g�_�E�����J�n����
				timeLimitGauge->SetIsCountDown(true);
				//�X�^�[�g�{�^����\������
				UIFrame->SetIsDrawStart(true);
			}
		}
	}

	//�Q�[���V�[��
	else if (scene == SceneName::GamePlayScene)
	{
		//BGM�̍Đ�
		if (!isBGM)
		{
			//�T�E���h�̍Đ�
			SoundManager(sound[16], true, false);
		}

		//�v���C���[�X�V
		player->Update();
		//�Ռ��g����
		if (player->GetIsShockWaveStart())
		{
			ShockWaveStart(player->GetPosition(), shockWaveGauge->GetGaugeLevel());
		}
		//�Ռ��g�X�V
		shockWave->Update();

		//�ǂ�����ꍇ
		if (wall->GetIsAlive())
		{
			//�G����
			SpawnEnemyManager(breakScore->GetBreakScore(), timeLimitGauge->GetTimer());
		}

		//�G�X�V
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//�X�V����
			(*itrEnemy)->Update();

			//�G�������Ă��Ȃ���Δ�΂�
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//�G�����t���O��true�̂Ƃ��G�𐶐�����
			if ((*itrEnemy)->GetIsCreateEnemy())
			{
				SpawnEnemyToEnemy(*itrEnemy);
			}

			//�ǂƓG�̓����蔻������
			if (GameCollision::CheckWallToEnemy(wall, (*itrEnemy)))
			{
				//�X�|�[���^�C�}�[���ő�܂ŏグ��
				spawnTimer = 300;
				//�T�E���h�̍Đ�
				SoundManager(sound[5], false, false);
				//�U���I��
				Xinput->StartVibration(XInputManager::STRENGTH::MEDIUM, 10);
			}
			//�͈͊O����
			XMFLOAT3 pos = (*itrEnemy)->GetPosition();
			if (pos.x <= outsideMinPosition.x || pos.x >= outsideMaxPosition.x ||
				pos.y <= outsideMinPosition.y || pos.y >= outsideMaxPosition.y)
			{
				(*itrEnemy)->SetDelete();
			}

			//�v���C���[�ƓG�̓����蔻��
			if (GameCollision::CheckPlayerToEnemy(player, (*itrEnemy)))
			{
				//�T�E���h�̍Đ�
				SoundManager(sound[2], false, false);
				//��ʃV�F�C�N
				isShake = true;
				//�U���I��
				Xinput->StartVibration(XInputManager::STRENGTH::SMALL, 10);
			}

			//�Ռ��g�ƓG�̓����蔻��
			if (GameCollision::CheckShockWaveToEnemy(shockWave, (*itrEnemy)))
			{
				//�T�E���h�̍Đ�
				SoundManager(sound[4], false, false);
			}
		}

		//�ǍX�V
		wall->Update();
		//�ǂ���ꂽ�u��
		if (wall->GetTriggerBreak())
		{
			//�G���폜
			for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
			{
				(*itrEnemy)->SetDelete();

				//�G�������鎞�̃G�t�F�N�g
				StageEffect::SetDeleteEnemey((*itrEnemy)->GetPosition());
			}

			//�󂵂��X�R�A���Z
			breakScore->AddScore();
			//�^�C�����~�b�g���L�т�
			timeLimitGauge->Recovery(7);
		}

		//UI�t���[���X�V
		UIFrame->Update();

		//�ǔj��X�R�A�X�V
		breakScore->Update();
		//�������ԍX�V
		timeLimitGauge->Update();
		//�������Ԃ�0�ɂȂ����玟�̃V�[����
		if (timeLimitGauge->GetIsCountDownEnd())
		{
			scene = SceneName::FinishScene;

			//�G���폜
			for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
			{
				(*itrEnemy)->SetDelete();

				//�G�������鎞�̃G�t�F�N�g
				StageEffect::SetDeleteEnemey((*itrEnemy)->GetPosition());
			}

			//�T�E���h�̒�~
			isBGM = false;
			SoundManager(sound[16], true, true);

			//�v���C���[�����R�ɑ���ł��Ȃ�����
			player->SetIsFreeMove(false);

			//���U���g�V�[���p�ɕǔj�󐔂�n���Ă���
			resultUI->SetBreakWallNum(breakScore->GetBreakScore());
		}

		//�Ռ��g�p�Q�[�W�X�V
		if (CheckPlayerToHealingZone(player))
		{
			shockWaveGauge->IncreasePoint();
			//�񕜃G�t�F�N�g
			StageEffect::SetHeal(player->GetPosition());
		}
		else
		{
			shockWaveGauge->DecreasePoint();
		}
		shockWaveGauge->Update();

		//���b�V���X�V
		if (isRushStart)
		{
			rush->SetMoveInScreen();

			//�g���K�[�Ȃ̂Ńt���O���~�낵�Ă���
			isRushStart = false;
		}
		else if (isRushEnd)
		{
			rush->SetMoveOutScreen();

			//�g���K�[�Ȃ̂Ńt���O���~�낵�Ă���
			isRushEnd = false;
		}
		rush->Update();

		//�w�肵���{�^���������ƃ|�[�Y�V�[����
		if (input->TriggerKey(DIK_1) || Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_START))
		{
			//�T�E���h�̍Đ�
			SoundManager(sound[1], false, false);

			scene = SceneName::PauseScene;

			//�|�[�Y�V�[�������Z�b�g
			pauseUI->Reset();

			//�X�^�[�g�{�^����\�����Ȃ�
			UIFrame->SetIsDrawStart(false);
		}
	}

	//�|�[�Y�V�[��
	else if (scene == SceneName::PauseScene)
	{
		//UI�t���[���X�V
		UIFrame->Update();

		//�|�[�Y�V�[��UI�X�V
		pauseUI->Update();
		//�m�肵�Ă��Ȃ��Ƃ��A�w�肵���{�^���������ƃV�[���؂�ւ��I�����m�肳����
		if (!pauseUI->GetIsSelect() && (input->TriggerKey(DIK_1) ||
			Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_A)))
		{
			//�T�E���h�̍Đ�
			SoundManager(sound[1], false, false);

			//�m�肳����
			pauseUI->SetSelect();

			//�Q�[���ɖ߂�
			if (pauseUI->GetIsBackGame())
			{
				scene = SceneName::GamePlayScene;

				//�X�^�[�g�{�^����\������
				UIFrame->SetIsDrawStart(true);
			}
			//�^�C�g���V�[���ɖ߂�
			else
			{
				//�T�E���h�̒�~
				isBGM = false;
				SoundManager(sound[16], true, true);

				//�Ó]�J�n
				blackout->SetBlackout();
			}
		}

		//�V�[���J�ڗp�Ó]�X�V
		blackout->Update();
		//�Ó]�ŉ�ʂ��^���ÂɂȂ�����^�C�g���V�[���ɖ߂�
		if (blackout->GetIsAllBlack())
		{
			//�^�C�g���V�[���ڍs�p������
			ResetTitleScene();

			//�Ó]��������ԂɃZ�b�g
			blackout->SetBlackoutReturn();
		}
	}

	//Finish�V�[��
	else if (scene == SceneName::FinishScene)
	{
		//�v���C���[�X�V
		player->Update();
		//�Ռ��g�X�V
		shockWave->Update();

		//�G�X�V
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//�X�V����
			(*itrEnemy)->Update();

			//�G�������Ă��Ȃ���Δ�΂�
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//�G�����t���O��true�̂Ƃ��G�𐶐�����
			if ((*itrEnemy)->GetIsCreateEnemy())
			{
				SpawnEnemyToEnemy(*itrEnemy);
			}

			//�ǂƓG�̓����蔻������
			GameCollision::CheckWallToEnemy(wall, (*itrEnemy));
		}

		//�ǍX�V
		wall->Update();
		//UI�X�V
		UIFrame->Update();
		timeLimitGauge->Update();
		shockWaveGauge->Update();
		rush->Update();

		//Finish�X�V
		finish->Update();
		//Finish��\�����I�����玟�̃V�[����
		if (finish->GetIsFinishSpriteMoveEnd())
		{
			scene = SceneName::ResultScene;

			//�G�����U���g�V�[���p�̓����ɕς���
			BaseEnemy::SetIsResultMove(true);

			//�ǂ����U���g�V�[���p�ɉ�ʊO�ɓ�����
			wall->SetOutScreen();

			//UI����ʊO�Ɉړ�������
			UIFrame->SetMoveResultPos();
			timeLimitGauge->SetMoveResultPos();

			//���b�V������ʓ��ɂ���ꍇ��ʊO�Ɉړ�������
			if (rush->GetIsInScreen())
			{
				rush->SetMoveOutScreen();
			}
		}
	}

	//���U���g�V�[��
	else if (scene == SceneName::ResultScene)
	{
		//�v���C���[�X�V
		player->Update();
		//�G�X�V
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//�X�V����
			(*itrEnemy)->Update();

			//�G�������Ă��Ȃ���Δ�΂�
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//�ǂ��Ȃ��̂ŁA��ʊO�ɏo����폜����
			XMFLOAT3 pos = (*itrEnemy)->GetPosition();
			//�͈͊O����
			if (pos.x <= outsideMinPosition.x || pos.x >= outsideMaxPosition.x ||
				pos.y <= outsideMinPosition.y || pos.y >= outsideMaxPosition.y)
			{
				(*itrEnemy)->SetDelete();
			}
		}

		//�ǍX�V
		wall->Update();
		//UI�X�V
		UIFrame->Update();
		timeLimitGauge->Update();
		shockWaveGauge->Update();
		rush->Update();

		//���U���g�V�[��UI�X�V
		resultUI->Update();
		//�S�Ă̕`�悪�I�������
		if (resultUI->GetIsDrawAll())
		{
			//�m�肵�Ă��Ȃ��Ƃ��A�w�肵���{�^���������ƃV�[���؂�ւ��I�����m�肳����
			if (!resultUI->GetIsSelect() && (input->TriggerKey(DIK_Z) ||
				Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_A)))
			{
				//�T�E���h�̍Đ�
				SoundManager(sound[1], false, false);

				//�m�肳����
				resultUI->SetSelect();

				//���g���C
				if (resultUI->GetIsRetry())
				{
					ResetGame();
				}
				//�^�C�g���V�[���ɖ߂�
				else
				{
					//�Ó]�J�n
					blackout->SetBlackout();
				}
			}
		}
		//�V�[���J�ڗp�Ó]�X�V
		blackout->Update();
		//�Ó]�ŉ�ʂ��^���ÂɂȂ�����^�C�g���V�[���ɖ߂�
		if (blackout->GetIsAllBlack())
		{
			//�^�C�g���V�[���ڍs�p������
			ResetTitleScene();

			//�Ó]��������ԂɃZ�b�g
			blackout->SetBlackoutReturn();
		}
	}

	//�G�폜
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//�폜�t���O��true�Ȃ�폜
		if ((*itrEnemy)->GetIsDelete())
		{
			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrEnemy);
			itrEnemy = enemys.erase(itrEnemy);
			continue;
		}
		//for������
		itrEnemy++;
	}

	//�G�t�F�N�g�̍X�V
	effects->Update(camera);
	//�w�i�X�V
	backGround->Update();
	//UI���͂��g�X�V
	UIFrame->Update();
	//�J�����X�V
	CameraUpdate(camera);

	//�f�o�b�O�e�L�X�g
	std::string wallHP = std::to_string(wall->GetHP());
	DebugText::GetInstance()->Print("HP : " + wallHP, 200, 200);

	input = nullptr;
}

void GameScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//�^�C�g���V�[��
	if (scene == SceneName::TitleStayScene || scene == SceneName::TitleScene)
	{
		//�X�v���C�g�w�ʕ`��
		Sprite::PreDraw(cmdList);

		//�w�i�`��
		backGround->Draw();
		//�Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�Ǖ`��
		wall->Draw();

		//�v���C���[�`��
		player->Draw();

		//�Ռ��g�`��
		shockWave->Draw();

		//�G�`��
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();

		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//UI���͂��g�`��
		UIFrame->Draw();

		//�Q�[�������`��
		explanation->Draw();

		//�V�[���J�ڗp�Ó]�`��
		blackout->Draw();

		Sprite::PostDraw();
	}
	//ReadyGo�V�[��
	else if (scene == SceneName::ReadyGoScene)
	{
		//�X�v���C�g�w�ʕ`��
		Sprite::PreDraw(cmdList);

		//�w�i�`��
		backGround->Draw();
		//�Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�Ǖ`��
		wall->Draw();

		//�v���C���[�`��
		player->Draw();

		Object3d::PostDraw();

		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//UI���͂��g�`��
		UIFrame->Draw();

		//ReadyGo�`��
		readyGo->Draw();
		//�������ԕ`��
		timeLimitGauge->Draw();

		//�Q�[�������`��
		explanation->Draw();

		Sprite::PostDraw();
	}
	//�Q�[���V�[��
	else if (scene == SceneName::GamePlayScene)
	{
		//�X�v���C�g�w�ʕ`��
		Sprite::PreDraw(cmdList);

		//�w�i�`��
		backGround->Draw();
		//�Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�Ǖ`��
		wall->Draw();

		//�v���C���[�`��
		player->Draw();

		//�Ռ��g�`��
		shockWave->Draw();


		//�G�`��
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();


		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//�ǔj��X�R�A�`��
		breakScore->Draw();

		//UI���͂��g�`��
		UIFrame->Draw();

		//�������ԕ`��
		timeLimitGauge->Draw();

		//���b�V���`��
		rush->Draw();

		Sprite::PostDraw();
	}
	//�|�[�Y�V�[��
	else if (scene == SceneName::PauseScene)
	{
		//�X�v���C�g�w�ʕ`��
		Sprite::PreDraw(cmdList);

		//�w�i�`��
		backGround->Draw();
		//�Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�Ǖ`��
		wall->Draw();

		//�v���C���[�`��
		player->Draw();

		//�Ռ��g�`��
		shockWave->Draw();


		//�G�`��
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();

		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//UI���͂��g�`��
		UIFrame->Draw();

		//�������ԕ`��
		timeLimitGauge->Draw();

		//���b�V���`��
		rush->Draw();

		//�|�[�Y�V�[��UI�`��
		pauseUI->Draw();

		//�V�[���J�ڗp�Ó]�`��
		blackout->Draw();

		Sprite::PostDraw();
	}
	//Finish�V�[��
	else if (scene == SceneName::FinishScene)
	{
		//�X�v���C�g�w�ʕ`��
		Sprite::PreDraw(cmdList);

		//�w�i�`��
		backGround->Draw();
		//�Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�Ǖ`��
		wall->Draw();

		//�v���C���[�`��
		player->Draw();

		//�Ռ��g�`��
		shockWave->Draw();


		//�G�`��
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();

		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//UI���͂��g�`��
		UIFrame->Draw();

		//�������ԕ`��
		timeLimitGauge->Draw();

		//���b�V���`��
		rush->Draw();

		//Finish�`��
		finish->Draw();

		Sprite::PostDraw();
	}
	//���U���g�V�[��
	else if (scene == SceneName::ResultScene)
	{
		//�X�v���C�g�w�ʕ`��
		Sprite::PreDraw(cmdList);

		//�w�i�`��
		backGround->Draw();
		//�Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

		Sprite::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�Ǖ`��
		wall->Draw();

		//�v���C���[�`��
		player->Draw();

		//�G�`��
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
		}

		Object3d::PostDraw();

		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//UI���͂��g�`��
		UIFrame->Draw();
		//�������ԕ`��
		timeLimitGauge->Draw();

		//���b�V���`��
		rush->Draw();

		//���U���g�V�[��UI�`��
		resultUI->Draw();

		//�V�[���J�ڗp�Ó]�`��
		blackout->Draw();

		Sprite::PostDraw();
	}

	//�S�V�[�����ʂ̕`��
	{
		//�G�t�F�N�g�̕`��
		effects->Draw(cmdList);

		//�f�o�b�O�e�L�X�g�`��
		Sprite::PreDraw(cmdList);
		
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
}

void GameScene::ResetTitleScene()
{
	//�v���C���[������
	player->ResetTitle();
	//�Ռ��g������
	shockWave->Reset();

	//�G���c���Ă�����폜
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//�폜�t���O��true�ɂ��ċ����폜
		(*itrEnemy)->SetDelete();
		if ((*itrEnemy)->GetIsDelete())
		{
			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrEnemy);
			itrEnemy = enemys.erase(itrEnemy);
			continue;
		}
		//for������
		itrEnemy++;
	}
	//�G�����̓����ɖ߂�
	BaseEnemy::SetIsResultMove(false);

	//�Ǐ�����
	wall->Reset();

	//�Q�[������������
	explanation->Reset();
	//UI�t���[��������
	UIFrame->Reset();

	//�ǔj��X�R�A������
	breakScore->Reset();
	//���b�V��������
	rush->Reset();
	//�������ԏ�����
	timeLimitGauge->Reset();
	//�Ռ��g�p�Q�[�W������
	shockWaveGauge->Reset();

	//ReadyGo������
	readyGo->Reset();
	//Finish������
	finish->Reset();
	//���U���g�V�[��UI������
	resultUI->Reset();

	//�V�[�����^�C�g���V�[���Ɉڍs
	scene = SceneName::TitleStayScene;

	//�ǂ̓����蔻�胉�C�����^�C�g���V�[���p�ɖ߂�
	WallLineSet(true);

	//��ʃV�F�C�N���Ȃ�
	isShake = false;
	//��ʃV�F�C�N���ԏ�����
	ShakeTime = 0;

	spawnInterval = 0;//�X�|�[���Ԋu
	spawnRate = 0;//�X�|�[�����[�g(��x�ɃX�|�[������G�̐�)
	enemyType = 0;//�G�̎�ޔ��ʗp
	enemyDirection = 0;//�G�̏o���������ʗp
	isRush = false;//���b�V������
	isRushStart = false;//���b�V���J�n��
	isRushEnd = false;//���b�V���I����
	spawnTimer = 80;
	rushTimer = 1800;//���b�V���܂ł̃J�E���g
}

void GameScene::ResetGame()
{
	//�v���C���[������
	player->ResetGame();
	//�Ռ��g������
	shockWave->Reset();

	//�G���c���Ă�����폜
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//�폜�t���O��true�ɂ��ċ����폜
		(*itrEnemy)->SetDelete();
		if ((*itrEnemy)->GetIsDelete())
		{
			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrEnemy);
			itrEnemy = enemys.erase(itrEnemy);
			continue;
		}
		//for������
		itrEnemy++;
	}
	//�G�����̓����ɖ߂�
	BaseEnemy::SetIsResultMove(false);

	//�Ǐ�����
	wall->Reset(false);

	//UI�t���[��������
	UIFrame->Reset();

	//�ǔj��X�R�A������
	breakScore->Reset();
	//���b�V��������
	rush->Reset();
	//�������ԏ�����
	timeLimitGauge->Reset();
	//�Ռ��g�p�Q�[�W������
	shockWaveGauge->SetGaugeReset();

	//ReadyGo������
	readyGo->Reset();
	//Finish������
	finish->Reset();
	//���U���g�V�[��UI������
	resultUI->Reset();

	//�V�[����ReadyGo�V�[���Ɉڍs
	scene = SceneName::ReadyGoScene;

	//��ʃV�F�C�N���Ȃ�
	isShake = false;
	//��ʃV�F�C�N���ԏ�����
	ShakeTime = 0;

	spawnInterval = 0;//�X�|�[���Ԋu
	spawnRate = 0;//�X�|�[�����[�g(��x�ɃX�|�[������G�̐�)
	enemyType = 0;//�G�̎�ޔ��ʗp
	enemyDirection = 0;//�G�̏o���������ʗp
	isRush = false;//���b�V������
	isRushStart = false;//���b�V���J�n��
	isRushEnd = false;//���b�V���I����
	spawnTimer = 80;
	rushTimer = 1800;//���b�V���܂ł̃J�E���g
}

void GameScene::ShockWaveStart(XMFLOAT3 pos, int powerLevel)
{
	//�v���C���[�����R�ɓ����Ȃ��i�_���[�W��ԁj�Ȃ甲����
	if (!player->GetIsFreeMove()) { return; }

	//���˂���Ă����甲����
	if (shockWave->GetIsAlive()) { return; }

	//�Ռ��g����
	shockWave->ShockWaveStart(pos, powerLevel);

	//��ʃV�F�C�N
	isShake = true;

	//�T�E���h�̍Đ�
	SoundManager(sound[3], false, false);
}

void GameScene::SpawnChaser()
{
	//�������ɏ������W�����߂�
	XMFLOAT3 startPos = {};

	//�͈�
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
	XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

	//4�p�^�[���̃����_���ŏ������W�ƈړ��������Z�b�g
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; }

	//�Ǐ]�G�𐶐�
	enemys.push_back(Chaser::Create(startPos));
}

void GameScene::SpawnDivision()
{
	//�������ɏ������W�ƈړ����������߂�
	XMFLOAT3 startPos = {};
	float angle = 0;

	//�͈�
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
	XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

	//4�p�^�[���̃����_���ŏ������W�ƈړ��������Z�b�g
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; angle = 300; }

	//�����G�𐶐�
	enemys.push_back(Division::Create(startPos, angle));
}

void GameScene::SpawnReleaser()
{
	//�������ɏ������W�ƈړ����������߂�
	XMFLOAT3 startPos = {};
	float angle = 0;

	//�͈�
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
	XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

	//4�p�^�[���̃����_���ŏ������W�ƈړ��������Z�b�g
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; angle = 300; }

	//��~�n�_�������_������
	XMFLOAT3 stayPos = {};
	stayPos.x = (float)(rand() % 200 - 100);
	stayPos.y = (float)(rand() % 120 - 60);

	//���o�I�𐶐�
	enemys.push_back(Releaser::Create(startPos, stayPos));
}

void GameScene::SpawnEnemyToEnemy(BaseEnemy* enemy)
{
	Audio* audio = Audio::GetInstance();

	//�G�������I�������ꍇ
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Division)
	{
		//�������ɏ������W�ƈړ����������߂�
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = enemy->GetMoveDegree();

		//�T�E���h�̍Đ�
		SoundManager(sound[11], false, false);

		//3�̐�������
		for (int i = 0; i < 3; i++)
		{
			angle += 120;

			//�Ǐ]�G�𐶐�����
			enemys.push_back(Chaser::Create(startPos, angle, true));
		}
	}

	//�G�����o�I�������ꍇ
	else if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Releaser)
	{
		//�������ɏ������W�ƈړ����������߂�
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = (float)(rand() % 360);

		//�T�E���h�̍Đ�
		SoundManager(sound[11], false, false);

		//�Ǐ]�G�𐶐�����
		enemys.push_back(Chaser::Create(startPos, angle, true));
	}
}

void GameScene::CameraUpdate(Camera* camera)
{
	//�V�F�C�N
	if (isShake)
	{
		camera->StartCameraShake(3);
		ShakeTime++;

		//�V�F�C�N�J�n�����莞�Ԍo������
		if (ShakeTime > 20) {
			isShake = false;
			ShakeTime = 0;

			//�V�F�C�N�I��
			camera->EndCameraShake();
		}
	}
	//�J�����������Z�b�g
	camera->TpsCamera(cameraPos);
	//�J�����X�V
	camera->Update();
}

void GameScene::SpawnEnemyManager(int score, int time)
{
	//Rush����Ȃ��Ƃ�
	if (!isRush)
	{
		//�e��Timer�X�V
		if (6 <= score) { rushTimer++; }
		spawnTimer++;

		//RushTimer�����l�ɒB������
		if (rushInterval <= rushTimer) { rushTimer = 0; isRush = true; isRushStart = true; }

		//Break�������Ƃ�Interval�X�V
		if (score <= 5) { spawnInterval = 100; }
		else if (score <= 15) { spawnInterval = 80; }
		else if (score <= 22) { spawnInterval = 60; }
		else if (score <= 32) { spawnInterval = 70; }
		else if (score <= 42) { spawnInterval = 60; }
		else if (score <= 50) { spawnInterval = 70; }
		else if (score <= 60) { spawnInterval = 30; }
		else { spawnInterval = 30; }

		//Break�������Ƃ�Rate�X�V
		spawnRate = 1;
	}

	//Rush�̂Ƃ�
	if (isRush)
	{
		//�e��Timer�X�V
		rushTimer++;
		spawnTimer++;

		//RushTimer�����l�ɒB������
		if (rushFinish <= rushTimer) { rushTimer = 0; isRush = false; isRushEnd = true; }

		//Break�������Ƃ�Interval�X�V
		if (score <= 5) { spawnInterval = 50; }
		else if (score <= 15) { spawnInterval = 40; }
		else if (score <= 22) { spawnInterval = 30; }
		else if (score <= 32) { spawnInterval = 35; }
		else if (score <= 42) { spawnInterval = 30; }
		else if (score <= 50) { spawnInterval = 35; }
		else if (score <= 60) { spawnInterval = 15; }
		else { spawnInterval = 15; }

		//Break�������Ƃ�Rate�X�V
		spawnRate = 2;
	}

	//Timer��Interval�𒴂�����G�𐶐�����
	if (spawnInterval <= spawnTimer)
	{
		//Timer��0�ɖ߂�
		spawnTimer = 0;

		//Rate�̐�������
		for (int i = 0; i < spawnRate; i++)
		{
			//�������ɏ������W�ƈړ�����������
			XMFLOAT3 startPos = {};
			float angle = 0;

			//�͈�
			float range = 5.0f;
			XMFLOAT2 startLineMin = { minWallLinePosition.x - range,minWallLinePosition.y - range };
			XMFLOAT2 startLineMax = { maxWallLinePosition.x + range,maxWallLinePosition.y + range };

			//�����œG�̎�ނ�����
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
			else if (score <= 55) { enemyTypeRand = rand() % 6; }
			else if (score <= 60) { enemyTypeRand = (rand() % 6) + 1; }
			else { enemyTypeRand = rand() % 10; }

			if (enemyTypeRand <= 3) { enemyType = 0; }
			else if (enemyTypeRand <= 6) { enemyType = 1; }
			else { enemyType = 2; }

			//�����œG���o���������������
			int enemyDirectionRand = rand() % 4;
			if (enemyDirectionRand == 0) { enemyDirection = 0; }
			else if (enemyDirectionRand == 1) { enemyDirection = 1; }
			else if (enemyDirectionRand == 2) { enemyDirection = 2; }
			else { enemyDirection = 3; }

			//�������W�ƈړ�����������
			if (enemyDirection == 0)
			{
				//�㑤����o��
				//�������W������
				startPos = { (float)(rand() % 300 - 150), startLineMax.y, 0 };

				//�ړ�����������(180�}30)
				angle = (float)(rand() % 61) + 150;
			}
			else if (enemyDirection == 1)
			{
				//��������o��
				//�������W������
				startPos = { startLineMin.x, (float)(rand() % 140 - 70), 0 };

				//�ړ�����������(270�}30)
				angle = (float)(rand() % 61) + 240;
			}
			else if (enemyDirection == 2)
			{
				//��������o��
				//�������W������
				startPos = { (float)(rand() % 300 - 150), startLineMin.y, 0 };

				//�ړ�����������(0�}30)
				angle = (float)(rand() % 61) - 30;
				if (angle < 0)
				{
					angle += 360;
				}
			}
			else
			{
				//�E������o��
				//�������W������
				startPos = { startLineMax.x, (float)(rand() % 140 - 70), 0 };

				//�ړ�����������(90�}30)
				angle = (float)(rand() % 61) + 60;
			}

			//�G�̐���
			if (enemyType == 0)
			{
				//�G1(�ǔ��^)�𐶐�
				enemys.push_back(Chaser::Create(startPos));
			}
			else if (enemyType == 1)
			{
				//�G2(�g�U�^)�𐶐�
				enemys.push_back(Division::Create(startPos, angle));
			}
			else if (enemyType == 2)
			{
				//��~�n�_�������_������
				XMFLOAT3 stayPos = {};
				stayPos.x = (float)(rand() % 200 - 100);
				stayPos.y = (float)(rand() % 120 - 60);

				//�G3(���o�^)�𐶐�
				enemys.push_back(Releaser::Create(startPos, stayPos));
			}
		}
	}
}

void GameScene::WallLineSet(const bool isTitle)
{
	//�ǂ̐����͈�
	XMFLOAT2 minWallPosition = WallObject::GetWallMinPosition();
	XMFLOAT2 maxWallPosition = WallObject::GetWallMaxPosition();

	//�^�C�g����ʂ̂Ƃ�
	if (isTitle)
	{
		minWallPosition.y += 8.0f;
		maxWallPosition.y += 12.0f;
	}
	//�Q�[����ʂ̂Ƃ�
	else
	{
		maxWallPosition.y += 4.0f;
	}
	//�ǐ����͈͂Ƃ̍���
	const float wallPosDis = 18.0f;
	minWallLinePosition = { minWallPosition.x + wallPosDis, minWallPosition.y + wallPosDis };
	maxWallLinePosition = { maxWallPosition.x - wallPosDis, maxWallPosition.y - wallPosDis };
	//�͈͊O�͈�
	const XMFLOAT2 outsideRange = { 25.0f, 45.0f };
	outsideMinPosition = { minWallLinePosition.x - outsideRange.x, minWallLinePosition.y - outsideRange.y };
	outsideMaxPosition = { maxWallLinePosition.x + outsideRange.x, maxWallLinePosition.y + outsideRange.y };

	//�Ǌ��蓖��
	Player::SetMoveRange(minWallLinePosition, maxWallLinePosition);
	XMFLOAT2 enemyWallLineMin = minWallLinePosition;
	enemyWallLineMin.y -= 2;
	XMFLOAT2 enemyWallLineMax = maxWallLinePosition;
	enemyWallLineMax.y += 2;
	BaseEnemy::SetWallLine(enemyWallLineMin, enemyWallLineMax);
}

bool GameScene::CheckPlayerToHealingZone(Player* player)
{
	//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
	XMFLOAT3 zonePos = { 1, -11, 0 };
	float zoneSize = 30.0f;
	XMFLOAT3 playerPos = player->GetPosition();
	float playerSize = player->GetScale().x;

	//�Փ˔�����v�Z
	bool isCollision = Collision::CheckCircle2Circle(
		zonePos, zoneSize, playerPos, playerSize);

	return isCollision;
}

void GameScene::SoundManager(int soundNum, bool isBGM, bool stop)
{
	Audio* audio = Audio::GetInstance();

	//SE�̏ꍇ
	if (!isBGM)
	{
		//�Đ�
		if (!stop)
		{
			audio->SoundPlayWava(soundNum, false);
		}
		//��~
		else
		{
			audio->StopSound(soundNum);
		}
	}
	//BGM�̏ꍇ
	else
	{
		//�Đ�
		if (!stop)
		{
			audio->SoundPlayWava(soundNum, true);
		}
		//��~
		else
		{
			audio->StopSound(soundNum);
		}
	}
}