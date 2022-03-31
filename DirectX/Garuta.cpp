#include "Garuta.h"

Garuta *Garuta::Create(Model *garutaModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//�C���X�^���X�𐶐�
	Garuta *instance = new Garuta();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(garutaModel, stayPointModel, spawnPosition, stayPosition)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool Garuta::Initialize(Model *enemyModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//�X�|�[�����̍��W�ƈړ���̍��W���Z�b�g
	this->spawnPosition = spawnPosition;
	this->stayPosition = stayPosition;

	//�K���^�I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(spawnPosition);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 2.0f, 2.0f, 1 });

	//���f�����Z�b�g
	if (enemyModel) {
		enemyObject->SetModel(enemyModel);
	}
	//�u���[����������
	enemyObject->SetBloom(true);

	//�X�|�[���n�_�I�u�W�F�N�g����
	stayPointObject = Object3d::Create();
	if (stayPointObject == nullptr) {
		return false;
	}
	//�X�|�[�����W�Z�b�g
	stayPointObject->SetPosition(stayPosition);
	//�傫�����Z�b�g
	stayPointObject->SetScale({ 2.0f, 2.0f, 1 });

	//���f�����Z�b�g
	if (stayPointModel) {
		stayPointObject->SetModel(stayPointModel);
	}

	//�u���[����������
	//stayPointObject->SetBloom(true);

	//�F��ύX
	stayPointObject->SetColor({ 1, 1, 1, 0.4f });

	return true;
}