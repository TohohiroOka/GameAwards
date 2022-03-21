#include "Garuta.h"

Garuta *Garuta::Create(Model *model, XMFLOAT3 position, float moveDegree)
{
	//�C���X�^���X�𐶐�
	Garuta *instance = new Garuta();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	//�ړ��p�x��ݒ�
	instance->SetMoveAngle(moveDegree);

	return instance;
}

bool Garuta::Initialize(Model *model, XMFLOAT3 position)
{
	//�U�R�����I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(position);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 1.5, 1.5, 1 });

	//���f�����Z�b�g
	if (model) {
		enemyObject->SetModel(model);
	}

	enemyObject->SetBloom(true);

	//�F��Ԃ�����
	//enemyObject->SetColor({ 1, 0, 0, 1 });

	return true;
}

void Garuta::SetMoveAngle(float moveDegree)
{
	//�������0�ɂ��邽��90�x�X����
	float degree = moveDegree;
	degree += 90;

	//�x�������W�A���ɒ���
	float angle = DirectX::XMConvertToRadians(degree);
	moveAngle = angle;
}
