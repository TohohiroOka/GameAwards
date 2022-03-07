#include "PowerUpLine.h"
#include "SafeDelete.h"

PowerUpLine *PowerUpLine::Create(XMFLOAT3 startPoint, XMFLOAT3 endPoint)
{
	//�C���X�^���X�𐶐�
	PowerUpLine *instance = new PowerUpLine();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(startPoint, endPoint)) {
		delete instance;
		assert(0);
	}

	return instance;
}

PowerUpLine::~PowerUpLine()
{
	safe_delete(line);
}

bool PowerUpLine::Initialize(XMFLOAT3 startPoint, XMFLOAT3 endPoint)
{
	//������
	line = DrawLine3D::Create();
	if (line == nullptr) {
		return false;
	}

	//�n�_�ƏI�_���Z�b�g
	this->startPoint = startPoint;
	this->endPoint = endPoint;

	//�F�Ƒ������w��
	XMFLOAT4 color = { 1, 1, 1, 1 };
	float weight = 0.5;

	//�������
	line->SetLine(startPoint, endPoint, color, weight);

	return true;
}

void PowerUpLine::Update(Camera *camera)
{
	//�����ŐV�̏�ԂɍX�V
	line->Update(camera);
}

void PowerUpLine::Draw()
{
	//����`��
	line->Draw();
}

void PowerUpLine::SetColor(XMFLOAT4 color)
{
	float weight = 0.5;

	line->SetLine(startPoint, endPoint, color, weight);
}
