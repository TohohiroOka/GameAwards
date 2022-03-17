#include "Garutata.h"

Garutata *Garutata::Create(Model *model, XMFLOAT3 position, XMFLOAT3 targetPosition)
{
	//�C���X�^���X�𐶐�
	Garutata *instance = new Garutata();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	//�ړ��p�x��ݒ�
	instance->SetMoveAngle(targetPosition);

	return instance;
}

bool Garutata::Initialize(Model *model, XMFLOAT3 position)
{
	//�U�R�����I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(position);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 6, 6, 1 });

	//���f�����Z�b�g
	if (model) {
		enemyObject->SetModel(model);
	}

	//�F��΂ɂ���
	//enemyObject->SetColor({ 0, 1, 0, 1 });

	return true;
}

void Garutata::Update()
{
	//�ʏ펞�̈ړ�
	if (isAlive)
	{
		//�ړ����x�Ɉړ��p�x����Z���č��W���X�V
		float moveSpeed = 0.05f;
		XMFLOAT3 pos = enemyObject->GetPosition();
		pos.x += moveSpeed * cosf(moveAngle);
		pos.y += moveSpeed * sinf(moveAngle);
		//�X�V�������W���Z�b�g
		enemyObject->SetPosition(pos);

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
	//�m�b�N�o�b�N�ł̈ړ�
	else
	{
		KnockBack();
	}

	//�I�u�W�F�N�g�X�V
	enemyObject->Update();
}

void Garutata::SetMoveAngle(XMFLOAT3 targetPosition)
{
	//�ړ��p�x��ݒ肷��i�W�I�Ɍ������Ĉ꒼���j
	XMFLOAT3 position = enemyObject->GetPosition(); 
	float radian = atan2f(targetPosition.y - position.y, targetPosition.x - position.x);
	moveAngle = radian;
}
