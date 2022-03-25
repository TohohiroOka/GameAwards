#include "PowerUpLine.h"
#include "SafeDelete.h"

PowerUpLine *PowerUpLine::Create(DeadEnemyPoint *startPoint, DeadEnemyPoint *endPoint)
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

bool PowerUpLine::Initialize(DeadEnemyPoint *startPoint, DeadEnemyPoint *endPoint)
{
	//������
	line = DrawLine3D::Create();
	if (line == nullptr) {
		return false;
	}

	//�n�_�ƏI�_���Z�b�g
	this->startPoint = startPoint;
	this->endPoint = endPoint;

	//�F���w��
	XMFLOAT4 color = { 0.4f, 1, 0.2f, 1 };

	//�������
	line->SetLine(startPoint->GetPosition(), endPoint->GetPosition(), color, weight);

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

bool PowerUpLine::IsAlreadyCreateLine(DeadEnemyPoint *startPoint, DeadEnemyPoint *endPoint)
{
	//�w�肵���n�_�ƏI�_�Ŋ��ɐ�������Ă�����true��Ԃ�
	if (this->startPoint == startPoint && this->endPoint == endPoint)
	{
		return true;
	}
	//�n�_�ƏI�_���t�̉\�������肷��
	if (this->startPoint == endPoint && this->endPoint == startPoint)
	{
		return true;
	}

	return false;
}

void PowerUpLine::CheckUsePoints(DeadEnemyPoint *point)
{
	//�n�_���I�_�ň����̉~���g�p���Ă�����
	if (point == startPoint || point == endPoint)
	{
		//�폜�t���O��true��
		isDelete = true;
	}
}

void PowerUpLine::SetColor(XMFLOAT4 color)
{
	//�F��ς��Đ�����蒼��
	line->SetLine(startPoint->GetPosition(), endPoint->GetPosition(), color, weight);
}
