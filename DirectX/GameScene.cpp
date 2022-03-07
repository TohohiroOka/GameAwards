#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"


const float radian = XM_PI / 180.0f;//���W�A��

GameScene::~GameScene()
{
	//new�I�u�W�F�N�g�̔j��
	safe_delete(audio);
	safe_delete(light);
	safe_delete(sprite);

	//���f�����
	safe_delete(playerModel);

	//�v���C���[���
	safe_delete(player);

	//�e���
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}
	//�G���
	for (int i = 0; i < enemyNum; i++)
	{
		safe_delete(enemy[i]);
	}
	//���񂾓G�̈ʒu���
	for (auto itr = deadEnemyPoints.begin(); itr != deadEnemyPoints.end(); itr++)
	{
		safe_delete(*itr);
	}
	//���񂾓G�̈ʒu�̃��X�g���
	deadEnemyPoints.clear();

	//�p���[�A�b�v�����
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		safe_delete(*itr);
	}
	//�p���[�A�b�v���̃��X�g���
	powerUpLines.clear();
}

void GameScene::Initialize(Input *input, Camera *camera)
{
	// nullptr�`�F�b�N
	assert(input);
	assert(camera);
	this->input = input;

	//���C�g
	light = LightGroup::Create();
	light->DefaultLightSetting();
	Object3d::SetLightGroup(light);

	playerModel = Model::CreateFromOBJ("uma");

	//�v���C���[����
	player = Player::Create(playerModel);

	//�e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(playerModel);
	}

	//�G����
	/*XMFLOAT3 pos = { 10, 30, 0 };
	for (int i = 0; i < enemyNum; i++)
	{
		pos.x += 15;
	}*/
	enemy[0] = Zakorin::Create(playerModel, { 0, 20, 0 });
	enemy[1] = Zakorin::Create(playerModel, { 10, 20, 0 });
	enemy[2] = Zakorin::Create(playerModel, { 0, 30, 0 });


	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/amm.jpg");

	//�X�v���C�g����
	sprite = Sprite::Create(1);
	sprite->SetPosition({ 100, 100 });
	sprite->SetSize({ 100, 100 });
	sprite->SetTexSize({ 1020, 965 });

	//�f�o�b�O�e�L�X�g����
	DebugText::GetInstance()->Initialize(0);

	//�T�E���h�p
	audio = new Audio();
}

