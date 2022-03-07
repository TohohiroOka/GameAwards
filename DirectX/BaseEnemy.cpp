#include "BaseEnemy.h"
#include "SafeDelete.h"

int BaseEnemy::deadCount = 0;

BaseEnemy::~BaseEnemy()
{
	safe_delete(enemySprite);
}

void BaseEnemy::Draw()
{
	//�������̓G�̂ݕ`��
	//if (!isAlive) return;

	//�X�v���C�g�`��
	enemySprite->Draw();
}

void BaseEnemy::Damage(int damagePower)
{
	//�w�肵�������̕�HP�����炷
	HP -= damagePower;
}

void BaseEnemy::Dead()
{
	//���S��Ԃɂ���
	isAlive = false;

	//������₷���悤�ɐF��ς���
	enemySprite->SetColor({ 1,0,0,1 });

	//�J�E���g�𑝂₷
	deadCount++;

	//���񂾏��Ԃ����蓖�Ă�(0�͏����l�Ȃ̂�1����)
	deadNum = deadCount;
}