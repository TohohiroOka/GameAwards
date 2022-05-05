#include "Division.h"


Division* Division::Create(Model* model, XMFLOAT3 spawnPosition, float moveDegree)
{
	//�C���X�^���X�𐶐�
	Division* instance = new Division();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, spawnPosition, moveDegree)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool Division::Initialize(Model* model, XMFLOAT3 spawnPosition, float moveDegree)
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
	enemyObject->SetScale({ 4, 4, 1 });

	//���f�����Z�b�g
	if (model) {
		enemyObject->SetModel(model);
	}

	//�F�������
	enemyObject->SetColor({ 0, 0, 1, 1 });

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
	pos.x += vel.x;
	pos.y += vel.y;

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
}

void Division::ReflectionY()
{
	//�㉺���˗p�ɔ��ˊp���Z�b�g����
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);
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
