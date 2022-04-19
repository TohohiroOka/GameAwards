#include "PinCircle.h"

PinCircle* PinCircle::Create(Model* model, Pin* pin)
{
	//�C���X�^���X�𐶐�
	PinCircle* instance = new PinCircle();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, pin)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool PinCircle::Initialize(Model* model, Pin* pin)
{
	//�~�I�u�W�F�N�g����
	circleObject = Object3d::Create();
	if (circleObject == nullptr) {
		return false;
	}

	//�G�̏���ێ�
	this->pin = pin;

	//���W���Z�b�g �G�̒��S���W�Ɠ���
	XMFLOAT3 pos = pin->GetPosition();
	circleObject->SetPosition(pos);

	//��̔��a���Z�b�g(�s���̑傫�� �~ 5�j
	this->baseRadius = pin->GetScale().x * 5;

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
