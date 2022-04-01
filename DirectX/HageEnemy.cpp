#include "HageEnemy.h"
#include "SafeDelete.h"

DirectX::XMFLOAT2 HageEnemy::bulletShotRange = { 110, 60 };

HageEnemy::~HageEnemy()
{
	safe_delete(enemyObject);
}

void HageEnemy::Update()
{
	//�ړ�����
	Move();

	//�e���ˏ���
	ShotBullet();

	//�G�����S���Ă�����폜��Ԃɂ���
	if (!isAlive)
	{
		SetDelete();
	}

	//�G�I�u�W�F�N�g�X�V
	enemyObject->Update();
}

void HageEnemy::Draw()
{
	//�G�I�u�W�F�N�g�`��
	enemyObject->Draw();
}

void HageEnemy::Damage(int damagePower)
{
	//�����Ŏw�肵�������̕�HP�����炷
	HP -= damagePower;
}

void HageEnemy::Dead()
{
	//���S��Ԃɂ���
	isAlive = false;
}

void HageEnemy::SetDelete()
{
	//�폜����
	isDelete = true;
}

void HageEnemy::Move()
{
	//�ړ����x�Ɉړ��p�x����Z���č��W���X�V
	float moveSpeed = 1.6f;
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x += moveSpeed * cosf(moveAngle);
	pos.y += moveSpeed * sinf(moveAngle);

	//�폜���C���܂ŏo���玀�S��Ԃɂ���
	const XMFLOAT2 despawnPos = { 220, 125 };
	if (pos.x >= despawnPos.x || pos.x <= -despawnPos.x || pos.y >= despawnPos.y || pos.y <= -despawnPos.y)
	{
		Dead();
	}

	//�X�V�������W���Z�b�g
	enemyObject->SetPosition(pos);
}

void HageEnemy::ShotBullet()
{
	//�e�𔭎˂���͈͂ɂ��Ȃ���Δ�΂�
	XMFLOAT3 pos = enemyObject->GetPosition();
	if (pos.x >= bulletShotRange.x || pos.x <= -bulletShotRange.x || pos.y >= bulletShotRange.y || pos.y <= -bulletShotRange.y)
	{
		return;
	}

	//�e�͖��t���[�����˂��Ȃ��̂�false�ɖ߂��Ă���
	isBulletShot = false;
	//�e���˃^�C�}�[���X�V����
	bulletShotTimer++;
	//�e���˃^�C�}�[����莞�Ԃ܂ŃJ�E���g���ꂽ��
	const int bulletInterval = 800;
	if (bulletShotTimer >= bulletInterval)
	{
		//�e���˃^�C�}�[������
		bulletShotTimer = 0;

		//�e����
		isBulletShot = true;
	}
}