#include "FixedObject.h"
#include "SafeDelete.h"

FixedObject *FixedObject::Create(Model *model, XMFLOAT3 position)
{
	//�C���X�^���X�𐶐�
	FixedObject *instance = new FixedObject();
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

FixedObject::~FixedObject()
{
	safe_delete(fixedObject);
}

bool FixedObject::Initialize(Model *model, XMFLOAT3 position)
{
	//�Œ�I�u�W�F�N�g����
	fixedObject = Object3d::Create();
	if (fixedObject == nullptr) {
		return false;
	}

	//���W���Z�b�g �G�̒��S���W�Ɠ���
	fixedObject->SetPosition(position);

	//�傫�����Z�b�g
	fixedObject->SetScale({ 2, 2, 1 });

	if (model) {
		fixedObject->SetModel(model);
	}

	return true;
}

void FixedObject::Update()
{
	//�Œ�I�u�W�F�N�g���X�V
	fixedObject->Update();
}

void FixedObject::Draw()
{
	//�Œ�I�u�W�F�N�g��`��
	fixedObject->Draw();
}

void FixedObject::SetDelete()
{
	//�폜����
	isDelete = true;
}
