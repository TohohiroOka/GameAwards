#include "Releaser.h"

Model* Releaser::releaserModel[Releaser::modelNum] = { nullptr };


Releaser* Releaser::Create(XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//�C���X�^���X�𐶐�
	Releaser* instance = new Releaser();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(spawnPosition, 0)) {
		delete instance;
		assert(0);
	}

	//��~���W���Z�b�g
	instance->SetStayPos(stayPosition);

	return instance;
}

void Releaser::SetModel(Model* releaserModel1, Model* releaserModel2, Model* releaserModel3, Model* releaserModel4)
{
	//�����̃��f�������ʂŎg�����߃Z�b�g����
	Releaser::releaserModel[0] = releaserModel1;
	Releaser::releaserModel[1] = releaserModel2;
	Releaser::releaserModel[2] = releaserModel3;
	Releaser::releaserModel[3] = releaserModel4;
}

bool Releaser::Initialize(XMFLOAT3 spawnPosition, float moveDegree)
{
	//�����O���[�v��ݒ�
	group = EnemyGroup::Releaser;

	//�I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(spawnPosition);

	//�傫�����Z�b�g
	enemyObject->SetScale({ 10, 10, 1 });

	//���f�����Z�b�g
	if (releaserModel[0]) {
		enemyObject->SetModel(releaserModel[0]);
	}

	//�U���͂��Z�b�g
	power = 8;


	return true;
}

void Releaser::Update()
{
	//�����Ă���Ƃ�
	if (isAlive)
	{
		if (isReleaseMode)
		{
			//���o���[�h
			ReleaseMode();
		}
	}

	BaseEnemy::Update();
}

void Releaser::SetKnockBack(float angle, int powerLevel, int shockWaveGroup)
{
	//���o�^�C�}�[�������l�ɖ߂�
	releaseTimer = 0;

	BaseEnemy::SetKnockBack(angle, powerLevel, shockWaveGroup);

	//�G�̃��f����ύX
	if (enemyObject->GetModel() != releaserModel[3])
	{
		if (knockBackPowerLevel == 1) { enemyObject->SetModel(releaserModel[1]); }
		else if (knockBackPowerLevel == 2) { enemyObject->SetModel(releaserModel[2]); }
		else if (knockBackPowerLevel >= 3) { enemyObject->SetModel(releaserModel[3]); }
	}
}

void Releaser::Move()
{
	//���o���[�h�̏ꍇ�ړ����Ȃ��̂ŁA������
	if (isReleaseMode) { return; }

	//�ړ����x�Ɉړ��p�x����Z���č��W���X�V
	XMFLOAT3 pos = enemyObject->GetPosition();

	//�ړ��ʂ����W�ɉ��Z���Ĉړ�������
	pos.x += vel.x;
	pos.y += vel.y;

	//�X�V�������W���Z�b�g
	enemyObject->SetPosition(pos);

	//��~���W�ɓ��B������
	if (pos.x >= stayPos.x - 1 && pos.x <= stayPos.x + 1 && pos.y >= stayPos.y - 1 && pos.y <= stayPos.y + 1)
	{
		//���o���[�h�ɐ؂�ւ�
		isReleaseMode = true;
	}
}

void Releaser::SetStayPosAngle()
{
	XMFLOAT3 enemyPos = enemyObject->GetPosition();
	float angle = atan2f(stayPos.y - enemyPos.y, stayPos.x - enemyPos.x);

	//�ړ��������Z�b�g
	moveAngle = angle;

	//�I�u�W�F�N�g�̌�����i�s�����ɃZ�b�g
	XMFLOAT3 rota = { 0, 0, DirectX::XMConvertToDegrees(angle) - 90 };
	enemyObject->SetRotation(rota);

	//�ړ��ʂ��Z�b�g
	vel.x = moveSpeed * cosf(moveAngle);
	vel.y = moveSpeed * sinf(moveAngle);
}

void Releaser::SetStayPos(XMFLOAT3 stayPosition)
{
	//��~���W���Z�b�g����
	stayPos = stayPosition;

	//�ړ��������~���W�Ɍ����Z�b�g
	SetStayPosAngle();
}

void Releaser::KnockBack()
{
	BaseEnemy::KnockBack();

	//�m�b�N�o�b�N����Ă���~���W�������悤�ɂ���
	SetStayPosAngle();
}

void Releaser::ReleaseMode()
{
	//���o���Ԃ�ݒ�
	const int releaseTime = 60;

	//���o�^�C�}�[���X�V
	releaseTimer++;

	//���t���[�����o����킯�ł͂Ȃ��̂�false�ɖ߂��Ă���
	isCreateEnemy = false;

	//�^�C�}�[���w�莞�Ԃɓ��B������
	if (releaseTimer >= releaseTime)
	{
		//�G���o
		Release();
	}
}

void Releaser::Release()
{
	//���o�^�C�}�[��������
	releaseTimer = 0;

	//�G����
	isCreateEnemy = true;

	//�G�̃T�C�Y�����菬��������
	XMFLOAT3 scale = enemyObject->GetScale();
	scale.x -= 0.5f;
	scale.y -= 0.5f;
	enemyObject->SetScale(scale);

	//�G�̍U���͂�1������
	power--;

	//���o�񐔂��X�V
	releaseCount++;

	//���o��6��s������
	if (releaseCount >= 6)
	{
		//�폜
		SetDelete();
	}
}
