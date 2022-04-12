#include "EnemyCircle.h"

EnemyCircle* EnemyCircle::Create(Model* model, GaruEnemy* garuEnemy)
{
	//�C���X�^���X�𐶐�
	EnemyCircle* instance = new EnemyCircle();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, garuEnemy)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool EnemyCircle::Initialize(Model* model, GaruEnemy* garuEnemy)
{
	//�~�I�u�W�F�N�g����
	circleObject = Object3d::Create();
	if (circleObject == nullptr) {
		return false;
	}

	//�G�̏���ێ�
	this->garuEnemy = garuEnemy;

	//���W���Z�b�g �G�̒��S���W�Ɠ���
	XMFLOAT3 pos = garuEnemy->GetPosition();
	circleObject->SetPosition(pos);

	//��̔��a���Z�b�g( �K�����̑傫�� �~�i �|���ꂽ���̒e�̋��� / 4 �j�j
	float baseRadius = garuEnemy->GetScale().x * ((float)garuEnemy->GetKillBulletPower() / 4);
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
