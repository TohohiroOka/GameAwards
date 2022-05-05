#include "BaseEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "StageEffect.h"

DirectX::XMFLOAT2 BaseEnemy::wallLine = { 196, 110 };

BaseEnemy::~BaseEnemy()
{
	safe_delete(enemyObject);
}

void BaseEnemy::Update()
{
	//�����Ă���ꍇ
	if (isAlive)
	{
		//�m�b�N�o�b�N
		if (isKnockBack)
		{
			KnockBack();
		}
		//�m�b�N�o�b�N���ȊO�͈ړ�
		else
		{
			Move();
		}
	}
	//���S�����ꍇ
	else
	{
		//�m�b�N�o�b�N��̃G�t�F�N�g����
		effectCount--;
		//�G�t�F�N�g���Ԃ�0�ȉ��ɂȂ�����
		if (effectCount <= 0)
		{
			//���݂��Ȃ��Ȃ�
			SetDelete();
		}
	}

	//�G�I�u�W�F�N�g�X�V
	enemyObject->Update();
}

void BaseEnemy::Draw()
{
	//�G�I�u�W�F�N�g�`��
	enemyObject->Draw();
}

void BaseEnemy::Dead()
{
	//�G���|���ꂽ�Ƃ��̃G�t�F�N�g
	effectCount = StageEffect::SetEnemeyDead(enemyObject->GetPosition());

	//���S��Ԃɂ���
	isAlive = false;
}

void BaseEnemy::SetDelete()
{
	//�폜����
	isDelete = true;
}

void BaseEnemy::SetKnockBack(float angle, int powerLevel, int shockWaveGroup)
{
	//�m�b�N�o�b�N�����A�O�񓖂������Ռ��g�̎�ނ��|�C�̂Ă��A
	//���񓖂������Ռ��g�̎�ނ��v���C���[����o�鎩���Ռ��g�̏ꍇ�̂�
	//�K���ǂ܂Ő�����΂��ō����x���̃m�b�N�o�b�N���J�n
	if (isKnockBack && lastCollisionShockWave == 2 && shockWaveGroup == 1)
	{
		isKnockBackMax = true;
	}

	//�m�b�N�o�b�N�Ɏg�p����p�x���Z�b�g
	knockBackAngle = angle;

	//�m�b�N�o�b�N�Ɏg�p���鋭�����Z�b�g
	if (isKnockBack)
	{
		knockBackPowerLevel = 5;
	}
	else if (powerLevel <= 3)
	{
		//�Ռ��g�ɓ����邽�тɐ�����ԈЗ͂��グ��
		knockBackPowerLevel += powerLevel;
		//�m�b�N�o�b�N�̋����͏�����z���Ȃ�
		const int powerLevelMax = 3;
		if (knockBackPowerLevel >= powerLevelMax)
		{
			knockBackPowerLevel = powerLevelMax;
		}
	}
	//�ō��З͂̏Ռ��g�ɓ��������Ƃ��̂ݏ�����z����
	else if (powerLevel == 4)
	{
		knockBackPowerLevel = powerLevel;
	}

	//�m�b�N�o�b�N�̋����Ǝ��Ԃ����߂�
	if (knockBackPowerLevel == 1) { knockBackPower = 1; knockBackTime = 30; }
	else if (knockBackPowerLevel == 2) { knockBackPower = 2; knockBackTime = 35; }
	else if (knockBackPowerLevel == 3) { knockBackPower = 3; knockBackTime = 40; }
	else if (knockBackPowerLevel == 4) { knockBackPower = 4; knockBackTime = 45; }
	else if (knockBackPowerLevel == 5) { knockBackPower = 6; knockBackTime = 50; }

	//�G�̐F��ύX
	if (isKnockBackMax) { enemyObject->SetColor({ 0, 1, 1, 1 }); }
	else if (knockBackPowerLevel == 1) { enemyObject->SetColor({ 0, 1, 0, 1 }); }
	else if (knockBackPowerLevel == 2) { enemyObject->SetColor({ 1, 1, 0, 1 }); }
	else if (knockBackPowerLevel >= 3) { enemyObject->SetColor({ 1, 0, 0, 1 }); }

	//�m�b�N�o�b�N�^�C�}�[��������
	knockBackTimer = 0;

	//�Ō�ɓ��������Ռ��g�̎�ނ��X�V
	lastCollisionShockWave = shockWaveGroup;

	//�m�b�N�o�b�N��Ԃɂ���
	isKnockBack = true;

	//�G�t�F�N�g�̃Z�b�g
	StageEffect::SetPushEnemy(enemyObject->GetPosition(), angle, enemyObject->GetColor());
}

bool BaseEnemy::IsCollisionWall(XMFLOAT2 wallLine)
{
	//�g�ɂԂ����Ă�����true��Ԃ�
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	if (pos.x <= -wallLine.x + size.x / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.x = -wallLine.x + size.x / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	else if (pos.x >= wallLine.x - size.x / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.x = wallLine.x - size.x / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	if (pos.y <= -wallLine.y + size.y / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.y = -wallLine.y + size.y / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	else if (pos.y >= wallLine.y - size.y / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.y = wallLine.y - size.y / 2;
		enemyObject->SetPosition(pos);
		return true;
	}

	//�������Ă��Ȃ��ꍇ��false��Ԃ�
	return false;
}

void BaseEnemy::SetMoveAngle(float moveDegree)
{
	//�������0�ɂ��邽��90�x�X����
	this->moveDegree = moveDegree;

	//�x�������W�A���ɒ���
	moveAngle = DirectX::XMConvertToRadians(this->moveDegree + 90);

	//�I�u�W�F�N�g�̌�����i�s�����ɃZ�b�g
	XMFLOAT3 rota = { 0, 0, this->moveDegree };
	enemyObject->SetRotation(rota);

	//�ړ��ʂ��Z�b�g
	vel.x = moveSpeed * cosf(moveAngle);
	vel.y = moveSpeed * sinf(moveAngle);
}

void BaseEnemy::KnockBack()
{
	//�m�b�N�o�b�N�^�C�}�[�X�V
	knockBackTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)knockBackTimer / knockBackTime;
	//�m�b�N�o�b�N��̑��x
	const float knockBackStartSpeed = 4.0f;
	float knockBackSpeed = Easing::OutCubic(knockBackStartSpeed, 0, easeTimer);

	//���W���X�V
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x += knockBackSpeed * cosf(knockBackAngle) * knockBackPower;
	pos.y += knockBackSpeed * sinf(knockBackAngle) * knockBackPower;
	//�X�V�������W���Z�b�g
	enemyObject->SetPosition(pos);


	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (knockBackTimer >= knockBackTime)
	{
		//�m�b�N�o�b�N�I��
		isKnockBack = false;
	}
}

