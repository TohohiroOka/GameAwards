#include "Tuff.h"
#include "SafeDelete.h"
#include "Easing.h"

Tuff* Tuff::Create(Model* model)
{
	//�C���X�^���X�𐶐�
	Tuff* instance = new Tuff();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Tuff::~Tuff()
{
	safe_delete(bossObject);
}

bool Tuff::Initialize(Model* model)
{
	//�I�u�W�F�N�g�����Ȃ�
	if (!BossEnemy::Initialize(model))
	{
		return false;
	}

	//�{�X���^�b�t���Ƃ������Ƃ�ݒ�
	name = BossName::Tuff;

	//���W���Z�b�g
	bossObject->SetPosition({ 0, 0, 0 });

	//�傫�����Z�b�g
	bossObject->SetScale({ 4, 4, 1 });

	//�F���Ȃ��ɂ���
	bossObject->SetColor({ 1, 1, 1, 0 });

	return true;
}

void Tuff::Update(XMFLOAT3 targetPosition)
{
	//�X�|�[�����̏���
	if (isDuringSpawn)
	{
		//�G���X�|�[��
		Spawn();
	}
	else
	{
		//�����Ă�����
		if (isAlive)
		{
			//�s���V�[�������Ɉڂ�
			if (isChangeActionScene)
			{
				ChangeAction();
			}

			//�s���p�^�[���ɂ���������������
			if (action == MovementPattern::ThreeWayBullet)
			{
				ShotBulletThreeWay();
			}
			else if (action == MovementPattern::MoveRandomAngle)
			{
				MoveAngle();
			}
			else if (action == MovementPattern::FiveWayBullet)
			{
				ShotBulletFiveWay();
			}
			else if (action == MovementPattern::MoveTopScreen)
			{
				MoveTop();
			}
			else if (action == MovementPattern::FallLockOn)
			{
				LockOnTargetHead(targetPosition);
			}
			else if (action == MovementPattern::Shake)
			{
				ShakeObject();
			}
			else if (action == MovementPattern::Fall)
			{
				FallObject();
			}
			else if (action == MovementPattern::MoveRandomPos)
			{
				MovePos();
			}

			//�g�̃��C�����z��������W��߂�
			CollisitonFrame();
		}
	}

	//�{�X�I�u�W�F�N�g�X�V
	bossObject->Update();
}

void Tuff::Spawn()
{
	//�X�|�[�����s������
	const int spawnTime = 200;

	//�X�|�[���^�C�}�[�X�V
	spawnTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)spawnTimer / spawnTime;

	//�X�|�[�����W
	XMFLOAT3 spawnPosition = { 0, 400, 0 };
	//��~�n�_
	XMFLOAT3 stayPosition = { 0, 20, 0 };

	//�X�|�[�����̉�ʊO����̍��W�ړ�
	XMFLOAT3 pos = {};
	pos.x = Easing::OutCubic(spawnPosition.x, stayPosition.x, easeTimer);
	pos.y = Easing::OutCubic(spawnPosition.y, stayPosition.y, easeTimer);
	//�X�V�������W�l���Z�b�g
	bossObject->SetPosition(pos);

	//�X�|�[�����̐F�A���t�@�l
	float colorAlpha = Easing::InQuint(0.0f, 1.0f, easeTimer);
	//�X�V�����A���t�@�l���Z�b�g
	bossObject->SetColor({ 1, 1, 1, colorAlpha });

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (spawnTimer >= spawnTime)
	{
		//�X�|�[���I��
		isDuringSpawn = false;
	}
}

