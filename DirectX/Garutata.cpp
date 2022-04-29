#include "Garutata.h"

Garutata* Garutata::Create(Model* garutataModel, Model* stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//�C���X�^���X�𐶐�
	Garutata* instance = new Garutata();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(garutataModel, stayPointModel, spawnPosition, stayPosition)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool Garutata::Initialize(Model* enemyModel, Model* stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//�����O���[�v���K���^�^�ɂ���
	group = GaruGroup::Garutata;

	//�X�|�[�����̍��W�ƈړ���̍��W���Z�b�g
	this->spawnPosition = spawnPosition;
	this->stayPosition = stayPosition;

	//�K���^�^�I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(spawnPosition);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 6, 6, 1 });

	//���f�����Z�b�g
	if (enemyModel) {
		enemyObject->SetModel(enemyModel);
	}

	//�F��ύX
	enemyObject->SetColor({ 1, 1, 1, 1 });

	//�u���[����������
	//enemyObject->SetBloom(true);
	//�F��ύX
	enemyObject->SetColor({ 0, 0, 1, 1 });

	//�X�|�[���n�_�I�u�W�F�N�g����
	stayPointObject = Object3d::Create();
	if (stayPointObject == nullptr) {
		return false;
	}
	//�X�|�[�����W�Z�b�g
	stayPointObject->SetPosition(stayPosition);
	//�傫�����Z�b�g
	stayPointObject->SetScale({ 6, 6, 1 });

	//���f�����Z�b�g
	if (stayPointModel) {
		stayPointObject->SetModel(stayPointModel);
	}
	//�F��ύX
	stayPointObject->SetColor({ 1, 1, 1, 0.4f });

	//�u���[����������
	//stayPointObject->SetBloom(true);

	return true;
}