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

const float radian = XM_PI / 180.0f;//���W�A��

GameScene::~GameScene()
{
	XInputManager* Xinput = XInputManager::GetInstance();
	//�U���I�t
	Xinput->EndVibration();
	Xinput = nullptr;

	//new�I�u�W�F�N�g�̔j��
	safe_delete(audio);
	safe_delete(light);

	//���f�����
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

	//�v���C���[���
	safe_delete(player);
	//�Ռ��g���
	for (int i = 0; i < shockWaveNum; i++)
	{
		safe_delete(shockWave[i]);
	}
	//�v���C���[�e���
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}
	//���e�n�_���
	safe_delete(landingPoint);

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
	//buckGround->AllDelete();
	//safe_delete(buckGround);
	safe_delete(backGround);

	//UI���͂��g���
	safe_delete(UIFrame);

	//�R���{���
	safe_delete(combo);
	//�������ԉ��
	safe_delete(timeLimit);
	//�X�R�A���
	safe_delete(breakScore);
	//����Ռ��g�p�Q�[�W���
	safe_delete(shockWaveGauge);
	//�������ԉ񕜗p�Q�[�W���
	safe_delete(timeLimitGauge);
	//ReadyGo���
	safe_delete(readyGo);
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
	playerModel = Model::CreateFromOBJ("playerbody");//�v���C���[�̃��f��

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
	Sprite::LoadTexture(3, L"Resources/combo.png");
	Sprite::LoadTexture(4, L"Resources/break.png");
	Sprite::LoadTexture(5, L"Resources/gauge.png");
	Sprite::LoadTexture(6, L"Resources/gaugeframe.png");
	Sprite::LoadTexture(7, L"Resources/ready.png");
	Sprite::LoadTexture(8, L"Resources/go.png");
	Sprite::LoadTexture(9, L"Resources/finish.png");
	Sprite::LoadTexture(10, L"Resources/result.png");
	Sprite::LoadTexture(11, L"Resources/maxcombo.png");
	Sprite::LoadTexture(12, L"Resources/retry.png");
	Sprite::LoadTexture(13, L"Resources/pressA.png");
	Sprite::LoadTexture(14, L"Resources/timematerIn.png");
	Sprite::LoadTexture(15, L"Resources/timematerOut.png");
	Sprite::LoadTexture(16, L"Resources/background.png");
	Sprite::LoadTexture(17, L"Resources/blackframe.png");

	//�f�o�b�O�e�L�X�g����
	DebugText::GetInstance()->Initialize(0);


	//�v���C���[����
	player = Player::Create(playerModel, waveModel);
	//�Ռ��g����
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i] = ShockWave::Create(waveModel);
	}
	//�v���C���[�e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(eBullModel);
	}
	//���e�n�_����
	landingPoint = LandingPoint::Create(waveModel);

	//�G�̃��f�����Z�b�g
	Straighter::SetModel(straighterModel1, straighterModel2, straighterModel3, straighterModel4);
	Division::SetModel(divisionModel1, divisionModel2, divisionModel3, divisionModel4);
	Releaser::SetModel(releaserModel1, releaserModel2, releaserModel3, releaserModel4);
	Chaser::SetModel(chaserModel1, chaserModel2, chaserModel3, chaserModel4);
	/*Straighter::SetModel(portaModel, portaModel, portaModel, portaModel);
	Division::SetModel(portaModel, portaModel, portaModel, portaModel);
	Releaser::SetModel(portaModel, portaModel, portaModel, portaModel);
	Chaser::SetModel(charoModel, charoModel, charoModel, charoModel);*/
	TitleLogo::SetModel(titleLogoModel);

	//�^�C�g�����S����
	enemys.push_back(TitleLogo::Create({ 0,60,0 }));

	//�ǐ���
	wall = WallManager::Create();

	//�ړ��͈͐ݒ�
	Player::SetMoveRange(minPosition, maxPosition);
	PlayerBullet::SetDeadPos(minPosition, maxPosition);
	LandingPoint::SetMoveRange(minPosition, maxPosition);
	XMFLOAT2 enemyWallLineMin = minPosition;
	enemyWallLineMin.y -= 2;
	XMFLOAT2 enemyWallLineMax = maxPosition;
	enemyWallLineMax.y += 2;
	BaseEnemy::SetWallLine(enemyWallLineMin, enemyWallLineMax);

	//�G�t�F�N�g������
	effects = new StageEffect();
	effects->Initialize();

	//�w�i����
	//buckGround->Create(hexagonModel);
	backGround = BackGround::Create(16);

	//UI���͂��g����
	UIFrame = UIFrame::Create(17);

	//�R���{����
	combo = Combo::Create(2, 3);
	//�������Ԑ���
	timeLimit = TimeLimit::Create(2);
	//�X�R�A����
	breakScore = BreakScore::Create(2, 4);
	//����Ռ��g�p�Q�[�W����
	shockWaveGauge = BigShockWaveGauge::Create(6, 5);
	//�������ԉ񕜗p�Q�[�W����
	timeLimitGauge = TimeLimitGauge::Create(15, 14);

	//ReadyGo����
	readyGo = ReadyGo::Create(7, 8);
	//Finish����
	finish = Finish::Create(9);
	//���U���g�V�[��UI����
	resultUI = ResultUI::Create(1, 10, 4, 2, 11, 12, 13);

	//�T�E���h�p
	audio = new Audio();
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//�^�C�g������
	if (scene == SceneName::TitleStay)
	{
		//���o���I�������玟�ɍs��
		if (wall->GetIsAlive())
		{
			scene = SceneName::TitleScene;
		}

		//�ǍX�V
		wall->Update();
	}

	//�^�C�g���V�[��
	else if (scene == SceneName::TitleScene)
	{
		//�v���C���[�X�V
		player->Update();
		//�|�C�̂ĊJ�n
		if (player->GetIsLitteringStart())
		{
			//�v���C���[�e����
			ShotPlayerBullet();
		}

		//�v���C���[�e�X�V
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�X�V����
			playerBullet[i]->Update();

			//�|�C�̂Ă���o��Ռ��g����
			if (playerBullet[i]->GetIsShockWaveStart())
			{
				LitteringShockWaveStart(playerBullet[i]->GetPosition());
				//���e�n�_�����S������
				landingPoint->Dead(i);
			}

			//�e����ł������Ă�����
			if (playerBullet[i]->GetIsAlive())
			{
				//���e�n�_�̒Ǐ]���X�g�b�v
				landingPoint->SetIsChase(false);
			}
		}

		//�Ռ��g�X�V
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Update();
		}

		//���e�n�_������ł���ꍇ
		if (landingPoint->CheckAllDead())
		{
			//�|�C�̂ďՌ��g���S�Ď��񂾂�
			if (!(shockWave[1]->GetIsAlive() || shockWave[2]->GetIsAlive() || shockWave[3]->GetIsAlive()))
			{
				//���e�n�_�Đ�
				landingPoint->Revive();
			}
		}
		//���e�n�_�X�V
		landingPoint->Update(player->GetPosition(), player->GetRotation());

		//�G�X�V
		BaseEnemy::SetTargetPos(player->GetPosition());
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			//�X�V����
			(*itrEnemy)->Update();

			//�폜��ԂɂȂ����玟�̃V�[����
			if ((*itrEnemy)->GetIsDelete())
			{
				scene = SceneName::ReadyGoScene;

				//�v���C���[���Q�[���J�n���̍��W�Ɉړ���ԂɃZ�b�g
				player->SetGameStartPos();

				//�v���C���[�e���˒��ł����e�n�_��Ǐ]��Ԃɖ߂�
				landingPoint->SetIsChase(true);
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
				GameCollision::CheckShockWaveToEnemy(shockWave[i], (*itrEnemy));
			}
		}

		//�ǍX�V
		wall->Update();
	}

	//ReadyGo�V�[��
	else if (scene == SceneName::ReadyGoScene)
	{
		//�v���C���[�X�V
		player->Update();
		//�v���C���[�e�X�V
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�X�V����
			playerBullet[i]->Update();
		}
		//�Ռ��g�X�V
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Update();
		}
		//���e�n�_������ł���ꍇ
		if (landingPoint->CheckAllDead())
		{
			//�|�C�̂ďՌ��g���S�Ď��񂾂�
			if (!(shockWave[1]->GetIsAlive() || shockWave[2]->GetIsAlive() || shockWave[3]->GetIsAlive()))
			{
				//���e�n�_�Đ�
				landingPoint->Revive();
			}
		}
		//���e�n�_�X�V
		landingPoint->Update(player->GetPosition(), player->GetRotation());

		//�v���C���[���Q�[���J�n���̍��W�ɓ��B������
		if (player->GetTriggerMoveStartPosEnd())
		{
			//UI����ʏ㕔�Ɉړ�������
			combo->SetMoveGamePos();
			timeLimitGauge->SetMoveGamePos();
			timeLimit->SetMoveGamePos();
			breakScore->SetMoveGamePos();
			shockWaveGauge->SetMoveGamePos();
		}
		//UI�X�V
		combo->Update();
		timeLimitGauge->Update();
		timeLimit->Update();
		breakScore->Update();
		shockWaveGauge->Update(0);

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

				//�v���C���[�̍s������������
				player->SetIsFreeMove(true);
				//�������Ԃ̃J�E���g�_�E�����J�n����
				timeLimit->SetIsCountDown(true);
			}
		}
	}

	//�Q�[���V�[��
	else if (scene == SceneName::GamePlayScene)
	{
		//�v���C���[�X�V
		player->Update();
		//���Ԋu�Ńv���C���[����Ռ��g����
		if (player->AutoShockWaveStart(combo->GetCombo()))
		{
			PlayerShockWaveStart(player->GetPosition());
		}
		//�|�C�̂ĊJ�n
		if (player->GetIsLitteringStart())
		{
			//�v���C���[�e����
			ShotPlayerBullet();
		}

		//�v���C���[�e�X�V
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�X�V����
			playerBullet[i]->Update();

			//�|�C�̂Ă���o��Ռ��g����
			if (playerBullet[i]->GetIsShockWaveStart())
			{
				LitteringShockWaveStart(playerBullet[i]->GetPosition());
				//���e�n�_�����S������
				landingPoint->Dead(i);
			}

			//�e����ł������Ă�����
			if (playerBullet[i]->GetIsAlive())
			{
				//���e�n�_�̒Ǐ]���X�g�b�v
				landingPoint->SetIsChase(false);
			}
		}

		//����Ռ��g����
		if (input->TriggerKey(DIK_Z) || Xinput->TriggerButton(XInputManager::PAD_A))
		{
			//����Ռ��g�𔭎�
			BigShockWaveStart(player->GetPosition());
		}
		//�Ռ��g�X�V
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Update();
		}

		//���e�n�_������ł���ꍇ
		if (landingPoint->CheckAllDead())
		{
			//�|�C�̂ďՌ��g���S�Ď��񂾂�
			if (!(shockWave[1]->GetIsAlive() || shockWave[2]->GetIsAlive() || shockWave[3]->GetIsAlive()))
			{
				//���e�n�_�Đ�
				landingPoint->Revive();
			}
		}
		//���e�n�_�X�V
		landingPoint->Update(player->GetPosition(), player->GetRotation());

		//�G����
		SpawnEnemyManager(breakScore->GetScore());

		//�G����
		//if (input->TriggerKey(DIK_1) || Xinput->TriggerButton(XInputManager::PAD_RT))
		//{
		//	//���i�G�X�|�[��
		//	SpawnStraighter();
		//}
		//if (input->TriggerKey(DIK_2) || Xinput->TriggerButton(XInputManager::PAD_LT))
		//{
		//	//�����G�X�|�[��
		//	SpawnDivision();
		//}
		//if (input->TriggerKey(DIK_3) || Xinput->TriggerButton(XInputManager::PAD_LB))
		//{
		//	//���o�G�X�|�[��
		//	SpawnReleaser();
		//}
		//if (input->TriggerKey(DIK_4) || Xinput->TriggerButton(XInputManager::PAD_RIGHT))
		//{
		//	//�Ǐ]�G�X�|�[��
		//	SpawnChaser();
		//}
		//if (input->TriggerKey(DIK_5))
		//{
		//	timeLimit->Recovery(5);
		//}
		//if (input->TriggerKey(DIK_6))
		//{
		//	timeLimitGauge->AddPoint(10);
		//}

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
					//�G�t�F�N�g���o��
					wall->SetHitEffect((*itrEnemy)->GetPosition());

					//�R���{�𑝂₷
					combo->AddCombo();

					//�������ԉ񕜃Q�[�W�̑����ʂ��v�Z(1 + ���݃R���{ / 10)
					recoveryPower = 1 + (combo->GetCombo()) / 10;

					//�������ԉ񕜃Q�[�W�̑����ʂ�11�ȏ�ɂ͂��Ȃ�
					if (recoveryPower >= 11)
					{
						recoveryPower = 11;
					}

					//�������ԉ񕜃Q�[�W�𑝂₷
					timeLimitGauge->AddPoint(recoveryPower);
				}
			}
			//�ǂ��Ȃ��ꍇ
			else
			{
				//��ʊO�ɏo����폜����
				XMFLOAT3 pos = (*itrEnemy)->GetPosition();
				if (pos.x <= minPosition.x || pos.x >= maxPosition.x ||
					pos.y <= minPosition.y || pos.y >= maxPosition.y)
				{
					(*itrEnemy)->SetDelete();
				}
			}

			//�v���C���[�ƓG�̓����蔻��
			if (GameCollision::CheckPlayerToEnemy(player, (*itrEnemy)))
			{
				//�R���{�I��
				combo->LostCombo();

				//��ʃV�F�C�N
				isShake = true;
			}

			//�Ռ��g�ƓG�̓����蔻��
			for (int i = 0; i < shockWaveNum; i++)
			{
				if (GameCollision::CheckShockWaveToEnemy(shockWave[i], (*itrEnemy)))
				{
					//�R���{�𑝂₷
					combo->AddCombo();

					//�������ԉ񕜃Q�[�W�̑����ʂ��v�Z(1 + ���݃R���{ / 10)
					recoveryPower = 1 + (combo->GetCombo()) / 10;

					//�������ԉ񕜃Q�[�W�̑����ʂ�11�ȏ�ɂ͂��Ȃ�
					if (recoveryPower >= 11)
					{
						recoveryPower = 11;
					}

					//�������ԉ񕜃Q�[�W�𑝂₷
					timeLimitGauge->AddPoint(recoveryPower);
				}
			}
		}

		//�ǍX�V
		wall->Update();
		if (wall->GetTriggerBreak())
		{
			breakScore->AddScore();
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


		//�R���{�X�V
		//if (Xinput->TriggerButton(XInputManager::PAD_START)) {
		//	combo->AddCombo();
		//	//�������ԉ񕜃Q�[�W�𑝂₷
		//	timeLimitGauge->AddPoint(combo->GetCombo());
		//}
		combo->Update();
		//�������ԉ񕜗p�Q�[�W�X�V
		timeLimitGauge->Update();
		//�������ԉ񕜃Q�[�W���ő�ɂȂ�����
		if (timeLimitGauge->GetIsGaugeMax())
		{
			//��������5�b��
			timeLimit->Recovery(3);
			//�|�C���g������
			timeLimitGauge->UsePoint();
		}
		//�������ԍX�V
		timeLimit->Update();
		//�������Ԃ�0�ɂȂ����玟�̃V�[����
		if (timeLimit->GetTime() <= 0)
		{
			scene = SceneName::FinishScene;

			//�v���C���[�����R�ɑ���ł��Ȃ�����
			player->SetIsFreeMove(false);

			//�v���C���[�e���˒��ł����e�n�_��Ǐ]��Ԃɖ߂�
			landingPoint->SetIsChase(true);

			//����ȏ�J�E���g�_�E�����Ȃ��̂�false�ɂ���
			timeLimit->SetIsCountDown(false);

			//���U���g�V�[���p�ɍő�R���{���ƕǔj�󐔂�n���Ă���
			resultUI->SetMaxCombo(combo->GetMaxCombo());
			resultUI->SetBreakWallNum(breakScore->GetScore());
		}

		//�X�R�A�X�V
		breakScore->Update();
		//����Ռ��g�p�Q�[�W�X�V
		shockWaveGauge->Update(combo->GetCombo());
	}

	//Finish�V�[��
	else if (scene == SceneName::FinishScene)
	{
		//�v���C���[�X�V
		player->Update();
		//�v���C���[�e�X�V
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�X�V����
			playerBullet[i]->Update();
		}
		//�Ռ��g�X�V
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Update();
		}
		//���e�n�_������ł���ꍇ
		if (landingPoint->CheckAllDead())
		{
			//�|�C�̂ďՌ��g���S�Ď��񂾂�
			if (!(shockWave[1]->GetIsAlive() || shockWave[2]->GetIsAlive() || shockWave[3]->GetIsAlive()))
			{
				//���e�n�_�Đ�
				landingPoint->Revive();
			}
		}
		//���e�n�_�X�V
		landingPoint->Update(player->GetPosition(), player->GetRotation());
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
		combo->Update();
		timeLimitGauge->Update();
		timeLimit->Update();
		breakScore->Update();
		shockWaveGauge->Update(0);

		//Finish�X�V
		finish->Update();
		//Finish��\�����I�����玟�̃V�[����
		if (finish->GetIsFinishSpriteMoveEnd())
		{
			scene = SceneName::ResultScene;

			//�G�����U���g�V�[���p�̓����ɕς���
			BaseEnemy::SetIsResultMove(true);

			//�ǂ����U���g�V�[���p�ɉ�ʊO�ɓ�����
			wall->SetChangeResult();

			//UI����ʊO�Ɉړ�������
			combo->SetMoveResultPos();
			timeLimitGauge->SetMoveResultPos();
			timeLimit->SetMoveResultPos();
			breakScore->SetMoveResultPos();
			shockWaveGauge->SetMoveResultPos();
		}
	}

	//���U���g�V�[��
	else if (scene == SceneName::ResultScene)
	{
		//�v���C���[�X�V
		player->Update();
		//���e�n�_������ł���ꍇ
		if (landingPoint->CheckAllDead())
		{
			//�|�C�̂ďՌ��g���S�Ď��񂾂�
			if (!(shockWave[1]->GetIsAlive() || shockWave[2]->GetIsAlive() || shockWave[3]->GetIsAlive()))
			{
				//���e�n�_�Đ�
				landingPoint->Revive();
			}
		}
		//���e�n�_�X�V
		landingPoint->Update(player->GetPosition(), player->GetRotation());
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
		combo->Update();
		timeLimitGauge->Update();
		timeLimit->Update();
		breakScore->Update();
		shockWaveGauge->Update(0);
		//���U���g�V�[��UI�X�V
		resultUI->Update();
		//�S�Ă̕`�悪�I�������
		if (resultUI->GetIsDrawAll())
		{
			//�w��̃{�^���������ƃQ�[�������Z�b�g
			if (input->TriggerKey(DIK_Z) || Xinput->TriggerButton(XInputManager::PAD_A))
			{
				ResetGame();
			}
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
	if (scene == SceneName::TitleStay || scene == SceneName::TitleScene)
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

		//�v���C���[�e�X�V
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�X�V����
			playerBullet[i]->Draw();
		}
		//���e�n�_�`��
		landingPoint->Draw();

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

		//���e�n�_�`��
		landingPoint->Draw();

		Object3d::PostDraw();

		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//UI���͂��g�`��
		UIFrame->Draw();

		//ReadyGo�`��
		readyGo->Draw();
		//�R���{�`��
		combo->Draw();
		//�������ԉ񕜗p�Q�[�W�`��
		timeLimitGauge->Draw();
		//�������ԕ`��
		timeLimit->Draw();
		//�X�R�A�`��
		breakScore->Draw();
		//����Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

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

		//�v���C���[�e�X�V
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�X�V����
			playerBullet[i]->Draw();
		}
		//���e�n�_�`��
		landingPoint->Draw();

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

		//�R���{�`��
		combo->Draw();
		//�������ԉ񕜗p�Q�[�W�`��
		timeLimitGauge->Draw();
		//�������ԕ`��
		timeLimit->Draw();
		//�X�R�A�`��
		breakScore->Draw();
		//����Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

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

		//�v���C���[�e�X�V
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�X�V����
			playerBullet[i]->Draw();
		}
		//���e�n�_�`��
		landingPoint->Draw();

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

		//�R���{�`��
		combo->Draw();
		//�������ԉ񕜗p�Q�[�W�`��
		timeLimitGauge->Draw();
		//�������ԕ`��
		timeLimit->Draw();
		//�X�R�A�`��
		breakScore->Draw();
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

		//���e�n�_�`��
		landingPoint->Draw();

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
		//�R���{�`��
		combo->Draw();
		//�������ԉ񕜗p�Q�[�W�`��
		timeLimitGauge->Draw();
		//�������ԕ`��
		timeLimit->Draw();
		//�X�R�A�`��
		breakScore->Draw();
		//����Ռ��g�p�Q�[�W�`��
		shockWaveGauge->Draw();

		//���U���g�V�[��UI�`��
		resultUI->Draw();

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

