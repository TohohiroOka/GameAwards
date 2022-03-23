#include "Garutata.h"

Garutata *Garutata::Create(Model *model, XMFLOAT3 position)
{
	//�C���X�^���X�𐶐�
	Garutata *instance = new Garutata();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool Garutata::Initialize(Model *model, XMFLOAT3 position)
{
	//�U�R�����I�u�W�F�N�g����
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

	//�F��΂ɂ���
	//enemyObject->SetColor({ 0, 1, 0, 1 });

	return true;
}