#include "EnemyCircle.h"

EnemyCircle *EnemyCircle::Create(Model *model, GaruEnemy *enemy)
{
	//�C���X�^���X�𐶐�
	EnemyCircle *instance = new EnemyCircle();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, enemy)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool EnemyCircle::Initialize(Model *model, GaruEnemy *enemy)
{
	//�~�I�u�W�F�N�g����
	circleObject = Object3d::Create();
	if (circleObject == nullptr) {
		return false;
	}

	//�G�̏���ێ�
	this->enemy = enemy;

	//���W���Z�b�g �G�̒��S���W�Ɠ���
	XMFLOAT3 pos = enemy->GetPosition();
	circleObject->SetPosition(pos);

	//��̔��a���Z�b�g( �K�����̑傫�� �~�i �|���ꂽ���̒e�̋��� / 4 �j�j
	float baseRadius = enemy->GetScale().x * ((float)enemy->GetKillBulletPower() / 2.5);
	this->baseRadius = baseRadius;

	//�ύX�O�̉~�̃T�C�Y���Z�b�g
	changeRadiusStart = 0;
	//�ύX��̉~�̃T�C�Y���Z�b�g
	changeRadiusEnd = baseRadius;
	circleObject->SetScale({ changeRadiusStart, changeRadiusStart, 1 });

	//�~�̃T�C�Y��ύX��Ԃɂ���
	isChangeRadius = true;

	if (model) {
		circleObject->SetModel(model);
	}

	//�F��΂ɂ���
	circleObject->SetColor({ 0.2f, 1, 0.2f, 0.4f });

	return true;
}