void GameScene::ResetGame()
{
	//�v���C���[������
	player->Reset();
	//�Ռ��g������
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i]->Reset();
	}
	//�v���C���[�e������
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Reset();
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

	//�R���{������
	combo->Reset();
	//�������ԏ�����
	timeLimit->Reset();
	//�󂵂��X�R�A������
	breakScore->Reset();
	//����Ռ��g�p�Q�[�W������
	shockWaveGauge->Reset();
	//�������ԉ񕜗p�Q�[�W������
	timeLimitGauge->Reset();

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
	//���˂���Ă����甲����
	if (shockWave[0]->GetIsAlive()) { return; }

	//�Ռ��g����
	shockWave[0]->PlayerWaveStart(pos);

	//��ʃV�F�C�N
	isShake = true;
}

void GameScene::LitteringShockWaveStart(XMFLOAT3 pos)
{
	for (int i = 1; i <= 3; i++)
	{
		//���˂���Ă������΂�
		if (shockWave[i]->GetIsAlive()) { continue; }

		//�Ռ��g����
		shockWave[i]->LitteringWaveStart(pos);

		//��ʃV�F�C�N
		isShake = true;

		//�ꔭ���˂����甲����
		break;
	}
}

void GameScene::BigShockWaveStart(XMFLOAT3 pos)
{
	//���˂���Ă����甲����
	if (shockWave[4]->GetIsAlive()) { return; }

	//�R���{��������Ȃ��ꍇ�͔�����
	if (combo->GetCombo() < 9) { return; }
	//�R���{���ɉ����ċ���Ռ��g�̈З͂�ύX
	int shockWavePowerLevel = 0;
	if (combo->GetCombo() < 19) { shockWavePowerLevel = 1; }
	else if (combo->GetCombo() < 29) { shockWavePowerLevel = 2; }
	else { shockWavePowerLevel = 3; }

	//����Ռ��g����
	shockWave[4]->BigWaveStart(pos, shockWavePowerLevel);

	//����Ռ��g�𔭎˂����ꍇ�R���{���I������
	combo->LostCombo();

	//��ʃV�F�C�N
	isShake = true;
}