void Tuff::ChangeAction()
{
	//���̍s���V�[����
	actionScene++;

	//���[�v����
	if (actionScene > 11)
	{
		actionScene = 0;
	}

	//���ꂼ��̃V�[���ԍ��ɂ������s����ݒ�
	if (actionScene == 0) { action = MovementPattern::ThreeWayBullet; }
	else if (actionScene == 1) { action = MovementPattern::MoveRandomAngle; }
	else if (actionScene == 2) { action = MovementPattern::FiveWayBullet; }
	else if (actionScene == 3) { action = MovementPattern::MoveRandomAngle; }
	else if (actionScene == 4) { action = MovementPattern::ThreeWayBullet; }
	else if (actionScene == 5) { action = MovementPattern::MoveRandomAngle; }
	else if (actionScene == 6) { action = MovementPattern::FiveWayBullet; }
	else if (actionScene == 7) { action = MovementPattern::MoveTopScreen; }
	else if (actionScene == 8) { action = MovementPattern::FallLockOn; }
	else if (actionScene == 9) { action = MovementPattern::Shake; }
	else if (actionScene == 10) { action = MovementPattern::Fall; }
	else if (actionScene == 11) { action = MovementPattern::MoveRandomPos; }

	if (action == MovementPattern::MoveRandomAngle)
	{
		//�����_���Ŋp�x���Z�b�g
		SetRandomAngle();
	}
	else if (action == MovementPattern::MoveTopScreen)
	{
		//��ʏ㕔�ւ̈ړ����Z�b�g
		SetMoveTop();
	}
	else if (action == MovementPattern::Shake)
	{
		//�V�F�C�N�����Z�b�g
		SetShake();
	}
	else if (action == MovementPattern::Fall)
	{
		//���������Z�b�g
		SetFall();
	}
	else if (action == MovementPattern::MoveRandomPos)
	{
		//�����_���ō��W���Z�b�g
		SetRandomPos();
	}

	//false�ɖ߂��Ă���
	isChangeActionScene = false;
}

void Tuff::ShotBulletThreeWay()
{
	//�e�͖��t���[�����˂��Ȃ��̂�false�ɖ߂��Ă���
	isBulletShot = false;
	//�e���˃^�C�}�[���X�V����
	bulletShotTimer++;
	//�e���˃^�C�}�[����莞�Ԃ܂ŃJ�E���g���ꂽ��
	const int bulletInterval = 50;
	if (bulletShotTimer >= bulletInterval)
	{
		//�e���˃^�C�}�[������
		bulletShotTimer = 0;
		//�e���������񐔂��X�V
		shotCount++;
		//�e����
		isBulletShot = true;
	}

	//�w�肵���񐔒e�𔭎˂����玟�̍s���Ɉڂ�
	const int shotNum = 3;
	if (shotCount >= shotNum)
	{
		//�e���������񐔂�������
		shotCount = 0;
		//���̍s���ֈڍs
		isChangeActionScene = true;
	}
}

void Tuff::ShotBulletFiveWay()
{
	//�e�͖��t���[�����˂��Ȃ��̂�false�ɖ߂��Ă���
	isBulletShot = false;
	//�e���˃^�C�}�[���X�V����
	bulletShotTimer++;
	//�e���˃^�C�}�[����莞�Ԃ܂ŃJ�E���g���ꂽ��
	const int bulletInterval = 30;
	if (bulletShotTimer >= bulletInterval)
	{
		//�e���˃^�C�}�[������
		bulletShotTimer = 0;
		//�e���������񐔂��X�V
		shotCount++;
		//�e����
		isBulletShot = true;
	}

	//�w�肵���񐔒e�𔭎˂����玟�̍s���Ɉڂ�
	const int shotNum = 2;
	if (shotCount >= shotNum)
	{
		//�e���������񐔂�������
		shotCount = 0;
		//���̍s���ֈڍs
		isChangeActionScene = true;
	}
}

void Tuff::SetRandomAngle()
{
	//�����_����8�������w�肷��
	int random = rand() % 8;
	float angle = 0;
	if (random == 0) { angle = 0; }
	else if (random == 1) { angle = 45; }
	else if (random == 2) { angle = 90; }
	else if (random == 3) { angle = 135; }
	else if (random == 4) { angle = 180; }
	else if (random == 5) { angle = 225; }
	else if (random == 6) { angle = 270; }
	else if (random == 7) { angle = 315; }

	//���ˊp�x��ݒ肷�邽�߂Ɋp�x�����W�A���ɒ���(�E������0�Ȃ̂ŏ�����ɂ��炷)
	moveAngle = DirectX::XMConvertToRadians(angle + 90);
}

void Tuff::MoveAngle()
{
	//���̍s�����������Ԃ��X�V
	actionTimer++;

	moveSpeed += 0.02f;
	XMFLOAT3 pos = bossObject->GetPosition();
	pos.x += moveSpeed * cosf(moveAngle);
	pos.y += moveSpeed * sinf(moveAngle);
	//�X�V�������W���Z�b�g
	bossObject->SetPosition(pos);

	//�s������莞�Ԍo�߂����玟�̍s���Ɉڂ�
	const int actionTime = 40;
	if (actionTimer >= actionTime)
	{
		//�ړ����x��������
		moveSpeed = 0;
		//�s���^�C�}�[��������
		actionTimer = 0;
		//���̍s���ֈڍs
		isChangeActionScene = true;
	}
}

