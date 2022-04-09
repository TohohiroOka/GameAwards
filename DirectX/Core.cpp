#include "Core.h"
#include "SafeDelete.h"
#include "Easing.h"

Core* Core::Create(Model* model)
{
	//�C���X�^���X�𐶐�
	Core* instance = new Core();
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

Core::~Core()
{
	safe_delete(coreObject);
}

bool Core::Initialize(Model* model)
{
	//�R�A�I�u�W�F�N�g����
	coreObject = Object3d::Create();
	if (coreObject == nullptr) {
		return false;
	}

	//�����n�_�Ƒ傫�����Z�b�g
	coreObject->SetPosition({ 0, -1000, 0 });
	coreObject->SetScale({ 5, 5, 1 });

	//�R�A�̃��f�����Z�b�g
	if (coreObject) {
		coreObject->SetModel(model);
	}

	return true;
}

void Core::Update()
{
	//�X�|�[�����̏���
	if (isDuringSpawn)
	{
		//�R�A���X�|�[��
		Spawn();
	}

	//�I�u�W�F�N�g�`�X�V
	coreObject->Update();
}

void Core::Draw()
{
	//�I�u�W�F�N�g�`��
	coreObject->Draw();
}

void Core::Damage(int damagePower)
{
	//��������Ă��Ȃ��U���͖���������
	if (damagePower <= 10) { return; }

	//�������ł��Ȃ������ꍇ���S������
	Dead();

	coreObject->SetColor({ 1, 0, 0, 1 });
}

void Core::Dead()
{
	//���S��Ԃɂ���
	isAlive = false;
}

void Core::SetSpawn(XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//�X�|�[�����̍��W�ƈړ���̍��W���Z�b�g
	this->spawnPosition = spawnPosition;
	this->stayPosition = stayPosition;

	//�X�|�[����Ԃɂ���
	isDuringSpawn = true;
}

void Core::Spawn()
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
	//�X�V�������W���Z�b�g
	coreObject->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (spawnTimer >= spawnTime)
	{
		//�X�|�[���I��
		isDuringSpawn = false;
	}
}