void GameScene::ShotPlayerBullet()
{
	//�|�C�̂ďՌ��g���S�ď�����܂Œe�����ĂȂ��悤�ɂ���
	if (shockWave[1]->GetIsAlive() || shockWave[2]->GetIsAlive() || shockWave[3]->GetIsAlive()) { return; }

	//�v���C���[�E�G�|���̍��W�Ɗp�x��e������
	XMFLOAT3 pos = player->GetPosition();
	float angle = player->GetRotation().z;

	//�����̒e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		//���˂���Ă������΂�
		if (playerBullet[i]->GetIsAlive()) { continue; }

		//�e����
		playerBullet[i]->BulletStart(pos, angle - 30);

		//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
		break;
	}

	//�^�񒆂̒e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		//���˂���Ă������΂�
		if (playerBullet[i]->GetIsAlive()) { continue; }

		//�e����
		playerBullet[i]->BulletStart(pos, angle);

		//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
		break;
	}

	//�E���̒e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		//���˂���Ă������΂�
		if (playerBullet[i]->GetIsAlive()) { continue; }

		//�e����
		playerBullet[i]->BulletStart(pos, angle + 30);

		//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
		break;
	}
}

void GameScene::SpawnStraighter()
{
	//�������ɏ������W�ƈړ����������߂�
	XMFLOAT3 startPos = {};
	float angle = 0;

	//�͈�
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minPosition.x - range,minPosition.y - range };
	XMFLOAT2 startLineMax = { maxPosition.x + range,maxPosition.y + range };

	//4�p�^�[���̃����_���ŏ������W�ƈړ��������Z�b�g
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; angle = 300; }

	//���i�G�𐶐�
	enemys.push_back(Straighter::Create(startPos, angle));
}

