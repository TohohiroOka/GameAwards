#include "Wall.h"
#include "SafeDelete.h"
#include "Easing.h"

Wall* Wall::Create(Model* model)
{
	//�C���X�^���X�𐶐�
	Wall* instance = new Wall();
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

Wall::~Wall()
{
	safe_delete(wallObject);
}

bool Wall::Initialize(Model* model)
{
	//�ǃI�u�W�F�N�g����
	wallObject = Object3d::Create();
	if (wallObject == nullptr) {
		return false;
	}

	//�����n�_�Ƒ傫���ƐF���Z�b�g
	wallObject->SetPosition({ 0, 0, 0 });
	wallObject->SetScale({ 11.8f, 12.4f, 1 });
	wallObject->SetColor({ 0.8f, 0.3f, 0.1f, 1.0f });

	//���f�����Z�b�g
	if (model) {
		wallObject->SetModel(model);
	}

	//�ŏ���HP��ݒ�
	maxHP = baseMaxHP;
	HP = maxHP;

	return true;
}

void Wall::Update()
{
	//���U���g�V�[���p�ɓ�����
	if (isChangeResult) 
	{
		ChangeResult();
	}

	//�x�e��
	if (isBreakTime) 
	{
		BreakTime();
	}
	//�ǐ�����
	else if (isCreate)
	{
		CreateWall();
	}

	//�I�u�W�F�N�g�X�V
	wallObject->Update();
}

void Wall::Draw()
{
	if (isBreakTime) { return; }

	//�I�u�W�F�N�g�`��
	wallObject->Draw();
}

void Wall::Reset()
{
	//�ǂ̍ő�HP
	maxHP = 20;
	//�ǂ�HP
	HP = maxHP;
	//�x�e���ԃ^�C�}�[
	breakTimer = 0;
	//�x�e����
	isBreakTime = true;
	//�ǐ����^�C�}�[
	createTimer = 0;
	//�ǐ�����
	createCount = 0;
	//�ǐ�������
	isCreate = false;
	//���U���g�V�[���p�ɓ����������ԃ^�C�}�[
	changeResultTimer = 0;
	//�ǂ����U���g�V�[���p�ɓ������Ă��邩
	isChangeResult = false;
	//�󂳂ꂽ��
	isBreak = false;
	//�����Ă��邩
	isAlive = false;
}

void Wall::Damage(int damagePower)
{
	//�����Ŏw�肵�������̕�HP�����炷
	HP -= damagePower;

	//�c��HP�ɉ����ĐF�ύX
	ChangeColor();

	//HP��0�ȉ��ɂȂ�����j��
	if (HP <= 0)
	{
		Break();

		//�����Ă��Ȃ�
		isAlive = false;
	}
}

void Wall::Break()
{
	//�j�󂷂�
	isBreak = true;

	//�x�e��Ԃɂ���
	SetBreakTime();
}

void Wall::SetBreakTime()
{
	//�x�e���ԃ^�C�}�[��������
	breakTimer = 0;

	//�x�e��Ԃɂ���
	isBreakTime = true;
}

void Wall::SetCreateWall()
{
	//�ő�HP��ݒ�
	createCount++;
	maxHP = baseMaxHP + (10 * createCount);
	HP = maxHP;

	//�ǐ��������T�C�Y���Z�b�g
	createStartScale = { 12.1f, 12.7f, 1 };
	//�ǐ����ŏI�T�C�Y���Z�b�g
	createEndScale = { 11.8f, 12.4f, 1 };

	//�������̐F�ɃZ�b�g
	wallObject->SetColor({ 0.3f, 0.3f, 0.3f, 1.0f });

	//�ǐ����^�C�}�[��������
	createTimer = 0;

	//�ǐ�����Ԃɂ���
	isCreate = true;
}

bool Wall::GetTriggerBreak()
{
	//�j�󂳂�Ă�����
	if (isBreak) 
	{
		//�g���K�[�Ȃ̂�false�ɖ߂�
		isBreak = false;

		return true;
	}

	return false;
}

void Wall::SetChangeResult()
{
	//���U���g�V�[���p�ɓ������^�C�}�[��������
	changeResultTimer = 0;

	//���U���g�V�[���p�ɓ�������ԂɃZ�b�g
	isChangeResult = true;
}

void Wall::BreakTime()
{
	//�x�e����
	const int breakTime = 80;

	//�^�C�}�[���X�V
	breakTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (breakTimer >= breakTime)
	{
		//�ǐ�����Ԃɂ���
		SetCreateWall();

		//�x�e�I��
		isBreakTime = false;
	}
}

void Wall::CreateWall()
{
	//�ǐ�������
	const int createTime = 100;

	//�ǐ����^�C�}�[���X�V
	createTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)createTimer / createTime;

	//�C�[�W���O�ŕǃI�u�W�F�N�g�̑傫���ύX
	XMFLOAT3 scale = wallObject->GetScale();
	scale.x = Easing::OutQuint(createStartScale.x, createEndScale.x, easeTimer);
	scale.y = Easing::OutQuint(createStartScale.y, createEndScale.y, easeTimer);
	//�ǃI�u�W�F�N�g�̑傫�����X�V
	wallObject->SetScale(scale);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (createTimer >= createTime)
	{
		//HP�ő厞�̐F�ɃZ�b�g
		wallObject->SetColor({ 0.8f, 0.3f, 0.1f, 1.0f });

		//�����Ă���
		isAlive = true;

		//�ǐ����I��
		isCreate = false;
	}
}

void Wall::ChangeColor()
{
	//�c��HP���ő�HP��20%�ȉ��̏ꍇ
	if (HP <= maxHP / 5) { wallObject->SetColor({ 0.3f, 0.1f, 0.1f, 1.0f }); }
	//�c��HP���ő�HP��50%�ȉ��̏ꍇ
	else if (HP <= maxHP / 2) { wallObject->SetColor({ 0.5f, 0.2f, 0.2f, 1.0f }); }
}

void Wall::ChangeResult()
{
	//�ǂ𓮂�������
	const int changeTime = 100;

	//�^�C�}�[���X�V
	changeResultTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)changeResultTimer / changeTime;

	//�C�[�W���O�ŕǃI�u�W�F�N�g�̑傫���ύX
	XMFLOAT3 scale = wallObject->GetScale();
	scale.x = Easing::OutQuint(createEndScale.x, 12.8f, easeTimer);
	scale.y = Easing::OutQuint(createEndScale.y, 13.4f, easeTimer);
	//�ǃI�u�W�F�N�g�̑傫�����X�V
	wallObject->SetScale(scale);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (changeResultTimer >= changeTime)
	{
		//�ǂ𓮂����I��
		isChangeResult = false;
	}
}
