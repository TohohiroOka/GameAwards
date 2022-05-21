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
	safe_delete(circleModel);
	safe_delete(playerModel);
	safe_delete(RBModel);

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

	//�v���C���[���
	safe_delete(player);
	//�Ռ��g���
	for (int i = 0; i < shockWaveNum; i++)
	{
		safe_delete(shockWave[i]);
	}

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

	//�^�C�g���V�[��UI���
	safe_delete(titleUI);
	//�Q�[���������
	safe_delete(explanation);

	//UI���͂��g���
	safe_delete(UIFrame);

	//�ǔj��X�R�A���
	safe_delete(breakScore);
	//�������ԉ��
	safe_delete(timeLimitGauge);
	//����Ռ��g�p�Q�[�W���
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
	circleModel = Model::CreateFromOBJ("circle");//�^�o�R�̃��f��
	playerModel = Model::CreateFromOBJ("player");//�v���C���[�̃��f��
	RBModel = Model::CreateFromOBJ("RB");//RB�̃��f��

	straighterModel1 = Model::CreateFromOBJ("enemy1_1");//���i�G�̃��f��1
	straighterModel2 = Model::CreateFromOBJ("enemy1_2");//���i�G�̃��f��2
	straighterModel3 = Model::CreateFromOBJ("enemy1_3");//���i�G�̃��f��3
	straighterModel4 = Model::CreateFromOBJ("enemy1_4");//���i�G�̃��f��4

	divisionModel1 = Model::CreateFromOBJ("enemy2_1");//�����G�̃��f��1
	divisionModel2 = Model::CreateFromOBJ("enemy2_2");//�����G�̃��f��2
	divisionModel3 = Model::CreateFromOBJ("enemy2_3");//�����G�̃��f��3
	divisionModel4 = Model::CreateFromOBJ("enemy2_4");//�����G�̃��f��4

	releaserModel1 = Model::CreateFromOBJ("enemy3_1");//���o�G�̃��f��1
	releaserModel2 = Model::CreateFromOBJ("enemy3_2");//���o�G�̃��f��2
	releaserModel3 = Model::CreateFromOBJ("enemy3_3");//���o�G�̃��f��3
	releaserModel4 = Model::CreateFromOBJ("enemy3_4");//���o�G�̃��f��4

	chaserModel1 = Model::CreateFromOBJ("enemy4_1");//�ǐՓG�̃��f��1
	chaserModel2 = Model::CreateFromOBJ("enemy4_2");//�ǐՓG�̃��f��2
	chaserModel3 = Model::CreateFromOBJ("enemy4_3");//�ǐՓG�̃��f��3
	chaserModel4 = Model::CreateFromOBJ("enemy4_4");//�ǐՓG�̃��f��4

	eBullModel = Model::CreateFromOBJ("enemybullet");//�G�̒e�̃��f��
	hexagonModel = Model::CreateFromOBJ("hexagon");//�Z�p�`�̃��f��
	portaModel = Model::CreateFromOBJ("porta");//�|���^�̃��f��
	charoModel = Model::CreateFromOBJ("charo");//�`�����̃��f��
	frameModel = Model::CreateFromOBJ("frame");//�t���[���̃��f��
	waveModel = Model::CreateFromOBJ("wave");//�Ռ��g�̃��f��


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

	//�f�o�b�O�e�L�X�g����
	DebugText::GetInstance()->Initialize(0);


	//�v���C���[����
	player = Player::Create(playerModel);
	//�Ռ��g����
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i] = ShockWave::Create(waveModel);
	}


	//�G�̃��f�����Z�b�g
	Chaser::SetModel(straighterModel4);
	Division::SetModel(divisionModel3);
	Releaser::SetModel(releaserModel2);
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

	//�^�C�g���V�[��UI����
	titleUI = TitleUI::Create(RBModel);
	//�Q�[����������
	explanation = Explanation::Create(20);

	//UI���͂��g����
	UIFrame = UIFrame::Create(17, 18);


	//�ǔj��X�R�A����
	breakScore = BreakScore::Create(19, 2);
	//�������Ԑ���
	timeLimitGauge = TimeLimitGauge::Create(13, 14, 15);
	//����Ռ��g�p�Q�[�W����
	shockWaveGauge = BigShockWaveGauge::Create(6, 5);

	//ReadyGo����
	readyGo = ReadyGo::Create(7, 8);
	//�|�[�Y�V�[��UI����
	pauseUI = PauseUI::Create(1, 4, 3, 12);
	//Finish����
	finish = Finish::Create(9);
	//���U���g�V�[��UI����
	resultUI = ResultUI::Create(1, 10, 19, 2, 11, 12);

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

		//�|�C�̂ĊJ�n
		if (player->GetIsShockWaveStart())
		{
			//�v���C���[�Ռ��g����
			PlayerShockWaveStart(player->GetPosition());
		}

		//�Ռ��g�X�V
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Update();
		}

		//�G�X�V
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//�X�V����
			(*itrEnemy)->Update();

			//�폜��ԂɂȂ����玟�̃V�[����
			if ((*itrEnemy)->GetIsDelete())
			{
				//�T�E���h�̍Đ�
				SoundManager(sound[5], false, false);
				scene = SceneName::ReadyGoScene;

				//�v���C���[���Q�[���J�n���̍��W�Ɉړ���ԂɃZ�b�g
				player->SetGameStartPos();

				//�Q�[����������ʊO�Ɉړ�
				explanation->SetMoveOutScreen();
			}

			//�G�������Ă��Ȃ���Δ�΂�
			if (!(*itrEnemy)->GetIsAlive()) { continue; }

			//�ǂƓG�̓����蔻������
			GameCollision::CheckWallToEnemy(wall, (*itrEnemy));

			//�v���C���[�ƓG�̓����蔻��
			GameCollision::CheckPlayerToEnemy(player, (*itrEnemy));
			//�Ռ��g�ƓG�̓����蔻��
			for (int i = 0; i < shockWaveNum; i++)
			{
				//�������Ă��Ȃ���Δ�΂�
				if (!GameCollision::CheckShockWaveToEnemy(shockWave[i], (*itrEnemy))) { continue; }

				//�T�E���h�̍Đ�
				SoundManager(sound[4], false, false);

				GameCollision::CheckShockWaveToEnemy(shockWave[i], (*itrEnemy));
			}
		}
		//�^�C�g�����S�̃X�|�[��������������
		if (TitleLogo::GetTriggerSpawnEnd())
		{
			//�v���C���[����\
			player->SetIsFreeMove(true);
			//UI��`�悷��
			titleUI->SetIsDraw(true);
			//�Q�[����������ʓ��Ɉړ�������
			explanation->SetMoveInScreen();
		}

		//�ǍX�V
		wall->Update();
		//�^�C�g���V�[���pUI�X�V
		titleUI->Update(player->GetPosition());
		//�Q�[�������X�V
		explanation->Update();
	}

	//ReadyGo�V�[��
	else if (scene == SceneName::ReadyGoScene)
	{
		//�v���C���[�X�V
		player->Update();
		//�Ռ��g�X�V
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Update();
		}

		//�v���C���[���Q�[���J�n���̍��W�ɓ��B������
		if (player->GetTriggerMoveStartPosEnd())
		{
			//UI����ʏ㕔�Ɉړ�������
			UIFrame->SetMoveGamePos();
			timeLimitGauge->SetMoveGamePos();
			shockWaveGauge->SetMoveGamePos();
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
			//�^�C�g���V�[���Ŕj�󂵂��ǂ̂݉��������C����Ԃɂ���
			wall->GetTriggerBreak();

			//ReadyGo�X�v���C�g�X�V
			readyGo->Update();
			//ReadyGo��Go�܂ŕ\�����I������玟�̃V�[����
			if (readyGo->GetIsReadyGoEnd())
			{
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
		//�v���C���[�X�V
		player->Update();
		//�v���C���[�Ռ��g����
		if (player->GetIsShockWaveStart())
		{
			//�v���C���[�Ռ��g����
			PlayerShockWaveStart(player->GetPosition());
		}
		//����Ռ��g����
		if (input->TriggerKey(DIK_Z) || Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_A))
		{
			//����Ռ��g�𔭎�
			BigShockWaveStart(player->GetPosition());
		}
		//�Ռ��g�X�V
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Update();
		}

		//�G����
		SpawnEnemyManager(breakScore->GetBreakScore(), timeLimitGauge->GetTimer());

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

			//�ǂ�����ꍇ
			if (wall->GetIsAlive())
			{
				//�ǂƓG�̓����蔻������
				if (GameCollision::CheckWallToEnemy(wall, (*itrEnemy)))
				{
					//�T�E���h�̍Đ�
					SoundManager(sound[5], false, false);
					//�G�t�F�N�g���o��
					StageEffect::SetSmash((*itrEnemy)->GetPosition());
					//�U���I��
					Xinput->StartVibration(XInputManager::STRENGTH::MEDIUM, 10);
				}
			}
			//�ǂ��Ȃ��ꍇ
			else
			{
				//��ʊO�ɏo����폜����
				XMFLOAT3 pos = (*itrEnemy)->GetPosition();
				if (pos.x <= minWallLinePosition.x || pos.x >= maxWallLinePosition.x ||
					pos.y <= minWallLinePosition.y || pos.y >= maxWallLinePosition.y)
				{
					(*itrEnemy)->SetDelete();
				}
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
			for (int i = 0; i < shockWaveNum; i++)
			{
				//�������Ă��Ȃ���Δ�΂�
				if (!GameCollision::CheckShockWaveToEnemy(shockWave[i], (*itrEnemy))) { continue; }

				//�T�E���h�̍Đ�
				SoundManager(sound[4], false, false);

				//������΂����Ռ��g������Ռ��g�Ȃ��΂�
				if ((*itrEnemy)->GetLastCollisionShockWave() == ShockWave::ShockWaveGroup::BigWave) { continue; }

				//�K�E�Z�Q�[�W�𑝂₷
				shockWaveGauge->AddPoint();
			}
		}

		//�ǍX�V
		wall->Update();
		if (wall->GetTriggerBreak())
		{
			//�󂵂��X�R�A���Z
			breakScore->AddScore();

			//�^�C�����~�b�g���L�т�
			timeLimitGauge->Recovery(10);
		}

		if (Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_BUCK))
		{
			//�󂵂��X�R�A���Z
			breakScore->AddScore();
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

			//�v���C���[�����R�ɑ���ł��Ȃ�����
			player->SetIsFreeMove(false);

			//���U���g�V�[���p�ɕǔj�󐔂�n���Ă���
			resultUI->SetBreakWallNum(breakScore->GetBreakScore());
		}

		//����Ռ��g�p�Q�[�W�X�V
		shockWaveGauge->Update();

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
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Update();
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

			//�ǂ�����ꍇ
			if (wall->GetIsAlive())
			{
				//�ǂƓG�̓����蔻������
				GameCollision::CheckWallToEnemy(wall, (*itrEnemy));
			}
			//�ǂ��Ȃ��ꍇ
			else
			{
				//��ʊO�ɏo����폜����
				XMFLOAT3 pos = (*itrEnemy)->GetPosition();

				//�͈͊O����
				if (pos.x <= outsideMinPosition.x || pos.x >= outsideMaxPosition.x ||
					pos.y <= outsideMinPosition.y || pos.y >= outsideMaxPosition.y)
				{
					(*itrEnemy)->SetDelete();
				}
			}
		}

		//�ǍX�V
		wall->Update();
		//UI�X�V
		UIFrame->Update();
		timeLimitGauge->Update();
		shockWaveGauge->Update();

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
			shockWaveGauge->SetMoveResultPos();
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
	//buckGround->Update();
	backGround->Update();
	//UI���͂��g�X�V
	UIFrame->Update();
	//�J�����X�V
	CameraUpdate(camera);

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

		Sprite::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�^�C�g���V�[���pUI�`��
		titleUI->Draw();

		//�Ǖ`��
		wall->Draw();

		//�v���C���[�`��
		player->Draw();

		//�Ռ��g�`��
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Draw();
		}

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
		//����Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

		//�Q�[�������`��
		explanation->Draw();

		//�f�o�b�O�e�L�X�g�`��
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//�Q�[���V�[��
	else if (scene == SceneName::GamePlayScene)
	{
		//�X�v���C�g�w�ʕ`��
		Sprite::PreDraw(cmdList);

		//�w�i�`��
		backGround->Draw();

		Sprite::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�Ǖ`��
		wall->Draw();

		//�v���C���[�`��
		player->Draw();

		//�Ռ��g�`��
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Draw();
		}

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
		//����Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

		//�f�o�b�O�e�L�X�g�`��
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//�|�[�Y�V�[��
	else if (scene == SceneName::PauseScene)
	{
		//�X�v���C�g�w�ʕ`��
		Sprite::PreDraw(cmdList);

		//�w�i�`��
		backGround->Draw();

		Sprite::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�Ǖ`��
		wall->Draw();

		//�v���C���[�`��
		player->Draw();

		//�Ռ��g�`��
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Draw();
		}

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
		//����Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

		//�|�[�Y�V�[��UI�`��
		pauseUI->Draw();

		//�V�[���J�ڗp�Ó]�`��
		blackout->Draw();

		//�f�o�b�O�e�L�X�g�`��
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//Finish�V�[��
	else if (scene == SceneName::FinishScene)
	{
		//�X�v���C�g�w�ʕ`��
		Sprite::PreDraw(cmdList);

		//�w�i�`��
		backGround->Draw();

		Sprite::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�Ǖ`��
		wall->Draw();

		//�v���C���[�`��
		player->Draw();

		//�Ռ��g�`��
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Draw();
		}

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
		//����Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();
		//Finish�`��
		finish->Draw();

		//�f�o�b�O�e�L�X�g�`��
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//���U���g�V�[��
	else if (scene == SceneName::ResultScene)
	{
		//�X�v���C�g�w�ʕ`��
		Sprite::PreDraw(cmdList);

		//�w�i�`��
		backGround->Draw();

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
		//����Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

		//���U���g�V�[��UI�`��
		resultUI->Draw();

		//�V�[���J�ڗp�Ó]�`��
		blackout->Draw();

		//�f�o�b�O�e�L�X�g�`��
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}

	//�S�V�[�����ʂ̕`��
	{
		//�G�t�F�N�g�̕`��
		effects->Draw(cmdList);
	}
}

