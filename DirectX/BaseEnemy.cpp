#include "BaseEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"

BaseEnemy::~BaseEnemy()
{
	safe_delete(enemyObject);
}

void BaseEnemy::Update()
{
	//�X�|�[�����̏���
	if (isDuringSpawn)
	{
		Spawn();
	}
	//�X�|�[�����ȊO�̏���
	else
	{
		//�����Ă���ꍇ
		if (isAlive)
		{
			//�e���ˏ���
			ShotBullet();

			//����
			Escape();
		}
		//���S�����ꍇ
		else
		{
			//�m�b�N�o�b�N
			KnockBack();
		}
	}

	//�I�u�W�F�N�g�X�V
	enemyObject->Update();
}

void BaseEnemy::Draw()
{
	//�I�u�W�F�N�g�`��
	enemyObject->Draw();
}

void BaseEnemy::Damage(int damagePower)
{
	//�����Ŏw�肵�������̕�HP�����炷
	HP -= damagePower;
}

void BaseEnemy::Dead()
{
	//���S��Ԃɂ���
	isAlive = false;
}

void BaseEnemy::SetKnockBack(float angle, int power)
{
	//�m�b�N�o�b�N�Ɏg�p����p�x�Ƌ������Z�b�g
	this->knockBackAngle = angle;
	this->killBulletPower = power;

	//���S��Ԃɂ���
	Dead();
}

void BaseEnemy::Spawn()
{
	//�X�|�[�����s������
	const int spawnTime = 50;
	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)spawnTimer / spawnTime;
	//�X�|�[�����̐F�A���t�@�l
	float colorAlpha = Easing::InCubic(0.0f, 1.0f, easeTimer);

	//�X�V�����A���t�@�l���Z�b�g
	enemyObject->SetColor({ 1, 1, 1, colorAlpha });

	//�X�|�[���^�C�}�[�X�V
	spawnTimer++;
	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (spawnTimer >= spawnTime)
	{
		//�X�|�[���I��
		isDuringSpawn = false;
	}
}

void BaseEnemy::ShotBullet()
{
	//�e�͖��t���[�����˂��Ȃ��̂�false�ɖ߂��Ă���
	isBulletShot = false;
	//�e���˃^�C�}�[���X�V����
	bulletShotTimer++;
	//�e���˃^�C�}�[����莞�Ԃ܂ŃJ�E���g���ꂽ��
	const int bulletInterval = 300;
	if (bulletShotTimer >= bulletInterval)
	{
		//�e���˃^�C�}�[������
		bulletShotTimer = 0;

		//�e����
		isBulletShot = true;
	}
}

void BaseEnemy::KnockBack()
{
	//�m�b�N�o�b�N���s������
	const int knockBackTime = 20;
	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)knockBackTimer / knockBackTime;
	//�m�b�N�o�b�N��̑��x
	const float knockBackStartSpeed = 1.0f;
	float knockBackSpeed = Easing::OutCubic(knockBackStartSpeed, 0, easeTimer);
	int power = killBulletPower / 10;

	//���W���X�V
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x -= knockBackSpeed * sinf(knockBackAngle) * power;
	pos.y += knockBackSpeed * cosf(knockBackAngle) * power;
	//�X�V�������W���Z�b�g
	enemyObject->SetPosition(pos);

	//�m�b�N�o�b�N�^�C�}�[�X�V
	knockBackTimer++;
	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (knockBackTimer >= knockBackTime)
	{
		//�F�𔖂�����
		//enemyObject->SetColor({ 1, 0, 0, 0.5f });

		//���݂���I��
		isExistence = false;
	}
}

void BaseEnemy::Escape()
{
	//�������Ă��Ȃ��ꍇ
	if (!isEscape)
	{
		//���������ԃ^�C�}�[���X�V
		aliveTimer++;
		const int aliveTime = 300;

		//��莞�Ԑ�������
		if (aliveTimer >= aliveTime)
		{
			//�������J�n����
			isEscape = true;
		}
	}

	//�������s���Ă���ꍇ
	else
	{
		//�������s������
		const int escapeTime = 200;
		//�C�[�W���O�v�Z�p�̎���
		float easeTimer = (float)escapeTimer / escapeTime;
		//�������̐F�A���t�@�l
		float colorAlpha = Easing::OutCubic(1.0f, 0.0f, easeTimer);

		//�X�V�����A���t�@�l���Z�b�g
		enemyObject->SetColor({ 1, 1, 1, colorAlpha });

		//�����p�^�C�}�[�X�V
		escapeTimer++;
		//�^�C�}�[���w�肵�����ԂɂȂ�����
		if (escapeTimer >= escapeTime)
		{
			//��������
			isEscapeCompleted = true;
		}
	}
}

