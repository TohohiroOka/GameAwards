#include "Straighter.h"

Model* Straighter::straighterModel[Straighter::modelNum] = { nullptr };


Straighter* Straighter::Create(XMFLOAT3 spawnPosition, float moveDegree, int knockBackPowerLevel)
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

	//�e�������ꍇ�A�e�̃m�b�N�o�b�N�̋����������p��
	instance->SetParentKnockBackPowerLevel(knockBackPowerLevel);

	return instance;
}

void Straighter::SetModel(Model* straighterModel1, Model* straighterModel2, Model* straighterModel3, Model* straighterModel4)
{
	//�����̃��f�������ʂŎg�����߃Z�b�g����
	Straighter::straighterModel[0] = straighterModel1;
	Straighter::straighterModel[1] = straighterModel2;
	Straighter::straighterModel[2] = straighterModel3;
	Straighter::straighterModel[3] = straighterModel4;
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
	if (straighterModel[0]) {
		enemyObject->SetModel(straighterModel[0]);
	}

	//�U���͂��Z�b�g
	power = 2;

	//�ړ��p�x���Z�b�g
	SetMoveAngle(moveDegree);

	return true;
}

void Straighter::SetKnockBack(float angle, int powerLevel, int shockWaveGroup)
{
	BaseEnemy::SetKnockBack(angle, powerLevel, shockWaveGroup);

	//�G�̃��f����ύX
	if (enemyObject->GetModel() != straighterModel[3])
	{
		if (knockBackPowerLevel == 1) { enemyObject->SetModel(straighterModel[1]); }
		else if (knockBackPowerLevel == 2) { enemyObject->SetModel(straighterModel[2]); }
		else if (knockBackPowerLevel >= 3) { enemyObject->SetModel(straighterModel[3]); }
	}
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

void Straighter::SetParentKnockBackPowerLevel(int knockBackPowerLevel)
{
	//�e�̐�����шЗ͂������p��
	this->knockBackPowerLevel = knockBackPowerLevel;

	//������шЗ͂ɍ��킹�ēG�̃��f����ύX
	if (knockBackPowerLevel == 0) { enemyObject->SetModel(straighterModel[0]); }
	else if (knockBackPowerLevel == 1) { enemyObject->SetModel(straighterModel[1]); }
	else if (knockBackPowerLevel == 2) { enemyObject->SetModel(straighterModel[2]); }
	else if (knockBackPowerLevel >= 3) { enemyObject->SetModel(straighterModel[3]); }
}
