#include "LaserSite.h"
#include "SafeDelete.h"

LaserSite* LaserSite::Create(Model* model)
{
	//�C���X�^���X�𐶐�
	LaserSite* instance = new LaserSite();
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

LaserSite::~LaserSite()
{
	safe_delete(razorObject);
}

bool LaserSite::Initialize(Model* model)
{
	//�I�u�W�F�N�g����
	razorObject = Object3d::Create(model);
	if (razorObject == nullptr) {
		return false;
	}

	//�傫�����Z�b�g
	razorObject->SetScale({ 2, 2, 1 });

	return true;
}

void LaserSite::Update()
{
	//�I�u�W�F�N�g���X�V
	razorObject->Update();
}

void LaserSite::Draw()
{
	//�I�u�W�F�N�g��`��
	razorObject->Draw();
}

void LaserSite::SetPosition(XMFLOAT3 weaponPosition, XMFLOAT3 weaponRotation)
{
	//���[�U�[�T�C�g�̓E�G�|���I�u�W�F�N�g�̊p�x��Ǐ]����
	XMFLOAT3 rota = weaponRotation;
	razorObject->SetRotation(rota);

	float angle = DirectX::XMConvertToRadians(rota.z + 90);

	//���[�U�[�T�C�g�̎n�_�̓E�G�|���I�u�W�F�N�g�̒��S���班�����ꂽ�Ƃ���(��ʂł͂��傤�ǃI�u�W�F�N�g�̎n�_)
	XMFLOAT3 startPoint = weaponPosition;
	//startPoint.x -= 6.0f * cosf(angle);
	//startPoint.y -= 6.0f * sinf(angle);
	razorObject->SetPosition(startPoint);
}