void GameScene::ResetTitleScene()
{
	//�v���C���[������
	player->ResetTitle();
	//�Ռ��g������
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i]->Reset();
	}
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

	//UI��`�悵�Ȃ�
	titleUI->SetIsDraw(false);
	//�Q�[������������
	explanation->Reset();
	//UI�t���[��������
	UIFrame->Reset();

	//�ǔj��X�R�A������
	breakScore->Reset();
	//�������ԏ�����
	timeLimitGauge->Reset();
	//����Ռ��g�p�Q�[�W������
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

	//�X�|�[������܂ł̃J�E���g������
	spawnTimer = 0;
}

void GameScene::ResetGame()
{
	//�v���C���[������
	player->ResetGame();
	//�Ռ��g������
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i]->Reset();
	}
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
	//�������ԏ�����
	timeLimitGauge->Reset();
	//����Ռ��g�p�Q�[�W������
	shockWaveGauge->Reset();

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

	//�X�|�[������܂ł̃J�E���g������
	spawnTimer = 0;
}

void GameScene::PlayerShockWaveStart(XMFLOAT3 pos)
{
	//�v���C���[���_���[�W��ԂȂ甲����
	if (player->GetIsDamege()) { return; }

	//���˂���Ă����甲����
	if (shockWave[0]->GetIsAlive()) { return; }

	//�Ռ��g����
	shockWave[0]->PlayerWaveStart(pos);

	//��ʃV�F�C�N
	isShake = true;

	//�T�E���h�̍Đ�
	SoundManager(sound[3], false, false);
}

