#include "Division.h"

Model* Division::divisionModel[Division::modelNum] = { nullptr };


Division* Division::Create(XMFLOAT3 spawnPosition, float moveDegree)
{
	//�C���X�^���X�𐶐�
	Division* instance = new Division();
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

void Division::SetModel(Model* divisionModel1, Model* divisionModel2, Model* divisionModel3, Model* divisionModel4)
{
	//�����̃��f�������ʂŎg�����߃Z�b�g����
	Division::divisionModel[0] = divisionModel1;
	Division::divisionModel[1] = divisionModel2;
	Division::divisionModel[2] = divisionModel3;
	Division::divisionModel[3] = divisionModel4;
}

bool Division::Initialize(XMFLOAT3 spawnPosition, float moveDegree)
{
	//�����O���[�v��ݒ�
	group = EnemyGroup::Division;

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
	enemyObject->SetScale({ 8, 8, 1 });

	//���f�����Z�b�g
	if (divisionModel[0]) {
		enemyObject->SetModel(divisionModel[0]);
	}

	//�U���͂��Z�b�g
	power = 7;

	//�ړ��p�x���Z�b�g
	SetMoveAngle(moveDegree);

	return true;
}

void Division::Update()
{
	//�����Ă���Ƃ�
	if (isAlive)
	{
		//�������ԍX�V
		AliveTimeUpdate();
	}

	BaseEnemy::Update();
}

void Division::SetKnockBack(float angle, int powerLevel, int shockWaveGroup)
{
	//�����������Ԃ������l�ɖ߂�
	aliveTimer = 0;

	BaseEnemy::SetKnockBack(angle, powerLevel, shockWaveGroup);

	//�G�̃��f����ύX
	if (enemyObject->GetModel() != divisionModel[3])
	{
		if (knockBackPowerLevel == 1) { enemyObject->SetModel(divisionModel[1]); }
		else if (knockBackPowerLevel == 2) { enemyObject->SetModel(divisionModel[2]); }
		else if (knockBackPowerLevel >= 3) { enemyObject->SetModel(divisionModel[3]); }
	}
}

void Division::Move()
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

bool Division::CheckInScreen()
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

void Division::ReflectionX()
{
	//���E���˗p�ɔ��ˊp���Z�b�g����
	float reflectionAngle = 360 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//���x��ύX����
	moveSpeed = 1.5f;
}

void Division::ReflectionY()
{
	//�㉺���˗p�ɔ��ˊp���Z�b�g����
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//���x��ύX����
	moveSpeed = 1.5f;
}

void Division::AliveTimeUpdate()
{
	//�����\����
	const int aliveTime = 300;

	//�^�C�}�[�X�V
	aliveTimer++;

	//�^�C�}�[���w�莞�Ԃɓ��B������
	if (aliveTimer >= aliveTime)
	{
		//�폜����
		SetDelete();

		//�V���ɓG�𐶐�����
		isCreateEnemy = true;
	}
}
