#include "BaseEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"

BaseEnemy::~BaseEnemy()
{
	safe_delete(enemyObject);
}

void BaseEnemy::Update()
{
	//�ʏ펞�̈ړ�
	if (isAlive)
	{
		//�ړ�����
		Move();

		//�e���ˏ���
		ShotBullet();
	}
	//�m�b�N�o�b�N�ł̈ړ�
	else
	{
		KnockBack();
	}

	//�I�u�W�F�N�g�X�V
	enemyObject->Update();
}

void BaseEnemy::Draw()
{
	//�I�u�W�F�N�g�`��
	enemyObject->Draw();
}

void BaseEnemy::Damage(int damagePower)
{
	//�����Ŏw�肵�������̕�HP�����炷
	HP -= damagePower;
}

void BaseEnemy::Dead()
{
	//���S��Ԃɂ���
	isAlive = false;
}

void BaseEnemy::SetKnockBack(float angle, int power)
{
	//�m�b�N�o�b�N�Ɏg�p����p�x�Ƌ������Z�b�g
	this->knockBackAngle = angle;
	this->killBulletPower = power;

	//���S��Ԃɂ���
	Dead();
}

void BaseEnemy::Move()
{
	//�ړ����x�Ɉړ��p�x����Z���č��W���X�V
	float moveSpeed = 0.1f;
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x += moveSpeed * cosf(moveAngle);
	pos.y += moveSpeed * sinf(moveAngle);


	//��ʊO�ɏo����t���O��false�ɂ���
	if (pos.x >= 120 || pos.x <= -120 || pos.y >= 75 || pos.y <= -75)
	{
		isInScreen = false;
	}


	//�X�V�������W���Z�b�g
	enemyObject->SetPosition(pos);

}

void BaseEnemy::ShotBullet()
{
	//�e�͖��t���[�����˂��Ȃ��̂�false�ɖ߂��Ă���
	isBulletShot = false;
	//�e���˃^�C�}�[���X�V����
	bulletShotTimer++;
	//�e���˃^�C�}�[����莞�Ԃ܂ŃJ�E���g���ꂽ��
	const int bulletInterval = 300;
	if (bulletShotTimer >= bulletInterval)
	{
		//�e���˃^�C�}�[������
		bulletShotTimer = 0;

		//�e����
		isBulletShot = true;
	}
}

void BaseEnemy::KnockBack()
{
	//�m�b�N�o�b�N���s������
	const int knockBackTime = 20;
	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)knockBackTimer / knockBackTime;
	//�m�b�N�o�b�N��̑��x
	const float knockBackStartSpeed = 1.0f;
	float knockBackSpeed = Easing::OutCubic(knockBackStartSpeed, 0, easeTimer);
	int power = killBulletPower / 10;

	//���W���X�V
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x -= knockBackSpeed * sinf(knockBackAngle) * power;
	pos.y += knockBackSpeed * cosf(knockBackAngle) * power;
	//�X�V�������W���Z�b�g
	enemyObject->SetPosition(pos);

	//�m�b�N�o�b�N�^�C�}�[�X�V
	knockBackTimer++;
	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (knockBackTimer >= knockBackTime)
	{
		//�F�𔖂�����
		//enemyObject->SetColor({ 1, 0, 0, 0.5f });

		//���݂���I��
		isExistence = false;
	}
}
