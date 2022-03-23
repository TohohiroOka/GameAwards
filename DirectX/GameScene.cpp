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
	//new�I�u�W�F�N�g�̔j��
	safe_delete(audio);
	safe_delete(light);
	safe_delete(sprite);

	//���f�����
	safe_delete(circleModel);
	safe_delete(playerModel);
	safe_delete(pBullModel);
	safe_delete(enemy01Model);
	safe_delete(enemy02Model);
	safe_delete(eBullModel);
	safe_delete(deadEnemyModel);

	//�v���C���[���
	safe_delete(player);

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
	//�G�̒e���
	for (int i = 0; i < enemyBulletNum; i++)
	{
		safe_delete(enemyBullet[i]);
	}
	//���񂾓G�̈ʒu���
	for (auto itrDeadPoint = deadEnemyPoints.begin(); itrDeadPoint != deadEnemyPoints.end(); itrDeadPoint++)
	{
		safe_delete(*itrDeadPoint);
	}
	//���񂾓G�̈ʒu�̃��X�g���
	deadEnemyPoints.clear();

	//�p���[�A�b�v�����
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		safe_delete(*itrLine);
	}
	//�p���[�A�b�v���̃��X�g���
	powerUpLines.clear();

	//�G�t�F�N�g�̉��
	safe_delete(effects);
}

void GameScene::Initialize(Camera *camera)
{
	// nullptr�`�F�b�N
	assert(camera);

	//���C�g
	light = LightGroup::Create();
	light->DefaultLightSetting();
	Object3d::SetLightGroup(light);

	circleModel = Model::CreateFromOBJ("circle");//�^�o�R�̃��f��
	playerModel = Model::CreateFromOBJ("player");//�v���C���[�̃��f��
	pBullModel = Model::CreateFromOBJ("playerbullet");//�v���C���[�̒e�̃��f��
	enemy01Model = Model::CreateFromOBJ("enemy");//�G01(�K���^)�̃��f��
	enemy02Model = Model::CreateFromOBJ("garutata");//�G02(�K���^�^)�̃��f��
	eBullModel = Model::CreateFromOBJ("enemybullet");//�G�̒e�̃��f��
	deadEnemyModel = Model::CreateFromOBJ("desenemy");//���񂾓G�̃��f��

	//�v���C���[����
	player = Player::Create(playerModel);

	//�e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(pBullModel);
	}

	//�G�̒e����
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/kari.png");

	//�X�v���C�g����
	sprite = Sprite::Create(1);
	sprite->SetPosition({ 100, 100 });
	sprite->SetSize({ 100, 100 });
	sprite->SetTexSize({ 64, 64 });

	//�f�o�b�O�e�L�X�g����
	DebugText::GetInstance()->Initialize(0);

	//�T�E���h�p
	audio = new Audio();

	//�G�t�F�N�g������
	effects = new StageEffect();
	effects->Initialize();
}

