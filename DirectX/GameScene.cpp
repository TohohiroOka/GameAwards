#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

#include "Garuta.h"
#include "Garutata.h"

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

	//�K�������
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		safe_delete(*itrGaruEnemy);
	}
	//�K�����̃��X�g���
	garuEnemys.clear();

	//�G�̒e���
	for (int i = 0; i < enemyBulletNum; i++)
	{
		safe_delete(enemyBullet[i]);
	}

	//��ʘg���
	safe_delete(frame);

	//�G�t�F�N�g�̉��
	safe_delete(effects);

	//�w�i�̉��
	buckGround->AllDelete();
	safe_delete(buckGround);

	//�R���{���
	safe_delete(combo);

	//�������ԉ��
	safe_delete(timeLimit);
}

void GameScene::Initialize(Camera* camera)
{
	// nullptr�`�F�b�N
	assert(camera);

	//���C�g
	light = LightGroup::Create();
	light->DefaultLightSetting();
	Object3d::SetLightGroup(light);

	circleModel = Model::CreateFromOBJ("circle");//�^�o�R�̃��f��
	playerModel = Model::CreateFromOBJ("player");//�v���C���[�̃��f��
	pBodyModel = Model::CreateFromOBJ("playerbody");//�v���C���[�̑̂̃��f��
	pHead01Model = Model::CreateFromOBJ("playerhead1");//�v���C���[�̓��̃��f��(HP1)
	pHead02Model = Model::CreateFromOBJ("playerhead2");//�v���C���[�̓��̃��f��(HP2)
	pHead03Model = Model::CreateFromOBJ("playerhead3");//�v���C���[�̓��̃��f��(HP3)
	enemy01Model = Model::CreateFromOBJ("enemy");//�G01(�K���^)�̃��f��
	enemyPoint01Model = Model::CreateFromOBJ("garutaspown");//�G01(�K���^)�̏o���ʒu�̃��f��
	enemy02Model = Model::CreateFromOBJ("garutata");//�G02(�K���^�^)�̃��f��
	enemyPoint02Model = Model::CreateFromOBJ("garutataspown");//�G02(�K���^�^)�̏o���ʒu�̃��f��
	eBullModel = Model::CreateFromOBJ("enemybullet");//�G�̒e�̃��f��
	deadEnemyModel = Model::CreateFromOBJ("desenemy");//���񂾓G�̃��f��
	initialCircleCoreModel = Model::CreateFromOBJ("initialcirclecore");;//�Œ�G�̃R�A�̃��f��
	initialCircleSquareModel = Model::CreateFromOBJ("initialcirclesquare");//�Œ�G�̊O�g�̃��f��
	hexagonModel = Model::CreateFromOBJ("hexagon");//�Z�p�`�̃��f��
	happyModel = Model::CreateFromOBJ("happy");//�^�o�R���f��
	portaModel = Model::CreateFromOBJ("porta");//�|���^�̃��f��
	charoModel = Model::CreateFromOBJ("charo");//�`�����̃��f��
	tuffModel = Model::CreateFromOBJ("tuff");//�^�b�t�̃��f��
	frameModel = Model::CreateFromOBJ("frame");//�t���[���̃��f��
	waveModel = Model::CreateFromOBJ("wave");//�Ռ��g�̃��f��
	RBbuttonModel = Model::CreateFromOBJ("RB");//RB�{�^���̃��f��


	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/white1x1.png");
	Sprite::LoadTexture(2, L"Resources/number.png");
	Sprite::LoadTexture(3, L"Resources/combo.png");
	Sprite::LoadTexture(4, L"Resources/result.png");
	Sprite::LoadTexture(5, L"Resources/pressButton.png");
	Sprite::LoadTexture(6, L"Resources/title.png");
	//�f�o�b�O�e�L�X�g����
	DebugText::GetInstance()->Initialize(0);


	//�v���C���[����
	player = Player::Create(pBodyModel);
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
	//�G�̒e����
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//��ʘg����
	frame = Frame::Create(frameModel);
	XMFLOAT2 frameline = frame->GetFrameLine();
	Player::SetMoveRange(frameline);
	PlayerBullet::SetDeadPos(frameline);

	//�G�t�F�N�g������
	effects = new StageEffect();
	effects->Initialize();

	//�w�i�̏�����
	buckGround->Create(hexagonModel);

	//�R���{����
	combo = Combo::Create(2, 3);

	//�������Ԑ���
	timeLimit = TimeLimit::Create(2);

	//�T�E���h�p
	audio = new Audio();
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//��ʘg�X�V
	frame->Update();

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

	//�K��������
	if (input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_RT))
	{
		//�K�������X�|�[��
		SpawnGaruEnemy();
	}
	//�K�����X�V
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		//�X�V����
		(*itrGaruEnemy)->Update();

		//�K�����������Ă��Ȃ���Δ�΂� �K�������X�|�[�������������΂� �K���������������������΂�
		if (!(*itrGaruEnemy)->GetIsAlive() || (*itrGaruEnemy)->GetIsDuringSpawn() || (*itrGaruEnemy)->GetIsEscape()) { continue; }

		//�e���˃t���O��true�Ȃ�
		if ((*itrGaruEnemy)->GetIsBulletShot())
		{
			//�e����
			GaruEnemyShotBullet(*itrGaruEnemy);
		}

		//�g�ɓ�����������������
		if ((*itrGaruEnemy)->IsCollisionFrame(frame->GetFrameLine()))
		{
			//���S
			(*itrGaruEnemy)->Dead();

			//�g�ɂ��_���[�W��^����
			int damagePower = 10;
			frame->Damage(damagePower);

			//�R���{�𑝂₷
			combo->AddCombo();
		}

		//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
		XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
		float enemySize = (*itrGaruEnemy)->GetScale().x;

		//�v���C���[�ƃK�����̓����蔻��
		//�v���C���[���_���[�W��ԂłȂ��Ȃ�
		if (!player->GetIsDamege())
		{
			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//�K�����ƃv���C���[���Փˏ��
			if (isCollision)
			{
				//�v���C���[�̓_���[�W����炤
				player->Damage();
				player->SetKnockback();

				//�G�����S
				(*itrGaruEnemy)->SetDelete();

				//�R���{�I��
				combo->LostCombo();

				isShake = true;
			}
		}

		//�Ռ��g�ƃK�����̓����蔻��
		for (int i = 0; i < shockWaveNum; i++)
		{
			//�Ռ��g�����ˏ��
			if (shockWave[i]->GetIsAlive())
			{
				//�����蔻��p�ϐ�
				XMFLOAT3 wavePos = shockWave[i]->GetPosition();
				float waveSize = shockWave[i]->GetRadius();

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					wavePos, waveSize, enemyPos, enemySize);

				//�Ռ��g�ƃK�������Փˏ��
				if (isCollision)
				{
					//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��_���[�W��H�炢�����Ă��܂�����)
					if (!shockWave[i]->IsKnowGaruEnemy((*itrGaruEnemy)))
					{
						//�K�������m�b�N�o�b�N�Ŕ�΂�
						float angle = atan2f(enemyPos.y - wavePos.y, enemyPos.x - wavePos.x);
						int powerLevel = shockWave[i]->GetPowerLevel();
						(*itrGaruEnemy)->SetKnockBack(angle, powerLevel);

						//�R���{�𑝂₷
						combo->AddCombo();
					}
				}
			}
		}
	}

	//�G�̒e�X�V
	for (int i = 0; i < enemyBulletNum; i++)
	{
		//�e�������Ă��Ȃ���Δ�΂�
		if (!enemyBullet[i]->GetIsAlive()) { continue; }

		//�X�V����
		enemyBullet[i]->Update();

		//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
		XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
		float bulletSize = enemyBullet[i]->GetScale().x;
		//�v���C���[�ƓG�̒e�̓����蔻��
		//�v���C���[���_���[�W��ԂłȂ��Ȃ�
		if (!player->GetIsDamege())
		{
			//�v���C���[�ƓG�̒e�̓����蔻��
			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, playerPos, playerSize);

			//�v���C���[�ƓG�̒e���Փˏ��
			if (isCollision)
			{
				//�G�̒e�͎��S
				enemyBullet[i]->Dead();
				//�v���C���[�̓_���[�W����炤
				player->Damage();
			}
		}

		//�Ռ��g�ƓG�̒e�̓����蔻��
		for (int j = 0; j < shockWaveNum; j++)
		{
			//�Ռ��g�����ˏ��
			if (shockWave[j]->GetIsAlive())
			{
				//�����蔻��p�ϐ�
				XMFLOAT3 wavePos = shockWave[j]->GetPosition();
				float waveSize = shockWave[j]->GetRadius();

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					wavePos, waveSize, bulletPos, bulletSize);

				//�Ռ��g�ƓG�̒e���Փˏ��
				if (isCollision)
				{
					//�G�̒e������
					enemyBullet[i]->Dead();
				}
			}
		}
	}

	//�K�����폜
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end();)
	{
		//�폜�t���O��true�Ȃ�폜
		if ((*itrGaruEnemy)->GetIsDelete())
		{
			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrGaruEnemy);
			itrGaruEnemy = garuEnemys.erase(itrGaruEnemy);
			continue;
		}
		//for������
		itrGaruEnemy++;
	}


	if (frame->GetHP() == 30) { DebugText::GetInstance()->Print("WALL HP:30", 100, 500); }
	else if (frame->GetHP() == 20) { DebugText::GetInstance()->Print("WALL HP:20", 100, 500); }
	else if (frame->GetHP() == 10) { DebugText::GetInstance()->Print("WALL HP:10", 100, 500); }
	else if (frame->GetHP() <= 0) { DebugText::GetInstance()->Print("WALL BREAK", 100, 500); }

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 100, 300);
	DebugText::GetInstance()->Print("RB:ShockWave", 100, 350);
	DebugText::GetInstance()->Print("RT:SpawnEnemy", 100, 400);

	//�G�t�F�N�g�̍X�V
	effects->Update(camera);

	//�w�i�X�V
	buckGround->Update();

	//�R���{�X�V
	if (Xinput->TriggerButton(XInputManager::PAD_START)) {
		combo->AddCombo();
	}
	combo->Update();

	//�������ԍX�V
	timeLimit->Update();

	//�J�����X�V
	CameraUpdate(camera);

	input = nullptr;
}

void GameScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	{
		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//��ʘg�`��
		frame->Draw();

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

		//�G�̒e�`��
		for (int i = 0; i < enemyBulletNum; i++)
		{
			enemyBullet[i]->Draw();
		}
		//�K�����`��
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			(*itrGaruEnemy)->Draw();
		}

		//�w�i
		buckGround->Draw();

		Object3d::PostDraw();


		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//�R���{�`��
		combo->Draw();

		//�������ԕ`��
		timeLimit->Draw();

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

	//�J��������������
	cameraPos = { 0, 0, -200 };
	//�J���������C�[�W���O�J�n������
	cameraDisEaseStart = 0;
	//�J���������C�[�W���O�I��������
	cameraDisEaseEnd = 0;
	//�J����������ύX���ł͂Ȃ�
	isChangecameraDis = false;
	//�J���������C�[�W���O�^�C�}�[������
	cameraDisEaseTimer = 0;
	//��ʃV�F�C�N���Ȃ�
	isShake = false;
	//��ʃV�F�C�N���ԏ�����
	ShakeTime = 0;

	//��ʘg������
	frame->Reset();
	//�g�̃��C���ύX�ɍ��킹�ăI�u�W�F�N�g�̗l�X�ȋ��E�����ύX
	XMFLOAT2 frameLine = frame->GetFrameLine();
	Player::SetMoveRange(frameLine);
	EnemyBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
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
	if (combo->GetCombo() <= 5) { return; }
	//�R���{���ɉ����ċ���Ռ��g�̈З͂�ύX
	int shockWavePowerLevel = 0;
	if (combo->GetCombo() <= 10) { shockWavePowerLevel = 1; }
	else if (combo->GetCombo() <= 15) { shockWavePowerLevel = 2; }
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

