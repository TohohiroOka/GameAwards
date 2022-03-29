#include "EnemyCircle.h"

EnemyCircle *EnemyCircle::Create(Model *model, BaseEnemy *enemy, float radius)
{
	//�C���X�^���X�𐶐�
	EnemyCircle *instance = new EnemyCircle();
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

bool EnemyCircle::Initialize(Model *model, BaseEnemy *enemy, float radius)
{
	//�~�I�u�W�F�N�g����
	circleObject = Object3d::Create();
	if (circleObject == nullptr) {
		return false;
	}

	//�G�̏���ێ�
	this->enemy = enemy;

	//���W���Z�b�g �G�̒��S���W�Ɠ���
	XMFLOAT3 pos = enemy->GetPosition();
	circleObject->SetPosition(pos);

	//��̃T�C�Y���Z�b�g
	baseRadius = radius;

	//�ύX�O�̉~�̃T�C�Y���Z�b�g
	changeRadiusStart = 0;
	//�ύX��̉~�̃T�C�Y���Z�b�g
	changeRadiusEnd = baseRadius;
	circleObject->SetScale({ changeRadiusStart, changeRadiusStart, 1 });

	//�~�̃T�C�Y��ύX��Ԃɂ���
	isChangeRadius = true;

	if (model) {
		circleObject->SetModel(model);
	}

	//�F��΂ɂ���
	circleObject->SetColor({ 0.2f, 1, 0.2f, 0.4f });

	return true;
}
