#include "LaserSite.h"
#include "SafeDelete.h"

LaserSite *LaserSite::Create()
{
	//�C���X�^���X�𐶐�
	LaserSite *instance = new LaserSite();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	return instance;
}

LaserSite::~LaserSite()
{
	safe_delete(line);
}

bool LaserSite::Initialize()
{
	//������
	line = DrawLine3D::Create();
	if (line == nullptr) {
		return false;
	}

	//�F���w��
	color = { 1, 0.1f, 0.1f, 1 };
	//���̑���
	weight = 0.5f;

	//�������
	line->SetLine(startPoint, endPoint, color, weight);

	return true;
}

void LaserSite::Update(Camera *camera)
{
	//���[�U�[���X�V
	line->Update(camera);
}

void LaserSite::Draw()
{
	//���[�U�[��`��
	line->Draw();
}

void LaserSite::SetPosition(XMFLOAT3 weaponPosition, XMFLOAT3 weaponRotation)
{
	//���[�U�[�T�C�g�̓E�G�|���I�u�W�F�N�g�̊p�x��Ǐ]����
	XMFLOAT3 rota = weaponRotation;
	float angle = DirectX::XMConvertToRadians(rota.z + 90);

	//���[�U�[�T�C�g�̎n�_�̓E�G�|���I�u�W�F�N�g�̒��S���班�����ꂽ�Ƃ���(��ʂł͂��傤�ǃI�u�W�F�N�g�̎n�_)
	XMFLOAT3 startPoint = weaponPosition;
	startPoint.x += 6.0f * cosf(angle);
	startPoint.y += 6.0f * sinf(angle);
	this->startPoint = startPoint;

	//���[�U�[�T�C�g�̏I�_�͎n�_��������Ɖ����Ƃ���(��ʂł͂��傤�ǋ�������Ă��Ȃ��e�̕`�悪�I������n�_)
	XMFLOAT3 endPoint = weaponPosition;
	endPoint.x += 35.0f * cosf(angle);
	endPoint.y += 35.0f * sinf(angle);
	this->endPoint = endPoint;

	//�X�V�������W�Ő�����蒼��
	line->SetLine(startPoint, endPoint, color, weight);
}
