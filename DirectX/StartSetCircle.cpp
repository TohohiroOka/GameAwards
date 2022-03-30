#include "StartSetCircle.h"

StartSetCircle *StartSetCircle::Create(Model *model, XMFLOAT3 position, float radius)
{
	//�C���X�^���X�𐶐�
	StartSetCircle *instance = new StartSetCircle();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, position, radius)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool StartSetCircle::Initialize(Model *model, XMFLOAT3 position, float radius)
{
	//�~�I�u�W�F�N�g����
	circleObject = Object3d::Create();
	if (circleObject == nullptr) {
		return false;
	}

	//���W���Z�b�g
	circleObject->SetPosition(position);

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

	//�F��Ԃɂ���
	circleObject->SetColor({ 1, 0.2f, 0.2f, 0.4f });

	return true;
}