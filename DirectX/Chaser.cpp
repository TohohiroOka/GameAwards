#include "Chaser.h"
#include "Easing.h"

Model* Chaser::chaserModel[Chaser::modelNum] = { nullptr };

Chaser* Chaser::Create(XMFLOAT3 spawnPosition)
{
	//�C���X�^���X�𐶐�
	Chaser* instance = new Chaser();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(spawnPosition, 0)) {
		delete instance;
		assert(0);
	}

	return instance;
}

void Chaser::SetModel(Model* chaserModel1, Model* chaserModel2, Model* chaserModel3, Model* chaserModel4)
{
	//�����̃��f�������ʂŎg�����߃Z�b�g����
	Chaser::chaserModel[0] = chaserModel1;
	Chaser::chaserModel[1] = chaserModel2;
	Chaser::chaserModel[2] = chaserModel3;
	Chaser::chaserModel[3] = chaserModel4;
}

bool Chaser::Initialize(XMFLOAT3 spawnPosition, float moveDegree)
{
	//�����O���[�v��ݒ�
	group = EnemyGroup::Chaser;

	//�I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(spawnPosition);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 8, 8, 1 });

	//���f�����Z�b�g
	if (chaserModel[0]) {
		enemyObject->SetModel(chaserModel[0]);
	}

	//�U���͂��Z�b�g
	power = 8;

	return true;
}

void Chaser::Update()
{
	if (isAlive)
	{
		if (!isKnockBack)
		{
			//�^�[�Q�b�g�Ɍ����ĒǏ]
			SetAngleForTarget(targetPos);
		}
	}

	BaseEnemy::Update();
}

void Chaser::SetKnockBack(float angle, int powerLevel, int shockWaveGroup)
{
	BaseEnemy::SetKnockBack(angle, powerLevel, shockWaveGroup);

	//�G�̃��f����ύX
	if (enemyObject->GetModel() != chaserModel[3])
	{
		if (knockBackPowerLevel == 1) { enemyObject->SetModel(chaserModel[1]); }
		else if (knockBackPowerLevel == 2) { enemyObject->SetModel(chaserModel[2]); }
		else if (knockBackPowerLevel >= 3) { enemyObject->SetModel(chaserModel[3]); }
	}
}

void Chaser::Move()
{
	//�ړ����x�ύX
	ChangeMoveSpeed();

	//�ړ����x�Ɉړ��p�x����Z���č��W���X�V
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x += moveSpeed * cosf(moveAngle);
	pos.y += moveSpeed * sinf(moveAngle);

	//�X�V�������W���Z�b�g
	enemyObject->SetPosition(pos);
}

void Chaser::SetAngleForTarget(XMFLOAT3 targetPosition)
{
	//�ړ��p�x��ݒ肷��i�W�I�Ɍ������Ĉ꒼���j
	XMFLOAT3 position = enemyObject->GetPosition();
	float radian = atan2f(targetPosition.y - position.y, targetPosition.x - position.x);
	moveAngle = radian;

	//�I�u�W�F�N�g�̌�����i�s�����ɃZ�b�g ���W�A�����p�x�ɒ����������0�ɒ�������
	float degree = DirectX::XMConvertToDegrees(radian);
	XMFLOAT3 rota = { 0, 0, degree - 90 };
	enemyObject->SetRotation(rota);
}

void Chaser::ChangeMoveSpeed()
{
	//1���[�v���s������
	const int roopTime = 60;

	//�ړ����x�ύX�^�C�}�[�X�V
	moveSpeedTimer++;

	//�ړ����s������
	const int moveSpeedTime = 55;
	if (moveSpeedTimer <= moveSpeedTime)
	{
		//�C�[�W���O�v�Z�p�̎���
		float easeTimer = (float)moveSpeedTimer / moveSpeedTime;
		//�ړ����x�ύX��̑��x
		moveSpeed = Easing::OutQuint(3.0f, 0, easeTimer);
	}
	//��~���s��
	else
	{
		//�^�C�}�[���w�肵�����ԂɂȂ�����
		if (moveSpeedTimer >= roopTime)
		{
			//�^�C�}�[�������ɖ߂�
			moveSpeedTimer = 0;
		}
	}
}
