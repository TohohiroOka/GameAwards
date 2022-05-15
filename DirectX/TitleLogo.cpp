#include "TitleLogo.h"
#include "Easing.h"

Model* TitleLogo::titleLogoModel = nullptr;
bool TitleLogo::isSpawnEnd = false;
const DirectX::XMFLOAT3 TitleLogo::spawnPos = { 0, 150, 0 };
const DirectX::XMFLOAT3 TitleLogo::stayPos = { 0, 40, 0 };

TitleLogo* TitleLogo::Create()
{
	//�C���X�^���X�𐶐�
	TitleLogo* instance = new TitleLogo();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize({}, 0)) {
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

bool TitleLogo::GetTriggerSpawnEnd()
{
	if (isSpawnEnd)
	{
		//�g���K�[�Ȃ̂�false�ɖ߂�
		isSpawnEnd = false;

		return true;
	}

	return false;
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

	//�X�|�[�����W�Z�b�g
	enemyObject->SetPosition(spawnPos);

	//�傫�����Z�b�g
	enemyObject->SetScale({ 20, 20, 1 });

	//���f�����Z�b�g
	if (titleLogoModel) {
		enemyObject->SetModel(titleLogoModel);
	}


	return true;
}

void TitleLogo::Move()
{
	//�X�|�[�����ȊO�͔�����
	if (!isSpawn) { return; }

	//�X�|�[�����s������
	const int spawnTime = 90;

	//�^�C�}�[���X�V
	spawnTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)spawnTimer / spawnTime;

	//�C�[�W���O�Ń^�C�g�����S�𓮂���
	XMFLOAT3 logoPos = enemyObject->GetPosition();
	logoPos.y = Easing::OutBounce(spawnPos.y, stayPos.y, easeTimer);
	enemyObject->SetPosition(logoPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (spawnTimer >= spawnTime)
	{
		//�X�|�[���I��
		isSpawn = false;
		//�X�|�[������
		isSpawnEnd = true;
	}
}

void TitleLogo::ResultMove()
{
}
