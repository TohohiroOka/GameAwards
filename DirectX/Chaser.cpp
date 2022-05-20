#include "Chaser.h"
#include "Easing.h"

Model* Chaser::chaserModel = nullptr;

Chaser* Chaser::Create(XMFLOAT3 spawnPosition, float moveDegree, bool isParent)
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

	if (isParent) {
		instance->SetSpawnParent(moveDegree);
	}

	return instance;
}

void Chaser::SetModel(Model* chaserModel)
{
	//�����̃��f�������ʂŎg�����߃Z�b�g����
	Chaser::chaserModel = chaserModel;
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
	enemyObject->SetScale({ 5, 5, 1 });

	//���f�����Z�b�g
	if (chaserModel) {
		enemyObject->SetModel(chaserModel);
	}

	return true;
}

void Chaser::SetSpawnParent(float moveDegree)
{
	//�e���琶�܂ꂽ����������
	isParentMove = true;

	//�ړ��p�x���Z�b�g
	BaseEnemy::SetMoveAngle(moveDegree);
}

void Chaser::Move()
{
	//�e���琶�܂ꂽ����(���i)
	if (isParentMove)
	{
		MoveStraight();

		//���i���鎞�Ԃ��v��
		CountMoveStraightTime();
	}
	//�^�[�Q�b�g�ɒǏ]
	else
	{
		MoveChase();
	}
}

void Chaser::ResultMove()
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
}

bool Chaser::CheckInScreen()
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

void Chaser::MoveStraight()
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

void Chaser::CountMoveStraightTime()
{
	//���i���鎞��
	const int moveStraightTime = 30;

	//���i�^�C�}�[���X�V
	moveStraightTimer++;

	//�^�C�}�[���w�肵�����Ԃɓ��B������Ǐ]���n�߂�
	if (moveStraightTimer >= moveStraightTime)
	{
		isParentMove = false;
	}
}

void Chaser::MoveChase()
{
	//�^�[�Q�b�g�Ɍ����ĒǏ]
	SetAngleForTarget(targetPos);

	//�ړ����x�ύX
	ChangeMoveSpeed();

	//�ړ����x�Ɉړ��p�x����Z���č��W���X�V
	XMFLOAT3 pos = enemyObject->GetPosition();

	//�^�[�Q�b�g���W�t�߂ɋ��Ȃ���ΒǏ]�œ�����
	const float chaseDistance = 1.0f;
	if (!(targetPos.x - chaseDistance < pos.x && targetPos.x + chaseDistance > pos.x &&
		targetPos.y - chaseDistance < pos.y && targetPos.y + chaseDistance > pos.y))
	{
		pos.x += moveSpeed * cosf(moveAngle);
		pos.y += moveSpeed * sinf(moveAngle);

		//�X�V�������W���Z�b�g
		enemyObject->SetPosition(pos);
	}

	//�I�u�W�F�N�g�̌�����i�s�����ɃZ�b�g ���W�A�����p�x�ɒ����������0�ɒ�������
	float degree = DirectX::XMConvertToDegrees(moveAngle);
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
