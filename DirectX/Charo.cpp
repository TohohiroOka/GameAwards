#include "Charo.h"
#include "SafeDelete.h"
#include "Easing.h"

Charo* Charo::Create(Model* model, XMFLOAT3 spawnPosition)
{
	//�C���X�^���X�𐶐�
	Charo* instance = new Charo();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, spawnPosition)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Charo::~Charo()
{
	safe_delete(enemyObject);
}

bool Charo::Initialize(Model* model, XMFLOAT3 spawnPosition)
{
	//�I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(spawnPosition);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 4, 4, 1 });

	//���f�����Z�b�g
	if (model) {
		enemyObject->SetModel(model);
	}

	//�F��Ԃ�����
	enemyObject->SetColor({ 1, 1, 1, 1 });

	return true;
}

void Charo::Update(XMFLOAT3 targetPosition)
{
	//�i�s�������Z�b�g
	SetMoveAngle(targetPosition);
	//�ړ�����
	Move();

	//�G�����S���Ă�����폜��Ԃɂ���
	if (!isAlive)
	{
		SetDelete();
	}

	//�G�I�u�W�F�N�g�X�V
	enemyObject->Update();
}

void Charo::Draw()
{
	//�G�I�u�W�F�N�g�`��
	enemyObject->Draw();
}

void Charo::Damage(int damagePower)
{
	//�����Ŏw�肵�������̕�HP�����炷
	HP -= damagePower;

	//HP��0�ȉ��Ȃ�Ύ��S������
	if (HP <= 0)
	{
		Dead();
	}
}

void Charo::Dead()
{
	//���S��Ԃɂ���
	isAlive = false;
}

void Charo::SetDelete()
{
	//�폜����
	isDelete = true;
}

void Charo::SetMoveAngle(XMFLOAT3 targetPosition)
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

void Charo::ChangeMoveSpeed()
{
	//1���[�v���s������
	const int roopTime = 50;

	//�ړ����x�ύX�^�C�}�[�X�V
	moveSpeedTimer++;

	//�ړ����s������
	const int moveSpeedTime = 40;
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

void Charo::Move()
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
