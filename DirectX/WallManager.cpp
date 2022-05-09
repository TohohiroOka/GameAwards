#include "WallManager.h"
#include "SafeDelete.h"
#include "Easing.h"
#include <random>

WallManager* WallManager::Create()
{
	//�C���X�^���X�𐶐�
	WallManager* instance = new WallManager();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	return instance;
}

WallManager::~WallManager()
{
	for (int i = 0; i < 10; i++)
	{
		safe_delete(model[i]);
	}

	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		safe_delete((*itr));
	}
	object.clear();
}

void WallManager::Update()
{
	if (isSetEffect != 0)
	{
		SetUpEffect();
	}

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
	int num = 0;
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		if ((int)status.wallNum < num) { continue; }
		(*itr)->Update();
		num++;
	}
}

void WallManager::Draw()
{
	if (isBreakTime) { return; }

	//�I�u�W�F�N�g�`��
	int num = 0;
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		if ((int)status.wallNum < num) { continue; }
		(*itr)->Draw();
		num++;
	}
}

void WallManager::Reset()
{
	//�I�u�W�F�N�g��
	status.wallNum = WALL_STEP::step1;
	//�ǂ̍ő�HP
	maxHP = 20;
	//�ǂ�HP
	status.hp = maxHP;
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
	status.isAlive = false;
}

void WallManager::Damage(int damagePower)
{
	//�����Ŏw�肵�������̕�HP�����炷
	status.hp -= damagePower;

	//�c��HP�ɉ����ĐF�ύX
	ChangeColor();

	//HP��0�ȉ��ɂȂ�����j��
	if (status.hp <= 0)
	{
		Break();

		//�����Ă��Ȃ�
		status.isAlive = false;
	}
}

void WallManager::Break()
{
	//�j�󂷂�
	isBreak = true;

	//�x�e��Ԃɂ���
	SetBreakTime();
}

void WallManager::SetBreakTime()
{
	//�x�e���ԃ^�C�}�[��������
	breakTimer = 0;

	//�x�e��Ԃɂ���
	isBreakTime = true;
}

void WallManager::SetCreateWall()
{
	//�ő�HP��ݒ�
	createCount++;
	maxHP = baseMaxHP + (10 * createCount);
	status.hp = maxHP;

	//�ǐ��������T�C�Y���Z�b�g
	createStartScale = { 12.1f, 12.7f, 1 };
	//�ǐ����ŏI�T�C�Y���Z�b�g
	createEndScale = { 11.8f, 12.4f, 1 };

	//�ǐ����^�C�}�[��������
	createTimer = 0;

	//�ǐ�����Ԃɂ���
	isCreate = true;
}

bool WallManager::GetTriggerBreak()
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

void WallManager::SetChangeResult()
{
	//���U���g�V�[���p�ɓ������^�C�}�[��������
	changeResultTimer = 0;

	//���U���g�V�[���p�ɓ�������ԂɃZ�b�g
	isChangeResult = true;
}

void WallManager::LoadModel()
{
	model[0] = Model::CreateFromOBJ("garakuta1");
	model[1] = Model::CreateFromOBJ("garakuta2");
	model[2] = Model::CreateFromOBJ("garakuta3");
	model[3] = Model::CreateFromOBJ("garakuta4");
	model[4] = Model::CreateFromOBJ("garakuta5");
	model[5] = Model::CreateFromOBJ("garakuta6");
	model[6] = Model::CreateFromOBJ("garakuta7");
	model[7] = Model::CreateFromOBJ("garakuta8");
	model[8] = Model::CreateFromOBJ("garakuta9");
	model[9] = Model::CreateFromOBJ("garakuta10");
}

bool WallManager::Initialize()
{
	//���f���ǂݍ���
	LoadModel();

	//��������
	std::random_device rand;
	std::mt19937 mt(rand());

	//���f���̔ԍ�
	int modelNum = 0;

	//�ǃI�u�W�F�N�g�̐���
	object.resize((int)status.wallNum);
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr) = WallObject::Create(model[modelNum % 10]);
		(*itr)->SetPosition({ 0.0f,0.0f,-500.0f });
		(*itr)->SetScale({ 5.0f,5.0f,5.0f });
		modelNum++;
	}

	//���o�̂��ߎn�߂̃C�e���[�^��ۑ����Ă���
	nowItr = object.begin();

	//�ŏ���HP��ݒ�
	maxHP = baseMaxHP;
	status.hp = maxHP;

	return true;
}

