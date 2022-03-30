#include "Hageta.h"

Hageta *Hageta::Create(Model *model, XMFLOAT3 position, float moveDegree)
{
	//�C���X�^���X�𐶐�
	Hageta *instance = new Hageta();
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

bool Hageta::Initialize(Model *model, XMFLOAT3 position)
{
	//�I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(position);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 3, 3, 1 });

	//���f�����Z�b�g
	if (model) {
		enemyObject->SetModel(model);
	}

	//�F��Ԃ�����
	enemyObject->SetColor({ 1, 0, 0, 1 });

	return true;
}

void Hageta::SetMoveAngle(float moveDegree)
{
	//�������0�ɂ��邽��90�x�X����
	float degree = moveDegree;
	degree += 90;

	//�x�������W�A���ɒ���
	float angle = DirectX::XMConvertToRadians(degree);
	moveAngle = angle;

	//�I�u�W�F�N�g�̌�����i�s�����ɃZ�b�g
	XMFLOAT3 rota = { 0, 0, moveDegree };
	enemyObject->SetRotation(rota);
}
