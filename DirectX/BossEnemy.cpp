#include "BossEnemy.h"
#include "SafeDelete.h"

DirectX::XMFLOAT2 BossEnemy::frameLine = { 97, 53 };

BossEnemy::~BossEnemy()
{
    safe_delete(bossObject);
}

bool BossEnemy::Initialize(Model* model)
{
	//�I�u�W�F�N�g����
	bossObject = Object3d::Create();
	if (bossObject == nullptr) {
		return false;
	}

	//���f�����Z�b�g
	if (model) {
		bossObject->SetModel(model);
	}

	return true;
}

void BossEnemy::Draw()
{
	//�{�X�I�u�W�F�N�g�`��
	bossObject->Draw();
}

void BossEnemy::Damage(int damagePower)
{
	//�����Ŏw�肵�������̕�HP�����炷
	HP -= damagePower;
}

void BossEnemy::Dead()
{
	//���S��Ԃɂ���
	isAlive = false;
}

void BossEnemy::SetDelete()
{
	//�폜����
	isDelete = true;
}

void BossEnemy::Reset()
{
	//�̗͂����Z�b�g
	HP = 20;
	//�s�����ŏ��ɖ߂�
	action = 0;
	//�s���V�[�����ŏ��ɖ߂�
	actionScene = 0;
	//���̍s���V�[���ɂ��Ȃ�
	isChangeActionScene = false;
	//�s���������Ԃ��v������^�C�}�[��������
	actionTimer = 0;
	//����ł��琶���Ԃ�
	isAlive = true;
	//�폜��Ԃ�����
	isDelete = false;
	//�X�|�[�����ɂ���
	isDuringSpawn = true;
	//�X�|�[���^�C�}�[��������
	spawnTimer = 0;
	//�e�𔭎˂��Ȃ�
	isBulletShot = false;
	//�e���˃J�E���g��������
	shotCount = 0;
	//�e���˂���̎��Ԃ�������
	bulletShotTimer = 0;
	//�����Ռ��������Ȃ�
	isImpact = false;
}

bool BossEnemy::TriggerImpact()
{
	//�����Ռ���^����Ȃ�
	if (isImpact)
	{
		//�g���K�[�Ȃ̂�false�ɖ߂��Ă���
		isImpact = false;

		//true��Ԃ�
		return true;
	}

	return false;
}
