#include "Garuta.h"

Garuta *Garuta::Create(Model *model, XMFLOAT3 position, float moveDegree)
{
	//�C���X�^���X�𐶐�
	Garuta *instance = new Garuta();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	//�ړ��p�x��ݒ�
	instance->SetMoveAngle(moveDegree);

	return instance;
}

bool Garuta::Initialize(Model *model, XMFLOAT3 position)
{
	//�U�R�����I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(position);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 3, 3, 1 });

	//���f�����Z�b�g
	if (model) {
		enemyObject->SetModel(model);
	}

	//�F��Ԃ�����
	//enemyObject->SetColor({ 1, 0, 0, 1 });

	return true;
}

void Garuta::Update()
{
	//�ʏ펞�̈ړ�
	if (isAlive)
	{
		//�ړ����x�Ɉړ��p�x����Z���č��W���X�V
		float moveSpeed = 0.05f;
		XMFLOAT3 pos = enemyObject->GetPosition();
		pos.x -= moveSpeed * sinf(moveAngle);
		pos.y += moveSpeed * cosf(moveAngle);
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

void Garuta::SetMoveAngle(float moveDegree)
{
	//�����̊p�x�����W�A���ɒ����Ĉړ��p�x���Z�b�g����
	moveAngle = DirectX::XMConvertToRadians(moveDegree);
}