#include "BaseEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "StageEffect.h"

DirectX::XMFLOAT2 BaseEnemy::wallLineMin = { 196, 110 };
DirectX::XMFLOAT2 BaseEnemy::wallLineMax = { 196, 110 };
DirectX::XMFLOAT3 BaseEnemy::targetPos = {};
bool BaseEnemy::isResultMove = false;

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
		//���U���g�V�[���p�̓���
		else if (isResultMove)
		{
			ResultMove();
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

void BaseEnemy::Damage()
{
	//HP��1���炷
	HP--;

	//HP��0�ɂȂ����玀�S
	if (HP <= 0)
	{
		isAlive = false;
	}
}

void BaseEnemy::SetDelete()
{
	//�폜����
	isDelete = true;
}

void BaseEnemy::SetKnockBack(float angle, int powerLevel, float powerMagnification, int shockWaveGroup)
{
	//�m�b�N�o�b�N�Ɏg�p����p�x���Z�b�g
	knockBackAngle = angle;


	//�m�b�N�o�b�N�̋����Ǝ��Ԃ����߂�
	//�v���C���[����o��ʏ�Ռ��g
	if (powerLevel == 1) { knockBackPower = 5.0f * powerMagnification; knockBackTime = (int)(40 * powerMagnification); }
	//����Ռ��g1�`3�i�K
	else if (powerLevel == 2) { knockBackPower = 6.0f * powerMagnification; knockBackTime = (int)(50 * powerMagnification); }
	else if (powerLevel == 3) { knockBackPower = 8.0f * powerMagnification; knockBackTime = (int)(60 * powerMagnification); }
	else if (powerLevel == 4) { knockBackPower = 12.0f * powerMagnification; knockBackTime = (int)(70 * powerMagnification); }

	//�m�b�N�o�b�N�^�C�}�[��������
	knockBackTimer = 0;

	//�ړ��p�x�ύX�J�n���x���Z�b�g
	changeAngleSpeed = 53 * powerMagnification;

	//�Ō�ɓ��������Ռ��g�̎�ނ��X�V
	lastCollisionShockWave = shockWaveGroup;

	//�m�b�N�o�b�N��Ԃɂ���
	isKnockBack = true;

	//���o���Z�b�g
	StageEffect::SetPushEnemy(enemyObject->GetPosition(), angle, enemyObject->GetColor());
}

bool BaseEnemy::IsCollisionWall()
{
	//�g�ɂԂ����Ă�����true��Ԃ�
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	if (pos.x <= wallLineMin.x + size.x / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.x = wallLineMin.x + size.x / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	else if (pos.x >= wallLineMax.x - size.x / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.x = wallLineMax.x - size.x / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	if (pos.y <= wallLineMin.y + size.y / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.y = wallLineMin.y + size.y / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	else if (pos.y >= wallLineMax.y - size.y / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.y = wallLineMax.y - size.y / 2;
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
	vel.x = cosf(moveAngle);
	vel.y = sinf(moveAngle);
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

	//�^�C�g�����S��
	if (group == EnemyGroup::TitleLogo)
	{
		//��]������
		float degree = moveDegree;
		degree += changeAngleSpeed;
		if (degree <= 360)
		{
			degree -= 360;
		}
		SetMoveAngle(degree);

		changeAngleSpeed -= 2.0f;
		if (changeAngleSpeed < 0) {
			changeAngleSpeed = 0;
		}
	}
	//�^�C�g�����S�ȊO��
	else
	{
		//�m�b�N�o�b�N���̑O����
		if (knockBackTimer <= knockBackTime / 2)
		{
			//��]������
			float degree = moveDegree;
			degree += changeAngleSpeed;
			if (degree <= 360)
			{
				degree -= 360;
			}
			SetMoveAngle(degree);

			changeAngleSpeed -= 2.0f;
			if (changeAngleSpeed < 0) {
				changeAngleSpeed = 0;
			}
		}
		//�m�b�N�o�b�N���̌㔼��
		else
		{
			//�^�[�Q�b�g�̕����������悤�ɂ���
			float radian = atan2f(targetPos.y - pos.y, targetPos.x - pos.x);
			moveAngle = radian;
			//�I�u�W�F�N�g�̌�����i�s�����ɃZ�b�g ���W�A�����p�x�ɒ����������0�ɒ�������
			float degree = DirectX::XMConvertToDegrees(radian);
			SetMoveAngle(degree - 90);
		}
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (knockBackTimer >= knockBackTime)
	{
		//�m�b�N�o�b�N�I��
		isKnockBack = false;
	}
}

void BaseEnemy::ReflectionX()
{
	//���E���˗p�ɔ��ˊp���Z�b�g����
	float reflectionAngle = 360 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//���x��ύX����
	moveSpeed = 1.5f;
}

void BaseEnemy::ReflectionY()
{
	//�㉺���˗p�ɔ��ˊp���Z�b�g����
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//���x��ύX����
	moveSpeed = 1.5f;
}