void GameScene::Update(Camera *camera)
{
	Input *input = Input::GetInstance();
	XInputManager *Xinput = XInputManager::GetInstance();

	//�v���C���[�X�V
	player->Update(effects);

	//�f�o�b�O�p���̐F
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		(*itr)->SetColor({ 0.4f, 1, 0.2f, 1 });
	}

	//�v���C���[�e����
	bulletShotTimer--;
	if (input->TriggerKey(DIK_SPACE) || Xinput->TriggerButton(XInputManager::PAD_RB))
	{
		for (int i = 0; i < playerBulletNum; i++)
		{
			//���˂���Ă������΂�
			if (playerBullet[i]->GetIsAlive()) { continue; }

			//�v���C���[�̍��W�Ɗp�x��e������
			XMFLOAT3 pos = player->GetPosition();
			XMFLOAT3 rota = player->GetRotation();

			//�e����
			playerBullet[i]->BulletStart(pos, rota);

			//���̒e���˂܂ł̃^�C�}�[������������
			bulletShotTimer = 10;

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
	}

	//�v���C���[�e�X�V
	for (int i = 0; i < playerBulletNum; i++)
	{
		//�e�������Ă��Ȃ���Δ�΂�
		if (!playerBullet[i]->GetIsAlive()) { continue; }

		//�X�V����
		playerBullet[i]->Update();

		//�e�������Ă��Ȃ���Δ�΂�
		if (!playerBullet[i]->GetIsAlive()) { continue; }

		//�e�ƃp���[�A�b�v���̓����蔻��
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			//�Փ˗p�ɒe�̍��W�Ɣ��a�A���̎n�_�ƏI�_���؂��
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletRadius = playerBullet[i]->GetScale().x;
			XMFLOAT3 lineStartPoint = (*itrLine)->GetStartPoint();
			XMFLOAT3 lineEndPoint = (*itrLine)->GetEndPoint();

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Line(
				bulletPos, bulletRadius, lineStartPoint, lineEndPoint);

			//�e�Ɛ����Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//�f�o�b�O�p���̐F�ύX
			(*itrLine)->SetColor({ 1, 0, 0, 1 });

			//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��p���[�A�b�v�������Ă��܂�����)
			if (playerBullet[i]->IsKnowLine((*itrLine))) { continue; }

			//�e���p���[�A�b�v������
			playerBullet[i]->PowerUp();
		}
	}

	//�G����
	if (input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_RT))
	{
		//�G���X�|�[��
		SpawnEnemy();
	}

	//�G�X�V
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		//���݂��Ȃ��������΂�
		if (!(*itrEnemy)->GetIsExistence()) { continue; }

		//�X�V����
		(*itrEnemy)->Update(effects);

		//�m�b�N�o�b�N���I���A���݂��Ȃ��Ȃ�����
		if (!(*itrEnemy)->GetIsExistence())
		{
			//�I�u�W�F�N�g�̃��f����ύX����
			(*itrEnemy)->SetModel(deadEnemyModel);

			//���񂾓G�̈ʒu�𑝂₷
			XMFLOAT3 deadPoint = (*itrEnemy)->GetPosition();

			//���񂾓G�̉~�̔��a���Z�b�g����i �G�̑傫���~�i �|���ꂽ���̒e�̋��� / 4 �j�j
			float radius = (*itrEnemy)->GetScale().x * ((float)(*itrEnemy)->GetKillBulletPower() / 4);
			deadEnemyPoints.push_back(
				DeadEnemyPoint::Create(circleModel, deadPoint, radius));

			//�G�̑��݂��Ȃ��Ȃ����̂Ŕ�΂�
			continue;
		}

		//�G�������Ă��Ȃ���Δ�΂� �G���X�|�[�������������΂� �G�����������������΂�
		if (!(*itrEnemy)->GetIsAlive() || (*itrEnemy)->GetIsDuringSpawn() || (*itrEnemy)->GetIsEscape()) { continue; }

		//�G�e���˃t���O��true�Ȃ�
		if ((*itrEnemy)->GetIsBulletShot())
		{
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//�G�e�����˂���Ă������΂�
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//�G�̍��W�ƕW�I�̍��W�i�v���C���[�̍��W�j��e������
				XMFLOAT3 startPos = (*itrEnemy)->GetPosition();
				XMFLOAT3 targetPos = player->GetPosition();

				//�G�e����
				enemyBullet[i]->BulletStart(startPos, targetPos);

				//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
				break;
			}
		}

		//�v���C���[�ƓG�̓����蔻��
		//�v���C���[���_���[�W��ԂłȂ��Ȃ画�������
		if (!player->GetIsDamege())
		{
			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 enemyPos = (*itrEnemy)->GetPosition();
			float enemySize = (*itrEnemy)->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//�G�ƃv���C���[�e���Փˏ��
			if (isCollision)
			{
				//�v���C���[�̓_���[�W����炤
				player->Damage();
				player->SetKnockback();

				isShake = true;

				//HP��0�Ȃ�
				if (player->GetHP() <= 0)
				{
					//�v���C���[���S
					player->Dead();
				}
			}
		}

		//�e�ƓG�̓����蔻��
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�e�����ˏ�ԂłȂ���Δ�΂�
			if (!playerBullet[i]->GetIsAlive()) { continue; }

			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetScale().x;
			XMFLOAT3 enemyPos = (*itrEnemy)->GetPosition();
			float enemySize = (*itrEnemy)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//�G�ƒe���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//�e�͎��S
			playerBullet[i]->Dead();

			//�G�̓_���[�W����炤
			int bulletPower = playerBullet[i]->GetPower();
			(*itrEnemy)->Damage(bulletPower);

			//�_���[�W�������Ă�HP���c���Ă������΂�
			if ((*itrEnemy)->GetHP() > 0) { continue; }

			//�G��HP��0�ȉ��Ȃ̂Ńm�b�N�o�b�N���J�n����
			float bulletAngle = playerBullet[i]->GetAngle();
			(*itrEnemy)->SetKnockBack(bulletAngle, bulletPower);
		}
	}

	//�G�폜
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//�������������Ă���Ȃ�
		if ((*itrEnemy)->GetIsEscapeCompleted())
		{
			safe_delete(*itrEnemy);
			itrEnemy = enemys.erase(itrEnemy);
			continue;
		}

		itrEnemy++;
	}

	//���񂾓G�̈ʒu�X�V
	for (auto itrDeadPoint = deadEnemyPoints.begin(); itrDeadPoint != deadEnemyPoints.end(); itrDeadPoint++)
	{
		//�T�C�Y�ύX��ԂłȂ��ꍇ�͔�΂�
		if (!(*itrDeadPoint)->GetIsChangeRadius()) { continue; }

		//�X�V
		(*itrDeadPoint)->Update();

		//�Փ˂𔻒肵�ăp���[�A�b�v�����쐬
		for (auto itrDeadPoint2 = deadEnemyPoints.begin(); itrDeadPoint2 != deadEnemyPoints.end(); itrDeadPoint2++)
		{
			CreatePowerUpLine(*itrDeadPoint, *itrDeadPoint2);
		}
	}

	//�G�̒e�X�V
	for (int i = 0; i < enemyBulletNum; i++)
	{
		//�e�������Ă��Ȃ���Δ�΂�
		if (!enemyBullet[i]->GetIsAlive()) { continue; }

		//�X�V����
		enemyBullet[i]->Update();

		//�v���C���[���_���[�W��Ԃ��������΂�
		if (player->GetIsDamege()) { continue; };

		//�v���C���[�ƓG�̒e�̓����蔻��
		//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
		XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
		float bulletSize = enemyBullet[i]->GetScale().x;
		XMFLOAT3 playerPos = player->GetPosition();
		float playerSize = player->GetScale().x;

		//�Փ˔�����v�Z
		bool isCollision = Collision::CheckCircle2Circle(
			bulletPos, bulletSize, playerPos, playerSize);

		//�G�ƃv���C���[�e���Փˏ�ԂłȂ���Δ�΂�
		if (!isCollision) { continue; }

		//�G�e�͎��S
		enemyBullet[i]->Dead();
		//�v���C���[�̓_���[�W����炤
		player->Damage();

		//�_���[�W�������Ă�HP���c���Ă������΂�
		if (player->GetHP() > 0) { continue; }

		//�v���C���[���S
		player->Dead();
	}

	//�p���[�A�b�v���X�V
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		(*itr)->Update(camera);
	}

	//�J�����Z�b�g
	if (isShake)
	{
		camera->CameraShake(5);
		ShakeTime++;
		if (ShakeTime > 10) {
			isShake = false;
			ShakeTime = 0;
		}
	}
	camera->TpsCamera({ 0,0,-100 });
	camera->Update();

	//�X�v���C�g�X�V
	sprite->Update();


	if (player->GetHP() == 3) { DebugText::GetInstance()->Print("HP : 3", 100, 500); }
	else if (player->GetHP() == 2) { DebugText::GetInstance()->Print("HP : 2", 100, 500); }
	else if (player->GetHP() == 1) { DebugText::GetInstance()->Print("HP : 1", 100, 500); }
	else if (player->GetHP() == 0) { DebugText::GetInstance()->Print("HP : 0", 100, 500); }

	if (player->GetIsAlive()) { DebugText::GetInstance()->Print("PLAYER ALIVE", 100, 550); }
	else { DebugText::GetInstance()->Print("PLAYER DEAD", 100, 550); }

	if (enemys.size() == 0) { DebugText::GetInstance()->Print("ENEMY : 0", 100, 600); }
	else if (enemys.size() == 1) { DebugText::GetInstance()->Print("ENEMY : 1", 100, 600); }
	else if (enemys.size() == 2) { DebugText::GetInstance()->Print("ENEMY : 2", 100, 600); }
	else if (enemys.size() == 3) { DebugText::GetInstance()->Print("ENEMY : 3", 100, 600); }
	else if (enemys.size() == 4) { DebugText::GetInstance()->Print("ENEMY : 4", 100, 600); }
	else if (enemys.size() == 5) { DebugText::GetInstance()->Print("ENEMY : 5", 100, 600); }
	else if (enemys.size() == 6) { DebugText::GetInstance()->Print("ENEMY : 6", 100, 600); }
	else if (enemys.size() == 7) { DebugText::GetInstance()->Print("ENEMY : 7", 100, 600); }
	else if (enemys.size() == 8) { DebugText::GetInstance()->Print("ENEMY : 8", 100, 600); }
	else if (enemys.size() == 9) { DebugText::GetInstance()->Print("ENEMY : 9", 100, 600); }

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 1000, 100);
	DebugText::GetInstance()->Print("LB:Sneak", 1000, 150);
	DebugText::GetInstance()->Print("RB:BulletShot", 1000, 200);
	DebugText::GetInstance()->Print("ENTERKEY:CreateEnemy", 1000, 250);

	//�G�t�F�N�g�̍X�V
	effects->Update(camera);
}

