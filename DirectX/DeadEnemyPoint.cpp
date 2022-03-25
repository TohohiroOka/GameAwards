#include "DeadEnemyPoint.h"
#include "Easing.h"
#include "SafeDelete.h"

DeadEnemyPoint *DeadEnemyPoint::Create(Model *model, BaseEnemy *enemy, float radius)
{
	//�C���X�^���X�𐶐�
	DeadEnemyPoint *instance = new DeadEnemyPoint();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, enemy, radius)) {
		delete instance;
		assert(0);
	}

	return instance;
}

DeadEnemyPoint::~DeadEnemyPoint()
{
	safe_delete(deadPointObject);
}

bool DeadEnemyPoint::Initialize(Model *model, BaseEnemy *enemy, float radius)
{
	//���񂾓G�̈ʒu�I�u�W�F�N�g����
	deadPointObject = Object3d::Create();
	if (deadPointObject == nullptr) {
		return false;
	}

	//�G�̏���ێ�
	this->enemy = enemy;

	//���W���Z�b�g �G�̒��S���W�Ɠ���
	XMFLOAT3 pos = enemy->GetPosition();
	deadPointObject->SetPosition(pos);

	//��̃T�C�Y���Z�b�g
	BaseRadius = radius;

	//�ύX�O�̉~�̃T�C�Y���Z�b�g
	changeRadiusStart = 0;
	//�ύX��̉~�̃T�C�Y���Z�b�g
	changeRadiusEnd = BaseRadius;
	deadPointObject->SetScale({ changeRadiusStart, changeRadiusStart, 1 });

	//�~�̃T�C�Y��ύX��Ԃɂ���
	isChangeRadius = true;

	if (model) {
		deadPointObject->SetModel(model);
	}

	//�F��΂ɂ���
	deadPointObject->SetColor({ 0.2f, 1, 0.2f, 0.4 });

	return true;
}

void DeadEnemyPoint::Update()
{
	//�T�C�Y�ύX���s������
	const int changeTime = 20;
	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)changeRadiusTimer / changeTime;

	//�C�[�W���O�ŃT�C�Y�ύX
	float newRadius = Easing::OutCubic(changeRadiusStart, changeRadiusEnd, easeTimer);

	//�T�C�Y���X�V
	radius = newRadius;
	//�X�V�����T�C�Y���Z�b�g
	deadPointObject->SetScale({ radius, radius, 1 });

	//�T�C�Y�ύX�^�C�}�[�X�V
	changeRadiusTimer++;
	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (changeRadiusTimer >= changeTime)
	{
		//�T�C�Y�ύX��ԏI��
		isChangeRadius = false;
	}

	//�I�u�W�F�N�g�X�V
	deadPointObject->Update();
}

void DeadEnemyPoint::Draw()
{
	//�I�u�W�F�N�g�`��
	deadPointObject->Draw();
}

void DeadEnemyPoint::ChangeRadius()
{
	//��������o�Ă�����̃J�E���g���X�V
	countLine++;
	//�ύX�O�̉~�̃T�C�Y���Z�b�g
	changeRadiusStart = radius;
	//�ύX��̉~�̃T�C�Y���Z�b�g
	changeRadiusEnd = BaseRadius + ((float)countLine / 3);
	//�T�C�Y�ύX�^�C�}�[��������
	changeRadiusTimer = 0;
	//�T�C�Y��ύX���ɂ���
	isChangeRadius = true;
}

void DeadEnemyPoint::CheckUseEnemy(BaseEnemy *enemy)
{
	//�ێ����Ă���G�������̓G���g�p���Ă�����
	if (this->enemy == enemy)
	{
		//�폜�t���O��true��
		isDelete = true;
	}
}
