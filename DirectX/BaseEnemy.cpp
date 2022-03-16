#include "BaseEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"

BaseEnemy::~BaseEnemy()
{
	safe_delete(enemyObject);
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