void GameScene::Draw(ID3D12GraphicsCommandList *cmdList)
{
	//�X�v���C�g�w�ʕ`��
	//Sprite::PreDraw(cmdList);

	//sprite->Draw();
	//Sprite::PostDraw();

	//�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	//�v���C���[�`��
	player->Draw();

	Object3d::PostDraw();

	//�G�t�F�N�g�̕`��
	effects->Draw(cmdList);

	//��3d
	DrawLine3D::PreDraw(cmdList);

	//�p���[�A�b�v���`��
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		(*itrLine)->Draw();
	}

	DrawLine3D::PostDraw();

	//�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	//�v���C���[�e�`��
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Draw();
	}
	//�G�̒e�`��
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i]->Draw();
	}
	//�G�`��
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		(*itrEnemy)->Draw();
	}
	//���񂾓G�̈ʒu�`��
	for (auto itrDeadPoint = deadEnemyPoints.begin(); itrDeadPoint != deadEnemyPoints.end(); itrDeadPoint++)
	{
		(*itrDeadPoint)->Draw();
	}

	Object3d::PostDraw();


	//�X�v���C�g�O�ʕ`��
	Sprite::PreDraw(cmdList);

	//�f�o�b�O�e�L�X�g�`��
	DebugText::GetInstance()->DrawAll(cmdList);

	Sprite::PostDraw();
}

