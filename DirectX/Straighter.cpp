#include "Straighter.h"


Straighter* Straighter::Create(Model* model, XMFLOAT3 spawnPosition, float moveDegree, int knockBackPowerLevel)
{
	//�C���X�^���X�𐶐�
	Straighter* instance = new Straighter();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, spawnPosition, moveDegree)) {
		delete instance;
		assert(0);
	}

	//�e�������ꍇ�A�e�̃m�b�N�o�b�N�̋����������p��
	instance->SetParentKnockBackPowerLevel(knockBackPowerLevel);

	return instance;
}

bool Straighter::Initialize(Model* model, XMFLOAT3 spawnPosition, float moveDegree)
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
	enemyObject->SetScale({ 2, 2, 1 });

	//���f�����Z�b�g
	if (model) {
		enemyObject->SetModel(model);
	}

	//�F�������
	enemyObject->SetColor({ 0, 0, 1, 1 });

	//�U���͂��Z�b�g
	power = 2;

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
		if (pos.x <= -wallLine.x + size.x / 2) { ReflectionX(); }
		else if (pos.x >= wallLine.x - size.x / 2) { ReflectionX(); }
		if (pos.y <= -wallLine.y + size.y / 2) { ReflectionY(); }
		else if (pos.y >= wallLine.y - size.y / 2) { ReflectionY(); }
	}
	//�Ǔ��ɂ��Ȃ��ꍇ
	else
	{
		//�Ǔ��ɓ��������`�F�b�N����
		isInScreen = CheckInScreen();
	}

	//�ړ��ʂ����W�ɉ��Z���Ĉړ�������
	pos.x += vel.x;
	pos.y += vel.y;

	//�X�V�������W���Z�b�g
	enemyObject->SetPosition(pos);
}

bool Straighter::CheckInScreen()
{
	//�Ǔ��ɂ��邩
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	if (pos.x > -wallLine.x + size.x / 2 && pos.x < wallLine.x - size.x / 2 &&
		pos.y > -wallLine.y + size.y / 2 && pos.y < wallLine.y - size.y / 2)
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
}

void Straighter::ReflectionY()
{
	//�㉺���˗p�ɔ��ˊp���Z�b�g����
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);
}

void Straighter::SetParentKnockBackPowerLevel(int knockBackPowerLevel)
{
	this->knockBackPowerLevel = knockBackPowerLevel;

	//�e�̐F�ɍ��킹�ēG�̐F��ύX
	if (knockBackPowerLevel == 0) { enemyObject->SetColor({ 0, 0, 1, 1 }); }
	else if (knockBackPowerLevel == 1) { enemyObject->SetColor({ 0, 1, 0, 1 }); }
	else if (knockBackPowerLevel == 2) { enemyObject->SetColor({ 1, 1, 0, 1 }); }
	else if (knockBackPowerLevel >= 3) { enemyObject->SetColor({ 1, 0, 0, 1 }); }
}
