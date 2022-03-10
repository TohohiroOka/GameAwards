#include "Zakorin.h"

Zakorin *Zakorin::Create(Model *model, XMFLOAT3 position)
{
	//�C���X�^���X�𐶐�
	Zakorin *instance = new Zakorin();
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

bool Zakorin::Initialize(Model *model, XMFLOAT3 position)
{
	//�U�R�����I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(position);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 4, 4, 1 });

	//���f�����Z�b�g
	if (model) {
		enemyObject->SetModel(model);
	}

	return true;
}

void Zakorin::Update()
{
	//�������̓G�̂ݕ`��
	if (!isAlive) return;

	/*XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x += 0.01f;
	pos.y -= 0.01f;
	enemyObject->SetPosition(pos);*/

	//�I�u�W�F�N�g�X�V
	enemyObject->Update();
}