void GameScene::SpawnDivision()
{
	//�������ɏ������W�ƈړ����������߂�
	XMFLOAT3 startPos = {};
	float angle = 0;

	//�͈�
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minPosition.x - range,minPosition.y - range };
	XMFLOAT2 startLineMax = { maxPosition.x + range,maxPosition.y + range };

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
	XMFLOAT2 startLineMin = { minPosition.x - range,minPosition.y - range };
	XMFLOAT2 startLineMax = { maxPosition.x + range,maxPosition.y + range };

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

void GameScene::SpawnChaser()
{
	//�������ɏ������W�����߂�
	XMFLOAT3 startPos = {};

	//�͈�
	float range = 5.0f;
	XMFLOAT2 startLineMin = { minPosition.x - range,minPosition.y - range };
	XMFLOAT2 startLineMax = { maxPosition.x + range,maxPosition.y + range };

	//4�p�^�[���̃����_���ŏ������W�ƈړ��������Z�b�g
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, startLineMin.y, 0 }; }
	else if (posAngleRand == 1) { startPos = { startLineMax.x, 0, 0 }; }
	else if (posAngleRand == 2) { startPos = { 0, startLineMax.y, 0 }; }
	else if (posAngleRand == 3) { startPos = { startLineMin.x, 0, 0 }; }

	//�Ǐ]�G�𐶐�
	enemys.push_back(Chaser::Create(startPos));
}

