#include "BossEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"

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
	//���S���ăT�C�Y��ύX��Ԃł͂Ȃ�
	isDeadChangeScale = false;
	//�c���O�̃T�C�Y������
	changeStartScale = 0;
	//�c����̃T�C�Y������
	changeEndScale = 0;
	//�T�C�Y�ύX�V�[��������
	changeScaleScene = 0;
	//�T�C�Y�ύX�^�C�}�[������
	changeScaleTimer = 0;
	//���݂��Ă���
	isExistence = true;
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

void BossEnemy::SetDeadChangeScale()
{
	//�c���O�̃T�C�Y���Z�b�g
	changeStartScale = bossObject->GetScale().x;
	//�c����̃T�C�Y���Z�b�g
	changeEndScale = bossObject->GetScale().x * 3;

	//�T�C�Y�ύX��ԂɃZ�b�g
	isDeadChangeScale = true;
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

void BossEnemy::DeadChangeScale()
{
	//�T�C�Y�ύX�^�C�}�[�X�V
	changeScaleTimer++;

	//�c���V�[��
	if (changeScaleScene == 0)
	{
		//�T�C�Y�ύX���鎞��
		const int changeTime = 18;

		//�C�[�W���O�v�Z�p�̎���
		float easeTimer = (float)changeScaleTimer / changeTime;

		//�C�[�W���O�Ŗc��������
		float size = Easing::OutCubic(changeStartScale, changeEndScale, easeTimer);
		//�X�V�����T�C�Y���Z�b�g
		bossObject->SetScale({ size, size, 1 });

		//�^�C�}�[���w�肵�����ԂɂȂ�����
		if (changeScaleTimer >= changeTime)
		{
			//�T�C�Y�ύX�^�C�}�[������
			changeScaleTimer = 0;

			//���̃V�[����
			changeScaleScene++;
		}
	}
	//�k���V�[��
	else if (changeScaleScene == 1)
	{
		//�T�C�Y�ύX���鎞��
		const int changeTime = 42;

		//�C�[�W���O�v�Z�p�̎���
		float easeTimer = (float)changeScaleTimer / changeTime;

		//�C�[�W���O�ŏk��������
		float size = Easing::OutCubic(changeEndScale, 0, easeTimer);
		//�X�V�����T�C�Y���Z�b�g
		bossObject->SetScale({ size, size, 1 });

		//�^�C�}�[���w�肵�����ԂɂȂ�����
		if (changeScaleTimer >= changeTime)
		{
			//�T�C�Y�ύX��ԏI��
			isDeadChangeScale = false;

			//���݂�����
			isExistence = false;
		}
	}
}