void Tuff::SetMoveTop()
{
	//�C�[�W���O�̃X�^�[�g���W���Z�b�g
	easeStartPos = bossObject->GetPosition();
}

void Tuff::MoveTop()
{
	//�ړ����s������
	const int moveTime = 80;

	//���̍s�����������Ԃ��X�V
	actionTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)actionTimer / moveTime;

	//�X�|�[�����̉�ʊO����̍��W�ړ�
	XMFLOAT3 pos = bossObject->GetPosition();
	//��ʏ㕔�Ɉړ�����悤�ɃZ�b�g
	float topY = frameLine.y - 10;
	pos.y = Easing::OutCubic(easeStartPos.y, topY, easeTimer);
	//�X�V�������W���Z�b�g
	bossObject->SetPosition(pos);

	if (actionTimer >= moveTime)
	{
		//�s���^�C�}�[��������
		actionTimer = 0;
		//���̍s���ֈڍs
		isChangeActionScene = true;
	}
}

void Tuff::LockOnTargetHead(XMFLOAT3 targetPosition)
{
	//���̍s�����������Ԃ��X�V
	actionTimer++;

	//X���̍��W��W�I�ɒǏ]������
	XMFLOAT3 pos = bossObject->GetPosition();

	//�^�[�Q�b�g���{�X��荶���ɂ���΍��ɓ�����
	float speedAccle = 0.12f;
	if (pos.x > targetPosition.x - 2.0f)
	{
		//�^�[�Q�b�g��ǂ��z���Ă��܂����Ƃ��̂ݐ���
		if (moveSpeed > 0)
		{
			speedAccle = 0.36f;
		}

		//���x��ς��邱�Ƃɂ���č��W�𓮂���
		moveSpeed -= speedAccle;
	}
	//�^�[�Q�b�g���{�X���E���ɂ���ΉE�ɓ�����
	if (pos.x < targetPosition.x + 2.0f)
	{
		//�^�[�Q�b�g��ǂ��z���Ă��܂����Ƃ��̂ݐ���
		if (moveSpeed < 0)
		{
			speedAccle = 0.36f;
		}

		//���x��ς��邱�Ƃɂ���č��W�𓮂���
		moveSpeed += speedAccle;
	}
	//�ő呬�x��ݒ�
	const float maxSpeed = 3.0f;
	if (moveSpeed > maxSpeed)
	{
		moveSpeed = maxSpeed;
	}
	else if (moveSpeed < -maxSpeed)
	{
		moveSpeed = -maxSpeed;
	}
	//���x�����W�ɉ��Z���ē�����
	pos.x += moveSpeed;
	bossObject->SetPosition(pos);

	//�s������莞�Ԍo�߂����玟�̍s���Ɉڂ�
	const int lockOnTime = 400;
	if (actionTimer >= lockOnTime)
	{
		//�ړ����x��������
		moveSpeed = 0;
		//�s���^�C�}�[��������
		actionTimer = 0;
		//���̍s���ֈڍs
		isChangeActionScene = true;
	}
}

void Tuff::SetRandomPos()
{
	//�����_���ō��W���w�肷��
	int random = rand() % 1;
	XMFLOAT3 pos = {};
	if (random == 0) { pos.x = 0; pos.y = 0; }

	//�C�[�W���O�̃X�^�[�g���W���Z�b�g
	easeStartPos = bossObject->GetPosition();
	//�C�[�W���O�̃G���h���W���Z�b�g
	easeEndPos = pos;
}

void Tuff::SetShake()
{
	//�V�F�C�N�̊�̍��W���Z�b�g
	shakeBasePos = bossObject->GetPosition();
}

