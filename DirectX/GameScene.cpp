#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

#include "Straighter.h"
#include "Division.h"
#include "Releaser.h"

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
	buckGround->AllDelete();
	safe_delete(buckGround);

	//�R���{���
	safe_delete(combo);
	//�������ԉ��
	safe_delete(timeLimit);
	//�X�R�A���
	safe_delete(breakScore);
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
	initialCircleSquareModel = Model::CreateFromOBJ("initialcirclesquare");//�Œ�G�̊O�ǂ̃��f��
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
	Sprite::LoadTexture(4, L"Resources/break.png");
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

	//�ǐ���
	wall = Wall::Create(frameModel);
	XMFLOAT2 wallline = wall->GetWallLine();
	Player::SetMoveRange(wallline);
	PlayerBullet::SetDeadPos(wallline);

	//�G�t�F�N�g������
	effects = new StageEffect();
	effects->Initialize();

	//�w�i�̏�����
	buckGround->Create(hexagonModel);

	//�R���{����
	combo = Combo::Create(2, 3);
	//�������Ԑ���
	timeLimit = TimeLimit::Create(2);
	//�X�R�A����
	breakScore = BreakScore::Create(2, 4);

	//�T�E���h�p
	audio = new Audio();
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

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

	//�G����
	if (input->TriggerKey(DIK_1) || Xinput->TriggerButton(XInputManager::PAD_RT))
	{
		//���i�G�X�|�[��
		SpawnStraighter();
	}
	if (input->TriggerKey(DIK_2) || Xinput->TriggerButton(XInputManager::PAD_LT))
	{
		//�����G�X�|�[��
		SpawnDivision();
	}
	if (input->TriggerKey(DIK_3) || Xinput->TriggerButton(XInputManager::PAD_LB))
	{
		//���o�G�X�|�[��
		SpawnReleaser();
	}

	//�G�X�V
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
			//�m�b�N�o�b�N���̓G�̂ݕǂƂ̔�������
			if ((*itrEnemy)->GetIsKnockBack())
			{
				//�ǂɓ�����������������
				if ((*itrEnemy)->IsCollisionWall(wall->GetWallLine()))
				{
					//���S
					(*itrEnemy)->Dead();

					//�ǂɂ��_���[�W��^����
					int damagePower = (*itrEnemy)->GetPower();
					wall->Damage(damagePower);

					//�R���{�𑝂₷
					combo->AddCombo();
				}
			}
		}
		//�ǂ��Ȃ��ꍇ
		else
		{
			//��ʊO�ɏo����폜����
			XMFLOAT3 pos = (*itrEnemy)->GetPosition();
			XMFLOAT2 wallLine = wall->GetWallLine();
			if (pos.x <= -wallLine.x || pos.x >= wallLine.x ||
				pos.y <= -wallLine.y || pos.y >= wallLine.y)
			{
				(*itrEnemy)->SetDelete();
			}
		}

		//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
		XMFLOAT3 enemyPos = (*itrEnemy)->GetPosition();
		float enemySize = (*itrEnemy)->GetScale().x;

		//�v���C���[�ƓG�̓����蔻��
		//�v���C���[���_���[�W��ԂłȂ��Ȃ�
		if (!player->GetIsDamege())
		{
			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//�G�ƃv���C���[���Փˏ��
			if (isCollision)
			{
				//�v���C���[�̓_���[�W����炤
				player->Damage();
				player->SetKnockback();

				//�G�����S
				(*itrEnemy)->SetDelete();

				//�R���{�I��
				combo->LostCombo();

				isShake = true;
			}
		}

		//�Ռ��g�ƓG�̓����蔻��
		for (int i = 0; i < shockWaveNum; i++)
		{
			//�Ռ��g�����ˏ�ԂłȂ���Δ�΂�
			if (!shockWave[i]->GetIsAlive()) { continue; }

			//�����蔻��p�ϐ�
			XMFLOAT3 wavePos = shockWave[i]->GetPosition();
			float waveSize = shockWave[i]->GetRadius();

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, enemyPos, enemySize);

			//�Ռ��g�ƓG���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��_���[�W��H�炢�����Ă��܂�����)
			if (shockWave[i]->IsKnowEnemy((*itrEnemy))) { continue; }

			//�G���m�b�N�o�b�N�Ŕ�΂�
			float angle = atan2f(enemyPos.y - wavePos.y, enemyPos.x - wavePos.x);
			int powerLevel = shockWave[i]->GetPowerLevel();
			(*itrEnemy)->SetKnockBack(angle, powerLevel);

			//�R���{�𑝂₷
			combo->AddCombo();
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

	//�ǍX�V
	wall->Update();
	if (wall->GetTriggerBreak())
	{
		breakScore->AddScore();
	}

	{
		if (wall->GetHP() == 20) { DebugText::GetInstance()->Print("WALL HP:20", 100, 500); }
		else if (wall->GetHP() == 19) { DebugText::GetInstance()->Print("WALL HP:19", 100, 500); }
		else if (wall->GetHP() == 18) { DebugText::GetInstance()->Print("WALL HP:18", 100, 500); }
		else if (wall->GetHP() == 17) { DebugText::GetInstance()->Print("WALL HP:17", 100, 500); }
		else if (wall->GetHP() == 16) { DebugText::GetInstance()->Print("WALL HP:16", 100, 500); }
		else if (wall->GetHP() == 15) { DebugText::GetInstance()->Print("WALL HP:15", 100, 500); }
		else if (wall->GetHP() == 14) { DebugText::GetInstance()->Print("WALL HP:14", 100, 500); }
		else if (wall->GetHP() == 13) { DebugText::GetInstance()->Print("WALL HP:13", 100, 500); }
		else if (wall->GetHP() == 12) { DebugText::GetInstance()->Print("WALL HP:12", 100, 500); }
		else if (wall->GetHP() == 11) { DebugText::GetInstance()->Print("WALL HP:11", 100, 500); }
		else if (wall->GetHP() == 10) { DebugText::GetInstance()->Print("WALL HP:10", 100, 500); }
		else if (wall->GetHP() == 9) { DebugText::GetInstance()->Print("WALL HP:9", 100, 500); }
		else if (wall->GetHP() == 8) { DebugText::GetInstance()->Print("WALL HP:8", 100, 500); }
		else if (wall->GetHP() == 7) { DebugText::GetInstance()->Print("WALL HP:7", 100, 500); }
		else if (wall->GetHP() == 6) { DebugText::GetInstance()->Print("WALL HP:6", 100, 500); }
		else if (wall->GetHP() == 5) { DebugText::GetInstance()->Print("WALL HP:5", 100, 500); }
		else if (wall->GetHP() == 4) { DebugText::GetInstance()->Print("WALL HP:4", 100, 500); }
		else if (wall->GetHP() == 3) { DebugText::GetInstance()->Print("WALL HP:3", 100, 500); }
		else if (wall->GetHP() == 2) { DebugText::GetInstance()->Print("WALL HP:2", 100, 500); }
		else if (wall->GetHP() == 1) { DebugText::GetInstance()->Print("WALL HP:1", 100, 500); }
		else if (wall->GetHP() <= 0) { DebugText::GetInstance()->Print("WALL BREAK", 100, 500); }
	}

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 100, 250);
	DebugText::GetInstance()->Print("RB:ShockWave", 100, 300);
	DebugText::GetInstance()->Print("RT:SpawnStraighter", 100, 350);
	DebugText::GetInstance()->Print("LT:SpawnDivision", 100, 400);
	DebugText::GetInstance()->Print("LB:SpawnReleaser", 100, 450);

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
	//�X�R�A�X�V
	breakScore->Update();

	//�J�����X�V
	CameraUpdate(camera);

	input = nullptr;
}

void GameScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	{
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

		//�G�`��
		for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
		{
			(*itrEnemy)->Draw();
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
		//�X�R�A�`��
		breakScore->Draw();

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

	//�ǂ̃��C���ύX�ɍ��킹�ăI�u�W�F�N�g�̗l�X�ȋ��E�����ύX
	XMFLOAT2 wallLine = wall->GetWallLine();
	Player::SetMoveRange(wallLine);
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

void GameScene::SpawnStraighter()
{
	//�������ɏ������W�ƈړ����������߂�
	XMFLOAT3 startPos = {};
	float angle = 0;

	XMFLOAT2 startLine = wall->GetWallLine();
	startLine.x += 5;
	startLine.y += 5;

	//4�p�^�[���̃����_���ŏ������W�ƈړ��������Z�b�g
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, -startLine.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLine.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLine.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { -startLine.x, 0, 0 }; angle = 300; }


	enemys.push_back(Straighter::Create(portaModel, startPos, angle));
}

void GameScene::SpawnDivision()
{
	//�������ɏ������W�ƈړ����������߂�
	XMFLOAT3 startPos = {};
	float angle = 0;

	XMFLOAT2 startLine = wall->GetWallLine();
	startLine.x += 5;
	startLine.y += 5;

	//4�p�^�[���̃����_���ŏ������W�ƈړ��������Z�b�g
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, -startLine.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLine.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLine.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { -startLine.x, 0, 0 }; angle = 300; }

	//�����G�𐶐�
	enemys.push_back(Division::Create(portaModel, startPos, angle));
}

void GameScene::SpawnReleaser()
{
	//�������ɏ������W�ƈړ����������߂�
	XMFLOAT3 startPos = {};
	float angle = 0;

	XMFLOAT2 startLine = wall->GetWallLine();
	startLine.x += 5;
	startLine.y += 5;

	//4�p�^�[���̃����_���ŏ������W�ƈړ��������Z�b�g
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, -startLine.y, 0 }; angle = 30; }
	else if (posAngleRand == 1) { startPos = { startLine.x, 0, 0 }; angle = 120; }
	else if (posAngleRand == 2) { startPos = { 0, startLine.y, 0 }; angle = 210; }
	else if (posAngleRand == 3) { startPos = { -startLine.x, 0, 0 }; angle = 300; }

	//��~�n�_�������_������
	XMFLOAT3 stayPos = {};
	stayPos.x = (float)(rand() % 200 - 100);
	stayPos.y = (float)(rand() % 120 - 60);

	//���o�I�𐶐�
	enemys.push_back(Releaser::Create(portaModel, startPos, stayPos));
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
			enemys.push_back(Straighter::Create(portaModel, startPos, angle, parantKnockBackPowerLevel));
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
		enemys.push_back(Straighter::Create(portaModel, startPos, angle, parantKnockBackPowerLevel));
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