void GameScene::SpawnEnemy()
{
	////20%�̊m���ŃK���^�^�@80%�̊m���ŃK���^�𐶐�
	//int spawnRand = rand() % 2;

	//if (spawnRand == 0)
	//{
	//	enemys.push_back(Garuta::Create(enemy01Model, { -10, -10, 0 }));
	//	enemys.push_back(Garuta::Create(enemy01Model, { -10, 10, 0 }));
	//	enemys.push_back(Garuta::Create(enemy01Model, { 10, -10, 0 }));
	//	enemys.push_back(Garutata::Create(enemy01Model, { 10, 10, 0 }));
	//}
	//else if (spawnRand == 1)
	//{
	//	enemys.push_back(Garuta::Create(enemy01Model, { -20, -20, 0 }));
	//	enemys.push_back(Garuta::Create(enemy01Model, { -20, 20, 0 }));
	//	enemys.push_back(Garuta::Create(enemy01Model, { 20, -20, 0 }));
	//	enemys.push_back(Garutata::Create(enemy01Model, { 20, 20, 0 }));
	//}

	//�������ɏ������W�ƈړ����������߂�
	XMFLOAT3 startPos = {};

	startPos.x = (float)(rand() % 200 - 100);
	startPos.y = (float)(rand() % 120 - 60);

	////4�p�^�[���̃����_���ŏ������W�ƈړ��������Z�b�g
	//int posAngleRand = rand() % 4;
	//if (posAngleRand == 0) { startPos = { 0, -65, 0 }; }
	//else if (posAngleRand == 1) { startPos = { 115, 0, 0 }; }
	//else if (posAngleRand == 2) { startPos = { 0, 65, 0 }; }
	//else if (posAngleRand == 3) { startPos = { -115, 0, 0 }; }

	//20%�̊m���ŃK���^�^�@80%�̊m���ŃK���^�𐶐�
	int enemyKindRand = rand() % 5;
	if (enemyKindRand == 0)
	{
		enemys.push_back(Garutata::Create(enemy02Model, startPos));
	}
	else
	{
		enemys.push_back(Garuta::Create(enemy01Model, startPos));
	}
}

void GameScene::CreatePowerUpLine(DeadEnemyPoint *startPoint, DeadEnemyPoint *endPoint)
{
	//�n�_�ƏI�_�������ꍇ�͔�΂�(�n�_�ƏI�_�������ʒu�̐����o���Ă��܂�����)
	if (startPoint == endPoint) { return; }

	//���ɐ���������Ă������΂�
	for (auto line = powerUpLines.begin(); line != powerUpLines.end(); line++)
	{
		if ((*line)->IsAlreadyCreateLine(startPoint, endPoint)) { return; }
	}

	//�Փ˗p�ɉ~1�Ɖ~2�̍��W�Ɣ��a�̑傫�����؂��
	XMFLOAT3 deadPoint1 = startPoint->GetPosition();
	float deadRadius1 = startPoint->GetRadius();
	XMFLOAT3 deadPoint2 = endPoint->GetPosition();
	float deadRadius2 = endPoint->GetRadius();

	//�Փ˔�����v�Z
	bool isCollision = Collision::CheckCircle2Circle(
		deadPoint1, deadRadius1, deadPoint2, deadRadius2);

	//�~1�Ɖ~2���Փˏ�ԂłȂ���Δ�΂�
	if (!isCollision) { return; }

	//�Փ˂��Ă���~1�Ɖ~2���q���V�����������
	powerUpLines.push_back(PowerUpLine::Create(
		startPoint, endPoint));

	//�q���ꂽ���̎n�_�ƏI�_�̉~���T�C�Y�ύX��Ԃɂ���
	startPoint->ChangeRadius();
	endPoint->ChangeRadius();
}