void GameScene::BigShockWaveStart(XMFLOAT3 pos)
{
	Audio* audio = Audio::GetInstance();

	//�v���C���[���_���[�W��ԂȂ甲����
	if (player->GetIsDamege()) { return; }

	//���˂���Ă����甲����
	if (shockWave[1]->GetIsAlive()) { return; }

	//�Q�[�W���x��������Ȃ��ꍇ�͔�����
	if (shockWaveGauge->GetGaugeLevel() == 0) { return; }
	//�Q�[�W���x���ɉ����ċ���Ռ��g�̈З͂�ύX
	int shockWavePowerLevel = 0;
	if (shockWaveGauge->GetGaugeLevel() == 1) { shockWavePowerLevel = 1; }
	else if (shockWaveGauge->GetGaugeLevel() == 2) { shockWavePowerLevel = 2; }
	else if (shockWaveGauge->GetGaugeLevel() == 3) { shockWavePowerLevel = 3; }
	else { return; }

	//����Ռ��g����
	shockWave[1]->BigWaveStart(pos, shockWavePowerLevel);

	//�Q�[�W��0�ɂ���
	shockWaveGauge->UsePoint();

	//��ʃV�F�C�N
	isShake = true;

	//�U��
	XInputManager* Xinput = XInputManager::GetInstance();
	XInputManager::STRENGTH strength = XInputManager::STRENGTH::SMALL;
	if (shockWavePowerLevel == 2)
	{
		strength = XInputManager::STRENGTH::MEDIUM;
	}
	else if (shockWavePowerLevel == 3)
	{
		strength = XInputManager::STRENGTH::LARGE;
	}
	Xinput->StartVibration(strength, 10);
	Xinput = nullptr;

	//�T�E���h�̍Đ�
	SoundManager(sound[7], false, false);
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
	//Timer�X�V
	spawnTimer++;

	//Interval�X�V
	spawnInterval = 60 - score * 5;

	//Interval��60�ȉ��ɂ͂Ȃ�Ȃ�
	if (spawnInterval <= 60)
	{
		spawnInterval = 60;
	}

	//Rate�X�V
	if (time <= 1200)
	{
		spawnRate = 4;
	}
	else if (time <= 2400)
	{
		spawnRate = 3;
	}
	else
	{
		spawnRate = 2;
	}

	//Rate��5�ȏ�ɂ͂Ȃ�Ȃ�
	if (spawnRate >= 10)
	{
		spawnRate = 10;
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
			if (score <= 1) { enemyTypeRand = rand() % 4; }
			else if (score <= 3) { enemyTypeRand = rand() % 7; }
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