void GameScene::Update(Camera *camera)
{
	//�v���C���[�X�V
	player->Update();

	//�f�o�b�O�p���̐F
		for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
		{
			(*itr)->SetColor({ 1, 1, 1, 1 });
		}

	//�e����
	if (input->TriggerKey(DIK_SPACE) || input->TriggerGamePadButton(Input::PAD_RB))
	{
		for (int i = 0; i < playerBulletNum; i++)
		{
			//���˂���Ă��Ȃ��e��T��
			if (!playerBullet[i]->GetIsAlive())
			{
				//�v���C���[�̍��W�Ɗp�x��e������
				XMFLOAT3 pos = player->GetPosition();
				XMFLOAT3 rota = player->GetRotation();

				//�e����
				playerBullet[i]->BulletStart(pos, rota);

				//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
				break;
			}
		}
	}
	//�e�X�V
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Update();

		//�e�������Ă��Ȃ���Δ�΂�
		if (!playerBullet[i]->GetIsAlive()) continue;

		//�e�ƃp���[�A�b�v���̓����蔻��
		for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
		{
			//�Փ˗p�ɒe�̍��W�Ɣ��a�A���̎n�_�ƏI�_���؂��
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletRadius = playerBullet[i]->GetScale().x * 10;
			XMFLOAT3 lineStartPoint = (*itr)->GetStartPoint();
			XMFLOAT3 lineEndPoint = (*itr)->GetEndPoint();

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Line(
				bulletPos, bulletRadius, lineStartPoint, lineEndPoint);

			//�e�Ɛ����Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) continue;

			//�f�o�b�O�p���̐F�ύX
			(*itr)->SetColor({ 1, 0, 0, 1 });
			playerBullet[i]->PowerUp();
		}
	}

	//1�t���[���O�̎��񂾓G�̐����X�V
	oldDeadPointNum = deadEnemyPoints.size();
	//�G�X�V
	for (int j = 0; j < enemyNum; j++)
	{
		enemy[j]->Update();

		//�G�������Ă��Ȃ���Δ�΂�
		if (!enemy[j]->GetIsAlive()) continue;

		//�e�ƓG�̓����蔻��
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�e�����ˏ�ԂłȂ���Δ�΂�
			if (!playerBullet[i]->GetIsAlive()) continue;

			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetScale().x * 10;
			XMFLOAT3 enemyPos = enemy[j]->GetPosition();
			float enemySize = enemy[j]->GetScale().x * 10;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//�G�ƒe���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) continue;

			//�e�͎��S
			playerBullet[i]->Dead();

			//�G�̓_���[�W����炤
			int bulletPower = playerBullet[i]->GetPower();
			enemy[j]->Damage(bulletPower);

			//�_���[�W�������Ă�HP���c���Ă������΂�
			if (enemy[j]->GetHP() > 0) continue;

			//HP��0�ȉ��ɂȂ����玀�S
			enemy[j]->Dead();

			//���񂾓G�̈ʒu�𑝂₷
			XMFLOAT3 deadPoint = enemy[j]->GetPosition();
			float radius = enemy[j]->GetScale().x * 2;
			deadEnemyPoints.push_back(
				DeadEnemyPoint::Create(playerModel, deadPoint, radius));
		}
	}

	//���񂾓G�̈ʒu�X�V
	for (auto itr = deadEnemyPoints.begin(); itr != deadEnemyPoints.end(); itr++)
	{
		(*itr)->Update();
	}

	//���񂾓G�̐���1�t���[���O��葽����ΐV����������邩����
	if (oldDeadPointNum < deadEnemyPoints.size())
	{
		//���X�g��(end - 1)�̗v�f��T��
		std::list<DeadEnemyPoint *>::iterator newPointIterator;
		newPointIterator = deadEnemyPoints.end();
		newPointIterator--;

		for (auto itr = deadEnemyPoints.begin(); itr != deadEnemyPoints.end(); itr++)
		{
			//���[�v���A(end - 1)�̗v�f�͔��肵�Ȃ�(start��end�������ʒu�̐����o���Ă��܂�����)
			if (itr == newPointIterator) continue;

			//�Փ˗p�ɐV�����~�Ɗ����̉~�̍��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 newDeadPoint = (*newPointIterator)->GetPosition();
			float newDeadRadius = (*newPointIterator)->GetRadius() * 10;
			XMFLOAT3 existingDeadPoint = (*itr)->GetPosition();
			float existingDeadRadius = (*itr)->GetRadius() * 10;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				newDeadPoint, newDeadRadius, existingDeadPoint, existingDeadRadius);

			//�V�����~�Ɗ����̉~���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) continue;

			//�V�����~�Ɗ����̉~���q���V�����������
			powerUpLines.push_back(PowerUpLine::Create(
				newDeadPoint, existingDeadPoint));
		}
	}

	//�p���[�A�b�v���X�V
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		(*itr)->Update(camera);
	}

	//�J�����Z�b�g
	camera->UpdateTps({ 0,0,-100 });

	//�X�v���C�g�X�V
	sprite->Update();
	//�f�o�b�O�e�L�X�g
	if (deadEnemyPoints.size() == 1)
	{
		DebugText::GetInstance()->Print("DEADNUM : 1", 100, 500);
	}
	else if (deadEnemyPoints.size() == 2)
	{
		DebugText::GetInstance()->Print("DEADNUM : 2", 100, 500);
	}
	else if (deadEnemyPoints.size() == 3)
	{
		DebugText::GetInstance()->Print("DEADNUM : 3", 100, 500);
	}

	if (playerBullet[0]->GetPower() >= 20)
	{
		DebugText::GetInstance()->Print("POWER : 20", 100, 550);
	}

	DebugText::GetInstance()->Print("PlayerMove:LSTICK", 1000, 100);
	DebugText::GetInstance()->Print("BulletAngle:RSTICK", 1000, 150);
	DebugText::GetInstance()->Print("BulletShot:RB", 1000, 200);
}

void GameScene::Draw(ID3D12GraphicsCommandList *cmdList)
{
	//�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	//���񂾓G�̈ʒu�`��
	for (auto itr = deadEnemyPoints.begin(); itr != deadEnemyPoints.end(); itr++)
	{
		(*itr)->Draw();
	}
	//�e�`��
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Draw();
	}
	//�G�`��
	for (int i = 0; i < enemyNum; i++)
	{
		enemy[i]->Draw();
	}

	//�v���C���[�`��
	player->Draw();

	Object3d::PostDraw();


	//�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	sprite->Draw();


	//�f�o�b�O�e�L�X�g�`��
	DebugText::GetInstance()->DrawAll(cmdList);
	Sprite::PostDraw();

	//��3d
	DrawLine3D::PreDraw(cmdList);

	//�p���[�A�b�v���`��
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		(*itr)->Draw();
	}

	DrawLine3D::PostDraw();
}