void GameScene::SpawnEnemyToEnemy(BaseEnemy* enemy)
{
	//�G�������I�������ꍇ
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Division)
	{
		//�������ɏ������W�ƈړ����������߂�
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = enemy->GetMoveDegree();
		int parantKnockBackPowerLevel = enemy->GetKnockBackPowerLevel();

		//3�̐�������
		for (int i = 0; i < 3; i++)
		{
			angle += 120;

			//�e�̃m�b�N�o�b�N�̋����������p�������i�G�𐶐�����
			enemys.push_back(Straighter::Create(startPos, angle, parantKnockBackPowerLevel));
		}
	}

	//�G�����o�I�������ꍇ
	else if (enemy->GetGroup() == BaseEnemy::EnemyGroup::Releaser)
	{
		//�������ɏ������W�ƈړ����������߂�
		XMFLOAT3 startPos = enemy->GetPosition();
		float angle = (float)(rand() % 360);
		int parantKnockBackPowerLevel = enemy->GetKnockBackPowerLevel();

		//�e�̃m�b�N�o�b�N�̋����������p�������i�G�𐶐�����
		enemys.push_back(Straighter::Create(startPos, angle, parantKnockBackPowerLevel));
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

void GameScene::SpawnEnemyManager(int score)
{
	//Timer�X�V
	spawnTimer++;

	//Interval�X�V
	spawnInterval = 60;

	//Interval��60�ȉ��ɂ͂Ȃ�Ȃ�
	if (spawnInterval <= 60)
	{
		spawnInterval = 60;
	}

	//Rate�X�V(1 + �j�󂵂��ǂ̐� / 3)
	spawnRate = 1 + (score / 3);

	//Rate��5�ȏ�ɂ͂Ȃ�Ȃ�
	if (spawnRate >= 5)
	{
		spawnRate = 5;
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
			XMFLOAT2 startLineMin = { minPosition.x - range,minPosition.y - range };
			XMFLOAT2 startLineMax = { maxPosition.x + range,maxPosition.y + range };

			//�����œG�̎�ނ�����
			int enemyTypeRand = rand() % 10;
			if (enemyTypeRand <= 2) { enemyType = 0; }
			else if (enemyTypeRand <= 4) { enemyType = 1; }
			else if (enemyTypeRand <= 6) { enemyType = 2; }
			else { enemyType = 3; }

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
				startPos = { startLineMin.x, (float)(rand() % 120 - 60), 0 };

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
				startPos = { startLineMax.x, (float)(rand() % 120 - 60), 0 };

				//�ړ�����������(90�}30)
				angle = (float)(rand() % 61) + 60;
			}

			//�G�̐���
			if (enemyType == 0)
			{
				//�G1(���i�^)�𐶐�
				enemys.push_back(Straighter::Create(startPos, angle));
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
			else
			{
				//�G4(�ǔ��^)�𐶐�
				enemys.push_back(Chaser::Create(startPos));
			}
		}
	}
}