void GameScene::SpawnGaruEnemy()
{
	//�������ɃX�|�[�����W�ƈړ���̍��W�����߂�
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	//�����_������
	spawnPos.x = (float)(rand() % 300 - 150);
	spawnPos.y = 100;
	stayPos.x = (float)(rand() % 300 - 150);
	stayPos.y = (float)(rand() % 180 - 90);

	//20%�̊m���ŃK���^�^�@80%�̊m���ŃK���^�𐶐�
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
	//���ˈʒu�͓G�̒��S
	XMFLOAT3 startPos = garuEnemy->GetPosition();

	//��������O���[�v���K���^�̏ꍇ�A�v���C���[�_���e�𔭎�
	if (garuEnemy->GetGroup() == GaruEnemy::GaruGroup::Garuta)
	{
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//�e�����˂���Ă������΂�
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//�W�I�̍��W�i�v���C���[�̍��W�j���Z�b�g
			XMFLOAT3 targetPos = player->GetPosition();

			//�^�[�Q�b�g�Ɍ����Ēe����
			enemyBullet[i]->AimBulletStart(startPos, targetPos);

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
	}
	//��������O���[�v���K���^�^�̏ꍇ�A4�����ɒe���΂�
	else if (garuEnemy->GetGroup() == GaruEnemy::GaruGroup::Garutata)
	{
		//����̒e����
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//���˂���Ă������΂�
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//����Ɋp�x��ݒ�
			float angle = 45;

			//����ɒe����
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
		//�����̒e����
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//���˂���Ă������΂�
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//�����Ɋp�x��ݒ�
			float angle = 135;

			//�����ɒe����
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
		//�E���̒e����
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//���˂���Ă������΂�
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//�E���Ɋp�x��ݒ�
			float angle = 225;

			//�E���ɒe����
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
		//�E��̒e����
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//���˂���Ă������΂�
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//�E��Ɋp�x��ݒ�
			float angle = 315;

			//�E��ɒe����
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
	}
}

