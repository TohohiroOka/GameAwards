#include "Hagetata.h"

Hagetata *Hagetata::Create(Model *model, XMFLOAT3 position, XMFLOAT3 targetPosition)
{
    //�C���X�^���X�𐶐�
	Hagetata *instance = new Hagetata();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	//�ړ��p�x��ݒ�
	instance->SetMoveAngle(targetPosition);

	return instance;
}

bool Hagetata::Initialize(Model *model, XMFLOAT3 position)
{
	//�I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(position);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 6, 6, 1 });

	//���f�����Z�b�g
	if (model) {
		enemyObject->SetModel(model);
	}

	//�F��΂ɂ���
	enemyObject->SetColor({ 0, 1, 0, 1 });

	return true;
}

void Hagetata::SetMoveAngle(XMFLOAT3 targetPosition)
{
	//�ړ��p�x��ݒ肷��i�W�I�Ɍ������Ĉ꒼���j
	XMFLOAT3 position = enemyObject->GetPosition(); 
	float radian = atan2f(targetPosition.y - position.y, targetPosition.x - position.x);
	moveAngle = radian;
}