void Tuff::ShakeObject()
{
	//�V�F�C�N���s������
	const int shakeTime = 60;

	//���̍s�����������Ԃ��X�V
	actionTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)actionTimer / shakeTime;

	//�V�F�C�N�̋����ɃC�[�W���O��������
	float shakePower = 0;
	shakePower = Easing::InQuad(0, 1, easeTimer);

	//�V�F�C�N�œ�����
	int randAngle = rand() % 360;
	//�V�F�C�N�p�x��ݒ肷�邽�߂Ɋp�x�����W�A���ɒ���(�E������0�Ȃ̂ŏ�����ɂ��炷)
	float shakeAngle = DirectX::XMConvertToRadians((float)randAngle + 90);

	//�p�x�ɃV�F�C�N�̋�������Z���ăV�F�C�N������
	XMFLOAT3 pos = shakeBasePos;
	pos.x += shakePower * cosf(shakeAngle);
	pos.y += shakePower * sinf(shakeAngle);
	//�X�V�������W���Z�b�g
	bossObject->SetPosition(pos);

	//�s������莞�Ԍo�߂����玟�̍s���Ɉڂ�
	if (actionTimer >= shakeTime)
	{
		//���W����̈ʒu�ɖ߂�
		bossObject->SetPosition(shakeBasePos);
		//�s���^�C�}�[��������
		actionTimer = 0;
		//���̍s���ֈڍs
		isChangeActionScene = true;
	}
}

void Tuff::SetFall()
{
	//�C�[�W���O�̃X�^�[�g���W���Z�b�g
	XMFLOAT3 startPos = bossObject->GetPosition();
	easeStartPos = startPos;
}

void Tuff::FallObject()
{
	//���̍s�����s������
	const int actionTime = 100;
	//���̍s�����������Ԃ��X�V
	actionTimer++;

	//�������s������
	const int fallTime = 40;
	if (actionTimer <= fallTime)
	{
		//�C�[�W���O�v�Z�p�̎���
		float easeTimer = (float)actionTimer / fallTime;

		//�C�[�W���O�ŉ��g�܂ŗ���������
		XMFLOAT3 pos = bossObject->GetPosition();
		pos.y = Easing::InCubic(easeStartPos.y, -frameLine.y, easeTimer);
		//�X�V�������W�l���Z�b�g
		bossObject->SetPosition(pos);

		//�����n�_�ɒ������u��
		if (actionTimer == fallTime)
		{
			//�����Ռ���^����
			isImpact = true;
		}
	}
	//�s������莞�Ԍo�߂����玟�̍s���Ɉڂ�
	if (actionTimer >= actionTime)
	{
		//�s���^�C�}�[��������
		actionTimer = 0;
		//���̍s���ֈڍs
		isChangeActionScene = true;
	}
}

void Tuff::MovePos()
{
	//�ړ����s������
	const int moveTime = 150;

	//���̍s�����������Ԃ��X�V
	actionTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)actionTimer / moveTime;

	//�X�|�[�����̉�ʊO����̍��W�ړ�
	XMFLOAT3 pos = {};
	pos.x = Easing::OutCubic(easeStartPos.x, easeEndPos.x, easeTimer);
	pos.y = Easing::OutCubic(easeStartPos.y, easeEndPos.y, easeTimer);
	//�X�V�������W�l���Z�b�g
	bossObject->SetPosition(pos);

	//�s������莞�Ԍo�߂����玟�̍s���Ɉڂ�
	if (actionTimer >= moveTime)
	{
		//�s���^�C�}�[��������
		actionTimer = 0;
		//���̍s���ֈڍs
		isChangeActionScene = true;
	}
}

void Tuff::CollisitonFrame()
{
	//��ʊO�ɏo�Ȃ��悤�ɂ���
	XMFLOAT3 pos = bossObject->GetPosition();
	XMFLOAT3 size = bossObject->GetScale();
	bool isCollision = false;
	if (pos.x <= -frameLine.x + size.x)
	{
		pos.x = -frameLine.x + size.x;
		isCollision = true;
	}
	else if (pos.x >= frameLine.x - size.x)
	{
		pos.x = frameLine.x - size.x;
		isCollision = true;
	}
	if (pos.y <= -frameLine.y + size.y / 2)
	{
		pos.y = -frameLine.y + size.y / 2;
		isCollision = true;
	}
	else if (pos.y >= frameLine.y - size.y * 1.4f)
	{
		pos.y = frameLine.y - size.y * 1.4f;
		isCollision = true;
	}

	//�g�Ƀ��C���������Ă�����
	if (isCollision)
	{
		//���W���Z�b�g
		bossObject->SetPosition(pos);

		//�ړ����x��������
		moveSpeed = 0;
	}
}
