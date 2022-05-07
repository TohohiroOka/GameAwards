#include "TitleLogo.h"

Model* TitleLogo::titleLogoModel = { nullptr };


TitleLogo* TitleLogo::Create(XMFLOAT3 spawnPosition)
{
	//�C���X�^���X�𐶐�
	TitleLogo* instance = new TitleLogo();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(spawnPosition, 0)) {
		delete instance;
		assert(0);
	}

	return instance;
}

void TitleLogo::SetModel(Model* titleLogoModel)
{
	//�����̃��f�������ʂŎg�����߃Z�b�g����
	TitleLogo::titleLogoModel = titleLogoModel;
}

bool TitleLogo::Initialize(XMFLOAT3 spawnPosition, float moveDegree)
{
	//�����O���[�v��ݒ�
	group = EnemyGroup::TitleLogo;

	//�I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(spawnPosition);

	//�傫�����Z�b�g
	enemyObject->SetScale({ 20, 20, 1 });

	//���f�����Z�b�g
	if (titleLogoModel) {
		enemyObject->SetModel(titleLogoModel);
	}

	//�U���͂��Z�b�g
	power = 1000;


	return true;
}

void TitleLogo::Move()
{
}

void TitleLogo::ResultMove()
{
}
