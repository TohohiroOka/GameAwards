#include "GaruEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "StageEffect.h"

GaruEnemy::~GaruEnemy()
{
	safe_delete(enemyObject);
	safe_delete(stayPointObject);
}

void GaruEnemy::Update()
{
	//���݂�����ꍇ
	if (isExistence)
	{
		//�X�|�[�����̏���
		if (isDuringSpawn)
		{
			//�G���X�|�[��
			Spawn();

			//��~���W�I�u�W�F�N�g���X�V
			stayPointObject->Update();
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
				//�m�b�N�o�b�N��̃G�t�F�N�g����
				if (isEffect)
				{
					effectCount--;
					//�G�t�F�N�g���Ԃ�0�ȉ��ɂȂ�����
					if (effectCount <= 0)
					{
						//���݂��Ȃ��Ȃ�
						isExistence = false;
						//�G�t�F�N�g���I��
						isEffect = false;
						//�m�b�N�o�b�N���I������u��
						triggerEndKnockBack = true;
					}
				}
				//�m�b�N�o�b�N����
				else
				{
					KnockBack();
				}
			}
		}
	}

	//�G�I�u�W�F�N�g�X�V
	enemyObject->Update();
}

void GaruEnemy::Draw()
{
	//�G�I�u�W�F�N�g�`��
	enemyObject->Draw();

	//�X�|�[�����ȊO�Ȃ��΂�
	if (!isDuringSpawn) { return; }
	//��~���W�I�u�W�F�N�g��`��
	stayPointObject->Draw();
}

void GaruEnemy::Damage(int damagePower)
{
	//�����Ŏw�肵�������̕�HP�����炷
	HP -= damagePower;
}

void GaruEnemy::Dead()
{
	//���S��Ԃɂ���
	isAlive = false;
}

void GaruEnemy::SetDelete()
{
	//�폜����
	isDelete = true;
}

void GaruEnemy::SetKnockBack(float angle, int power)
{
	//�m�b�N�o�b�N�Ɏg�p����p�x�Ƌ������Z�b�g
	this->knockBackAngle = angle;
	this->killBulletPower = power;

	//���S��Ԃɂ���
	Dead();
}

bool GaruEnemy::IsCollisionFrame(XMFLOAT2 frameLine)
{
	//�g�ɂԂ����Ă�����true��Ԃ�
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	if (pos.x <= -frameLine.x - size.x / 2) { return true; }
	if (pos.x >= frameLine.x + size.x / 2) { return true; }
	if (pos.y <= -frameLine.y - size.y / 2) { return true; }
	if (pos.y >= frameLine.y + size.y / 2) { return true; }

	//�������Ă��Ȃ��ꍇ��false��Ԃ�
	return false;
}

bool GaruEnemy::TriggerEndKnockBack()
{
	//�m�b�N�o�b�N���I������u�ԂȂ�
	if (triggerEndKnockBack)
	{
		//���̃t���[���̓g���K�[�ł͂Ȃ��̂�false�ɖ߂�
		triggerEndKnockBack = false;

		return true;
	}

	//�m�b�N�o�b�N���I������u�Ԃł͂Ȃ�
	return false;
}

void GaruEnemy::Spawn()
{
	//�X�|�[�����s������
	const int spawnTime = 150;

	//�X�|�[���^�C�}�[�X�V
	spawnTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)spawnTimer / spawnTime;
	//�X�|�[�����̉�ʊO����̍��W�ړ�
	XMFLOAT3 pos = {};
	pos.x = Easing::OutCubic(spawnPosition.x, stayPosition.x, easeTimer);
	pos.y = Easing::OutCubic(spawnPosition.y, stayPosition.y, easeTimer);
	//�X�V�����A���t�@�l���Z�b�g
	enemyObject->SetPosition(pos);

	//�X�|�[�����̐F�A���t�@�l
	float colorAlpha = Easing::OutCubic(0.0f, 1.0f, easeTimer);
	//�X�V�����A���t�@�l���Z�b�g
	enemyObject->SetColor({ 1, 1, 1, colorAlpha });

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (spawnTimer >= spawnTime)
	{
		//�X�|�[���I��
		isDuringSpawn = false;
	}
}

void GaruEnemy::ShotBullet()
{
	//�e�͖��t���[�����˂��Ȃ��̂�false�ɖ߂��Ă���
	isBulletShot = false;
	//�e���˃^�C�}�[���X�V����
	bulletShotTimer++;
	//�e���˃^�C�}�[����莞�Ԃ܂ŃJ�E���g���ꂽ��
	const int bulletInterval = 200;
	if (bulletShotTimer >= bulletInterval)
	{
		//�e���˃^�C�}�[������
		bulletShotTimer = 0;

		//�e����
		isBulletShot = true;
	}
}

void GaruEnemy::KnockBack()
{
	//�m�b�N�o�b�N���s������
	const int knockBackTime = 20;

	//�m�b�N�o�b�N�^�C�}�[�X�V
	knockBackTimer++;

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

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (knockBackTimer >= knockBackTime)
	{
		//�G���|���ꂽ�Ƃ��̃G�t�F�N�g
		effectCount = StageEffect::SetEnemeyDead(enemyObject->GetPosition());
		isEffect = true;
	}
}

void GaruEnemy::Escape()
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

		//�����p�^�C�}�[�X�V
		escapeTimer++;

		//�C�[�W���O�v�Z�p�̎���
		float easeTimer = (float)escapeTimer / escapeTime;
		//�������̐F�A���t�@�l
		float colorAlpha = Easing::OutCubic(1.0f, 0.0f, easeTimer);

		//�X�V�����A���t�@�l���Z�b�g
		enemyObject->SetColor({ 1, 1, 1, colorAlpha });

		//�^�C�}�[���w�肵�����ԂɂȂ�����
		if (escapeTimer >= escapeTime)
		{
			//�G���폜����
			SetDelete();
		}
	}
}