void WallManager::SetUpEffect()
{
	//��ʒu�ւ̈ړ��Z�b�g
	if (isSetEffect == 1)
	{
		effectTime++;

		//0�ȊO�Ȃ�I�u�W�F�N�g���Z�b�g���Ȃ�
		if (effectTime % 8 != 0) { return; }

		XMFLOAT2 maxPosition = WallObject::GetWallMaxPosition();
		XMFLOAT2 minPosition = WallObject::GetWallMinPosition();

		for (int i = 0; i < 4; i++)
		{
			WallObject::STATE state;
			DirectX::XMFLOAT3 lerpEnd = {};
			if (i == 0)
			{
				state = WallObject::STATE::DIRECTING_LEFT_UP;
				lerpEnd = { minPosition.x,maxPosition.y,0 };
			} else if (i == 1)
			{
				state = WallObject::STATE::DIRECTING_RIGHT_UP;
				lerpEnd = { maxPosition.x,maxPosition.y,0 };
			} else if (i == 2)
			{
				state = WallObject::STATE::DIRECTING_RIGHT_DOWN;
				lerpEnd = { maxPosition.x,minPosition.y,0 };
			} else if (i == 3)
			{
				state = WallObject::STATE::DIRECTING_LEFT_DOWN;
				lerpEnd = { minPosition.x,minPosition.y,0 };
			}

			//���̃Z�b�g
			(*nowItr)->SetState(state);
			(*nowItr)->SetLerpEndPosition(lerpEnd);

			//�C�e���[�^�ƃJ�E���g��i�߂�
			nowItr++;

			//�S�ăZ�b�g���I������Z�b�g�I��
			if (nowItr == object.end())
			{
				isSetEffect = 2;
				nowItr--;
				effectTime = 0;
				continue;
			}
		}
	}

	//����΂�
	else if (isSetEffect == 2)
	{
		//���X�g�̍Ō�̃I�u�W�F�N�g���ړ����I�����玟�ɍs��
		if ((*nowItr)->GetTime() < 150) { return; }

		//��������
		std::random_device rnd;
		std::mt19937 mt(rnd());

		//�ω��ʂ̃Z�b�g
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			(*itr)->SetState(WallObject::STATE::DISPERSE);
			(*itr)->SetDisperseMovePos({ (float)(mt() % 10) - 5,(float)(mt() % 10) - 5 ,(float)(mt() % 10) - 5 });
			(*itr)->SetDisperseMoveRota({ (float)(mt() % 360),(float)(mt() % 10) ,(float)(mt() % 10) });
		}

		//���o�t���O�ƃC�e���[�^�̏�����
		isSetEffect = 0;
		nowItr = object.begin();
		status.isAlive = true;
	}
}

void WallManager::BreakTime()
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

void WallManager::CreateWall()
{
	////�ǐ�������
	//const int createTime = 100;

	////�ǐ����^�C�}�[���X�V
	//createTimer++;

	////�C�[�W���O�v�Z�p�̎���
	//float easeTimer = (float)createTimer / createTime;

	////�C�[�W���O�ŕǃI�u�W�F�N�g�̑傫���ύX
	//XMFLOAT3 scale = wallObject->GetScale();
	//scale.x = Easing::OutQuint(createStartScale.x, createEndScale.x, easeTimer);
	//scale.y = Easing::OutQuint(createStartScale.y, createEndScale.y, easeTimer);
	////�ǃI�u�W�F�N�g�̑傫�����X�V
	//wallObject->SetScale(scale);

	////�^�C�}�[���w�肵�����ԂɂȂ�����
	//if (createTimer >= createTime)
	//{
	//	//HP�ő厞�̐F�ɃZ�b�g
	//	wallObject->SetColor({ 0.8f, 0.3f, 0.1f, 1.0f });

	//	//�����Ă���
	//	status.isAlive = true;

	//	//�ǐ����I��
	//	isCreate = false;
	//}
}

void WallManager::ChangeColor()
{
	////�c��HP���ő�HP��20%�ȉ��̏ꍇ
	//if (HP <= maxHP / 5) { wallObject->SetColor({ 0.3f, 0.1f, 0.1f, 1.0f }); }
	////�c��HP���ő�HP��50%�ȉ��̏ꍇ
	//else if (HP <= maxHP / 2) { wallObject->SetColor({ 0.5f, 0.2f, 0.2f, 1.0f }); }
}

void WallManager::ChangeResult()
{
	////�ǂ𓮂�������
	//const int changeTime = 100;

	////�^�C�}�[���X�V
	//changeResultTimer++;

	////�C�[�W���O�v�Z�p�̎���
	//float easeTimer = (float)changeResultTimer / changeTime;

	////�C�[�W���O�ŕǃI�u�W�F�N�g�̑傫���ύX
	//XMFLOAT3 scale = wallObject->GetScale();
	//scale.x = Easing::OutQuint(createEndScale.x, 12.8f, easeTimer);
	//scale.y = Easing::OutQuint(createEndScale.y, 13.4f, easeTimer);
	////�ǃI�u�W�F�N�g�̑傫�����X�V
	//wallObject->SetScale(scale);

	////�^�C�}�[���w�肵�����ԂɂȂ�����
	//if (changeResultTimer >= changeTime)
	//{
	//	//�ǂ𓮂����I��
	//	isChangeResult = false;
	//}
}