void GameScene::CameraUpdate(Camera* camera)
{
	//�J���������ύX
	if (isChangecameraDis)
	{
		//�J���������ύX���s������
		const int changeTime = 100;

		//�J���������ύX�^�C�}�[�X�V
		cameraDisEaseTimer++;

		//�C�[�W���O�v�Z�p�̎���
		float easeTimer = (float)cameraDisEaseTimer / changeTime;

		//�C�[�W���O�ŃT�C�Y�ύX
		float newDistance = Easing::InQuint(cameraDisEaseStart, cameraDisEaseEnd, easeTimer);

		//�J�����������X�V
		cameraPos.z = newDistance;

		//�^�C�}�[���w�肵�����ԂɂȂ�����
		if (cameraDisEaseTimer >= changeTime)
		{
			//�T�C�Y�ύX��ԏI��
			isChangecameraDis = false;
		}
	}

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

void GameScene::SetChangeCameraDistance(float distance)
{
	//�ύX�O�̃J�����������Z�b�g
	cameraDisEaseStart = cameraPos.z;
	//�ύX��̃J�����������Z�b�g
	cameraDisEaseEnd = distance;
	//�J���������ύX�^�C�}�[��������
	cameraDisEaseTimer = 0;
	//�T�C�Y��ύX���ɂ���
	isChangecameraDis = true;
}