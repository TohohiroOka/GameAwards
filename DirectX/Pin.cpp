#include "Pin.h"
#include "SafeDelete.h"

Pin* Pin::Create(Model* model, XMFLOAT3 position)
{
	//�C���X�^���X�𐶐�
	Pin* instance = new Pin();
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

Pin::~Pin()
{
	safe_delete(pinObject);
}

bool Pin::Initialize(Model* model, XMFLOAT3 position)
{
	//�I�u�W�F�N�g����
	pinObject = Object3d::Create();
	if (pinObject == nullptr) {
		return false;
	}

	//�����Z�b�g
	pinObject->SetPosition(position);
	pinObject->SetScale({ 2, 2, 1 });

	//���f�����Z�b�g
	if (model) {
		pinObject->SetModel(model);
	}

	return true;
}

void Pin::Update()
{
	//�_���[�W�t���O��true�Ȃ�
	if (isDamage)
	{
		//�^�C�}�[���X�V
		damageTimer++;

		//�^�C�}�[����莞�Ԍo�߂�����
		const int damageTime = 100;
		if (damageTimer >= damageTime)
		{
			//�_���[�W�^�C�}�[��������
			damageTimer = 0;

			//�_���[�W�t���O��false�ɂ���
			isDamage = false;

			pinObject->SetColor({ 1,1,1,1 });
		}
	}

	//�I�u�W�F�N�g�X�V
	pinObject->Update();
}

void Pin::Draw()
{
	//�I�u�W�F�N�g�`��
	pinObject->Draw();
}

void Pin::Damage(int damagePower)
{
	//�����̕��_���[�W����炤
	HP -= damagePower;
	//HP��0�ȉ��ɂȂ�����폜����
	if (HP <= 0) 
	{
		SetDelete();
	}

	//�F��ύX����
	pinObject->SetColor({ 1,0,1,1 });

	//�_���[�W��Ԃɂ���
	isDamage = true;
}

void Pin::Dead()
{
	//���S��Ԃɂ���
	isAlive = false;
}

void Pin::SetDelete()
{
	//�폜����
	isDelete = true;
}
