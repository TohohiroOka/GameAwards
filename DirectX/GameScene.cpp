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
	safe_delete(circleModel);

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

	circleModel = Model::CreateFromOBJ("circle");

	//�v���C���[����
	player = Player::Create(circleModel);

	//�e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(circleModel);
	}

	//�G����
	enemy[0] = Zakorin::Create(circleModel, { 0, 0, 0 });
	enemy[1] = Zakorin::Create(circleModel, { 0, 10, 0 });
	enemy[2] = Zakorin::Create(circleModel, { 0, 20, 0 });
	enemy[3] = Zakorin::Create(circleModel, { 10, 0, 0 });
	enemy[4] = Zakorin::Create(circleModel, { 10, 10, 0 });
	enemy[5] = Zakorin::Create(circleModel, { 10, 20, 0 });
	enemy[6] = Zakorin::Create(circleModel, { 20, 0, 0 });
	enemy[7] = Zakorin::Create(circleModel, { 20, 10, 0 });
	enemy[8] = Zakorin::Create(circleModel, { 20, 20, 0 });


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
		(*itr)->SetColor({ 0.4f, 1, 0.2f, 1 });
	}

	//�e����
	if (input->TriggerKey(DIK_SPACE) || input->TriggerGamePadButton(Input::PAD_RB))
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

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
	}

	//�e�X�V
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Update();

		//�e�������Ă��Ȃ���Δ�΂�
		if (!playerBullet[i]->GetIsAlive()) { continue; }

		//�e�ƃp���[�A�b�v���̓����蔻��
		for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
		{
			//�Փ˗p�ɒe�̍��W�Ɣ��a�A���̎n�_�ƏI�_���؂��
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletRadius = playerBullet[i]->GetScale().x;
			XMFLOAT3 lineStartPoint = (*itr)->GetStartPoint();
			XMFLOAT3 lineEndPoint = (*itr)->GetEndPoint();

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Line(
				bulletPos, bulletRadius, lineStartPoint, lineEndPoint);

			//�e�Ɛ����Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//�f�o�b�O�p���̐F�ύX
			(*itr)->SetColor({ 1, 0, 0, 1 });

			//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��p���[�A�b�v�������Ă��܂�����)
			if (playerBullet[i]->IsKnowLine((*itr))) { continue; }

			//�e���p���[�A�b�v������
			playerBullet[i]->PowerUp();
		}
	}

	//�G�X�V
	for (int j = 0; j < enemyNum; j++)
	{
		//���݂��Ȃ��������΂�
		if (!enemy[j]->GetIsExistence()) { continue; }

		//�X�V����
		enemy[j]->Update();

		//�m�b�N�o�b�N���I���A���݂��Ȃ��Ȃ�����
		if (!enemy[j]->GetIsExistence())
		{
			//���񂾓G�̈ʒu�𑝂₷
			XMFLOAT3 deadPoint = enemy[j]->GetPosition();

			//���񂾓G�̉~�̔��a���Z�b�g����i �G�̑傫���~�i �|���ꂽ���̒e�̋��� / 5 �j�j
			float radius = enemy[j]->GetScale().x * ((float)enemy[j]->GetKillBulletPower() / 5);
			deadEnemyPoints.push_back(
				DeadEnemyPoint::Create(circleModel, deadPoint, radius));

			//�G�̑��݂��Ȃ��Ȃ����̂Ŕ�΂�
			continue;
		}

		//�G�������Ă��Ȃ���Δ�΂�
		if (!enemy[j]->GetIsAlive()) { continue; }

		//�e�ƓG�̓����蔻��
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�e�����ˏ�ԂłȂ���Δ�΂�
			if (!playerBullet[i]->GetIsAlive()) { continue; }

			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetScale().x;
			XMFLOAT3 enemyPos = enemy[j]->GetPosition();
			float enemySize = enemy[j]->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//�G�ƒe���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//�e�͎��S
			playerBullet[i]->Dead();

			//�G�̓_���[�W����炤
			int bulletPower = playerBullet[i]->GetPower();
			enemy[j]->Damage(bulletPower);

			//�_���[�W�������Ă�HP���c���Ă������΂�
			if (enemy[j]->GetHP() > 0) { continue; }

			//�G��HP��0�ȉ��Ȃ̂Ńm�b�N�o�b�N���J�n����
			float bulletAngle = playerBullet[i]->GetAngle();
			enemy[j]->SetKnockBack(bulletAngle, bulletPower);
		}
	}

	//���񂾓G�̈ʒu�X�V
	for (auto itr = deadEnemyPoints.begin(); itr != deadEnemyPoints.end(); itr++)
	{
		//�T�C�Y�ύX��ԂłȂ��ꍇ�͔�΂�
		if (!(*itr)->GetIsChangeRadius()) { continue; }

		//�X�V
		(*itr)->Update();

		//�Փ˂𔻒肵�ăp���[�A�b�v�����쐬
		for (auto itr2 = deadEnemyPoints.begin(); itr2 != deadEnemyPoints.end(); itr2++)
		{
			CreatePowerUpLine(*itr, *itr2);
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

	if (powerUpLines.size() == 0) { DebugText::GetInstance()->Print("LINE : 0", 100, 600); }
	else if (powerUpLines.size() == 1) { DebugText::GetInstance()->Print("LINE : 1", 100, 600); }
	else if (powerUpLines.size() == 2) { DebugText::GetInstance()->Print("LINE : 2", 100, 600); }
	else if (powerUpLines.size() == 3) { DebugText::GetInstance()->Print("LINE : 3", 100, 600); }
	else if (powerUpLines.size() == 4) { DebugText::GetInstance()->Print("LINE : 4", 100, 600); }
	else if (powerUpLines.size() == 5) { DebugText::GetInstance()->Print("LINE : 5", 100, 600); }
	else if (powerUpLines.size() == 6) { DebugText::GetInstance()->Print("LINE : 6", 100, 600); }
	else if (powerUpLines.size() == 7) { DebugText::GetInstance()->Print("LINE : 7", 100, 600); }
	else if (powerUpLines.size() == 8) { DebugText::GetInstance()->Print("LINE : 8", 100, 600); }
	else if (powerUpLines.size() == 9) { DebugText::GetInstance()->Print("LINE : 9", 100, 600); }

	DebugText::GetInstance()->Print("PlayerMove:LSTICK", 1000, 100);
	DebugText::GetInstance()->Print("BulletAngle:RSTICK", 1000, 150);
	DebugText::GetInstance()->Print("BulletShot:RB", 1000, 200);
}

void GameScene::Draw(ID3D12GraphicsCommandList *cmdList)
{
	//�X�v���C�g�w�ʕ`��
	Sprite::PreDraw(cmdList);

	sprite->Draw();
	Sprite::PostDraw();

	//��3d
	DrawLine3D::PreDraw(cmdList);

	//�p���[�A�b�v���`��
	for (auto itr = powerUpLines.begin(); itr != powerUpLines.end(); itr++)
	{
		(*itr)->Draw();
	}

	DrawLine3D::PostDraw();

	//�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	//�v���C���[�`��
	player->Draw();

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
	//���񂾓G�̈ʒu�`��
	for (auto itr = deadEnemyPoints.begin(); itr != deadEnemyPoints.end(); itr++)
	{
		(*itr)->Draw();
	}

	Object3d::PostDraw();


	//�X�v���C�g�O�ʕ`��
	Sprite::PreDraw(cmdList);

	//sprite->Draw();


	//�f�o�b�O�e�L�X�g�`��
	DebugText::GetInstance()->DrawAll(cmdList);
	Sprite::PostDraw();
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
