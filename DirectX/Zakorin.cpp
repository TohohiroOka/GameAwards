#include "Zakorin.h"
#include "Easing.h"

Zakorin *Zakorin::Create(Model *model, XMFLOAT3 position)
{
	//�C���X�^���X�𐶐�
	Zakorin *instance = new Zakorin();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool Zakorin::Initialize(Model *model, XMFLOAT3 position)
{
	//�U�R�����I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(position);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 4, 4, 1 });

	//���f�����Z�b�g
	if (model) {
		enemyObject->SetModel(model);
	}

	return true;
}

void Zakorin::Update()
{
	////���݂���Ȃ��Ȃ�����X�V���Ȃ�
	//if (!isExistence) return;

	//�ʏ펞�̈ړ�
	if (isAlive)
	{
		XMFLOAT3 pos = enemyObject->GetPosition();
		pos.x += 0.01f;
		pos.y -= 0.01f;
		enemyObject->SetPosition(pos);
	}
	//�m�b�N�o�b�N�ł̈ړ�
	else
	{
		KnockBack();
	}

	//�I�u�W�F�N�g�X�V
	enemyObject->Update();
}

void Zakorin::KnockBack()
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
	if (knockBackTimer > knockBackTime)
	{
		//�F�𔖂�����
		enemyObject->SetColor({ 1, 1, 1, 0.8f });

		//���݂���I��
		isExistence = false;
	}
}
