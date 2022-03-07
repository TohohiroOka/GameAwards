#include "DeadEnemyPoint.h"
#include "SafeDelete.h"

DeadEnemyPoint *DeadEnemyPoint::Create(Model *model, XMFLOAT3 position, float radius)
{
	//�C���X�^���X�𐶐�
	DeadEnemyPoint *instance = new DeadEnemyPoint();
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

DeadEnemyPoint::~DeadEnemyPoint()
{
	safe_delete(deadPointObject);
}

bool DeadEnemyPoint::Initialize(Model *model, XMFLOAT3 position, float radius)
{
	//���񂾓G�̈ʒu�I�u�W�F�N�g����
	deadPointObject = Object3d::Create();
	if (deadPointObject == nullptr) {
		return false;
	}

	//�����n�_�Ɖ~�̔��a���Z�b�g
	deadPointObject->SetPosition(position);
	deadPointObject->SetRotation({ 0, 0, 180 });
	this->radius = radius;
	deadPointObject->SetScale({ radius, radius, radius });

	if (model) {
		deadPointObject->SetModel(model);
	}

	return true;
}

void DeadEnemyPoint::Update()
{
	//�I�u�W�F�N�g�X�V
	deadPointObject->Update();
}

void DeadEnemyPoint::Draw()
{
	//�I�u�W�F�N�g�`��
	deadPointObject->Draw();
}
