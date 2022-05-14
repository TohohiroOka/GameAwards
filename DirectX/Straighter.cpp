#include "Straighter.h"

Model* Straighter::straighterModel = nullptr;


Straighter* Straighter::Create(XMFLOAT3 spawnPosition, float moveDegree)
{
	//�C���X�^���X�𐶐�
	Straighter* instance = new Straighter();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(spawnPosition, moveDegree)) {
		delete instance;
		assert(0);
	}

	return instance;
}

void Straighter::SetModel(Model* straighterModel)
{
	//�����̃��f�������ʂŎg�����߃Z�b�g����
	Straighter::straighterModel = straighterModel;
}

bool Straighter::Initialize(XMFLOAT3 spawnPosition, float moveDegree)
{
	//�����O���[�v��ݒ�
	group = EnemyGroup::Straighter;

	//�I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(spawnPosition);
	//�Ǔ��ɂ��邩�`�F�b�N
	isInScreen = CheckInScreen();

	//�傫�����Z�b�g
	enemyObject->SetScale({ 5, 5, 1 });

	//���f�����Z�b�g
	if (straighterModel) {
		enemyObject->SetModel(straighterModel);
	}

	//�ړ��p�x���Z�b�g
	SetMoveAngle(moveDegree);

	return true;
}

void Straighter::Move()
{
	//�ړ����x�Ɉړ��p�x����Z���č��W���X�V
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();

	//�Ǔ��ɂ���ꍇ
	if (isInScreen)
	{
		//�g�ɓ������Ă����甽�˂�����
		if (pos.x <= wallLineMin.x + size.x / 2) { ReflectionX(); }
		else if (pos.x >= wallLineMax.x - size.x / 2) { ReflectionX(); }
		if (pos.y <= wallLineMin.y + size.y / 2) { ReflectionY(); }
		else if (pos.y >= wallLineMax.y - size.y / 2) { ReflectionY(); }
	}
	//�Ǔ��ɂ��Ȃ��ꍇ
	else
	{
		//�Ǔ��ɓ��������`�F�b�N����
		isInScreen = CheckInScreen();
	}

	//�ړ��ʂ����W�ɉ��Z���Ĉړ�������
	pos.x += vel.x * moveSpeed;
	pos.y += vel.y * moveSpeed;

	//���񂾂�x������
	moveSpeed -= 0.005f;
	const float moveSpeedMin = 0.7f;
	if (moveSpeed <= moveSpeedMin)
	{
		moveSpeed = moveSpeedMin;
	}

	//�X�V�������W���Z�b�g
	enemyObject->SetPosition(pos);
}

void Straighter::ResultMove()
{
	//�ړ����x�Ɉړ��p�x����Z���č��W���X�V
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();

	//�ړ��ʂ����W�ɉ��Z���Ĉړ�������
	pos.x += vel.x * moveSpeed;
	pos.y += vel.y * moveSpeed;

	//���񂾂�x������
	moveSpeed -= 0.005f;
	const float moveSpeedMin = 0.7f;
	if (moveSpeed <= moveSpeedMin)
	{
		moveSpeed = moveSpeedMin;
	}

	//�X�V�������W���Z�b�g
	enemyObject->SetPosition(pos);
}

bool Straighter::CheckInScreen()
{
	//�Ǔ��ɂ��邩
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	if (pos.x > wallLineMin.x + size.x / 2 && pos.x < wallLineMax.x - size.x / 2 &&
		pos.y > wallLineMin.y + size.y / 2 && pos.y < wallLineMax.y - size.y / 2)
	{
		return true;
	}

	return false;
}

void Straighter::ReflectionX()
{
	//���E���˗p�ɔ��ˊp���Z�b�g����
	float reflectionAngle = 360 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//���x��ύX����
	moveSpeed = 1.5f;
}

void Straighter::ReflectionY()
{
	//�㉺���˗p�ɔ��ˊp���Z�b�g����
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//���x��ύX����
	moveSpeed = 1.5f;
}
