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
	safe_delete(sprite);
	safe_delete(player);
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}
	for (int i = 0; i < enemyNum; i++)
	{
		safe_delete(enemy[i]);
	}
	safe_delete(line);
	safe_delete(line3d);
}

void GameScene::Initialize(Input *input, Camera *camera)
{
	// nullptr�`�F�b�N
	assert(input);
	this->input = input;

	//��
	line = new DrawLine();
	line = DrawLine::Create();
	line3d = new DrawLine3D();
	line3d = DrawLine3D::Create();

	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/amm.jpg");

	//�v���C���[����
	player = Player::Create(1, { 1280 / 2, 720 / 2 }, { 50, 50 });

	//�e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(1);
	}
	//�G����
	XMFLOAT2 pos = { 0, 0 };
	for (int i = 0; i < enemyNum; i++)
	{
		pos.x += 100;
		enemy[i] = Zakorin::Create(pos);
	}

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
	////��
	line->SetLine({ 300,300 }, { 700,700 }, { 1,1,1,1 }, 50);
	line->Update();
	line3d->SetLine({ 5,5,50 }, { 0,0,50 }, { 1,1,1,1 }, 1);
	line3d->Update(camera);

	//�v���C���[�X�V
	player->Update();

	//�e����
	if (input->TriggerKey(DIK_SPACE) || input->TriggerGamePadButton(Input::PAD_RB))
	{
		for (int i = 0; i < playerBulletNum; i++)
		{
			//���˂���Ă��Ȃ��e��T��
			if (!playerBullet[i]->GetIsAlive())
			{
				//�v���C���[�̍��W�Ɗp�x��e������
				XMFLOAT2 pos = player->GetPosition();
				float rota = player->GetRotation();

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
	}

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

			//�Փ˔�����v�Z
			XMFLOAT2 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetSize().x / 2;
			XMFLOAT2 enemyPos = enemy[j]->GetPosition();
			float enemySize = enemy[j]->GetSize().x / 2;

			bool isCollision = Collision::CircleCollision(
				bulletPos, bulletSize, enemyPos, enemySize);

			//�G�ƒe���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) continue;


			//�e�͎��S
			playerBullet[i]->Dead();

			//�G�̓_���[�W����炤
			int damagePower = playerBullet[i]->GetPower();
			enemy[j]->Damage(damagePower);

			//HP��0�ȉ��ɂȂ����玀�S
			if (enemy[j]->GetHP() <= 0)
			{
				enemy[j]->Dead();
			}
		}
	}


	//�X�v���C�g�X�V
	sprite->Update();
	//�f�o�b�O�e�L�X�g
	if (enemy[1]->GetDeadNum() == 1)
	{
		DebugText::GetInstance()->Print("DEADNUM : 1", 100, 500);
	}
	else if (enemy[1]->GetDeadNum() == 2)
	{
		DebugText::GetInstance()->Print("DEADNUM : 2", 100, 500);
	}

	DebugText::GetInstance()->Print("PlayerMove:LSTICK", 1000, 100);
	DebugText::GetInstance()->Print("BulletAngle:RSTICK", 1000, 150);
	DebugText::GetInstance()->Print("BulletShot:RB", 1000, 200);
}

void GameScene::Draw(ID3D12GraphicsCommandList *cmdList)
{
	//��
	DrawLine::PreDraw(cmdList);
	line->Draw();
	DrawLine::PostDraw();

	//��3d
	DrawLine3D::PreDraw(cmdList);
	line3d->Draw();
	DrawLine3D::PostDraw();

	//�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	//�e�`��
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Draw();
	}
	//�v���C���[�`��
	player->Draw();

	//�G�`��
	for (int i = 0; i < enemyNum; i++)
	{
		enemy[i]->Draw();
	}

	sprite->Draw();


	//�f�o�b�O�e�L�X�g�`��
	DebugText::GetInstance()->DrawAll(cmdList);
	Sprite::PostDraw();
}
