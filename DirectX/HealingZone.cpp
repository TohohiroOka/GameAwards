#include "HealingZone.h"
#include "SafeDelete.h"

HealingZone* HealingZone::Create(Model* model)
{
	//�C���X�^���X�𐶐�
	HealingZone* instance = new HealingZone();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

HealingZone::~HealingZone()
{
	safe_delete(healingZoneObject);
}

bool HealingZone::Initialize(Model* model)
{
	//�Ռ��g�I�u�W�F�N�g����
	healingZoneObject = Object3d::Create();
	if (healingZoneObject == nullptr) {
		return false;
	}
	//���f�����Z�b�g
	if (model) {
		healingZoneObject->SetModel(model);
	}
	//���W���Z�b�g
	healingZoneObject->SetPosition({ 0, -10, 0 });
	//�傫�����Z�b�g
	healingZoneObject->SetScale({ 30.0f, 30.0f, 1.0f });
	//�F���Z�b�g
	healingZoneObject->SetColor({ 0.7f, 0.7f, 0.7f, 1 });

	return true;
}

void HealingZone::Update()
{
	//�I�u�W�F�N�g�X�V
	healingZoneObject->Update();
}

void HealingZone::Draw()
{
	//�I�u�W�F�N�g�`��
	healingZoneObject->Draw();
}
