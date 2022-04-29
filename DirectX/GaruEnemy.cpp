#include "GaruEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "StageEffect.h"

DirectX::XMFLOAT2 GaruEnemy::frameLine = { 196, 110 };

GaruEnemy::~GaruEnemy()
{
	safe_delete(enemyObject);
	safe_delete(stayPointObject);
}

void GaruEnemy::Update()
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
			//�m�b�N�o�b�N
			if (isKnockBack)
			{
				KnockBack();
			}
			//�m�b�N�o�b�N���ȊO
			else
			{
				//�e���ˏ���
				ShotBullet();

				//����
				Escape();
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

	//HP��0�ȉ��ɂȂ����玀�S
	if (HP <= 0)
	{
		Dead();
	}
}

void GaruEnemy::Dead()
{
	//�G���|���ꂽ�Ƃ��̃G�t�F�N�g
	effectCount = StageEffect::SetEnemeyDead(enemyObject->GetPosition());

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
	//�m�b�N�o�b�N�񐔂��X�V
	knockBackCount++;

	if (knockBackCount == 1) { enemyObject->SetColor({ 0, 1, 0, 1 }); }
	else if (knockBackCount == 2) { enemyObject->SetColor({ 1, 1, 0, 1 }); }
	else if (knockBackCount >= 3) { enemyObject->SetColor({ 1, 0, 0, 1 }); }

	//�m�b�N�o�b�N�Ɏg�p����p�x�Ƌ������Z�b�g
	knockBackAngle = angle;
	knockBackPower = power * knockBackCount;

	//�m�b�N�o�b�N�^�C�}�[��������
	knockBackTimer = 0;

	//�m�b�N�o�b�N��Ԃɂ���
	isKnockBack = true;

	//�e���ˏ�Ԃ��������Ă���
	isBulletShot = false;
}

bool GaruEnemy::IsCollisionFrame(XMFLOAT2 frameLine)
{
	//�g�ɂԂ����Ă�����true��Ԃ�
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	if (pos.x <= -frameLine.x + size.x / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.x = -frameLine.x + size.x / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	else if (pos.x >= frameLine.x - size.x / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.x = frameLine.x - size.x / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	if (pos.y <= -frameLine.y + size.y / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.y = -frameLine.y + size.y / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	else if (pos.y >= frameLine.y - size.y / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.y = frameLine.y - size.y / 2;
		enemyObject->SetPosition(pos);
		return true;
	}

	//�������Ă��Ȃ��ꍇ��false��Ԃ�
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
	XMFLOAT4 color = enemyObject->GetColor();
	color.w = colorAlpha;
	enemyObject->SetColor(color);

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
	const int knockBackTime = 25 + knockBackCount * 5;

	//�m�b�N�o�b�N�^�C�}�[�X�V
	knockBackTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)knockBackTimer / knockBackTime;
	//�m�b�N�o�b�N��̑��x
	const float knockBackStartSpeed = 5.0f;
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
		XMFLOAT4 color = enemyObject->GetColor();
		color.w = colorAlpha;
		enemyObject->SetColor(color);

		//�^�C�}�[���w�肵�����ԂɂȂ�����
		if (escapeTimer >= escapeTime)
		{
			//�G���폜����
			SetDelete();
		}
	}
}

