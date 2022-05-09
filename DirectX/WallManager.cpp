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
	else if (status.isCreate)
	{
		CreateWall();
	}

	//�I�u�W�F�N�g�X�V
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr)->Update();
	}
}

void WallManager::Draw()
{
	if (isBreakTime) { return; }

	int i = 0;

	//�I�u�W�F�N�g�`��
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr)->Draw();
		i++;
	}
}

void WallManager::Reset()
{
	//�I�u�W�F�N�g��
	status.wallNum = WALL_STEP::step1;
	//�ǂ̍ő�HP
	status.maxHP = baseMaxHP;
	//�ǂ�HP
	status.hp = status.maxHP;
	//�x�e����
	isBreakTime = true;
	//�ǐ�����
	createCount = 0;
	//�ǐ�������
	status.isCreate = false;
	//���U���g�V�[���p�ɓ����������ԃ^�C�}�[
	changeResultTimer = 0;
	//�ǂ����U���g�V�[���p�ɓ������Ă��邩
	isChangeResult = false;
	//�󂳂ꂽ��
	isBreak = false;
	//�����Ă��邩
	status.isAlive = false;
	//�I�u�W�F�N�g�P�̂̏�����
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr)->Reset();
	}
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
		//�j�󂷂�
		isBreak = true;

		//�x�e��Ԃɂ���
		isBreakTime = true;

		//�����Ă��Ȃ�
		status.isAlive = false;
	}
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
	endItr = object.end();
	endItr--;

	//�ŏ���HP��ݒ�
	status.maxHP = baseMaxHP;
	status.hp = status.maxHP;

	return true;
}

void WallManager::SetUpEffect()
{
	//��ʒu�ւ̈ړ��Z�b�g
	if (!(isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START) &&
		!(isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_PLAY)) {
		return;
	}

	float time = 50.0f;
	if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START)
	{
		time = 150;
	}
	WallObject::SetEffectTime(time);

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
		(*nowItr)->Reset();
		(*nowItr)->SetState(state);
		(*nowItr)->SetLerpEndPosition(lerpEnd);

		//�C�e���[�^�ƃJ�E���g��i�߂�
		nowItr++;

		//�S�ăZ�b�g���I������Z�b�g�I��
		if (nowItr == object.end())
		{
			//�g�p�����l�̏�����
			nowItr = object.begin();
			effectTime = 0;
			isSetEffect = EFFECT_NUM::WAIT;
			status.isAlive = true;

			continue;
		}
	}
}

void WallManager::BreakTime()
{
	//���ŏ�ԂȂ�܂��������Ȃ�
	if ((*endItr)->SetState() == WallObject::STATE::TRANSPARENCY) { return; }

	//�ő�HP��ݒ�
	createCount++;
	status.maxHP = baseMaxHP + (10 * createCount);
	status.hp = status.maxHP;

	//�ǐ�����Ԃɂ���
	status.isCreate = true;

	//�v���C�O���
	if (isSetEffect == EFFECT_NUM::NONE)
	{
		isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_START;
	} else {
		isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_PLAY;
	}

	//�x�e�I��
	isBreakTime = false;
}

void WallManager::CreateWall()
{
	SetUpEffect();

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if ((*endItr)->GetTime() > WallObject::GetEffectTime())
	{
		//����
		status.wallNum = WALL_STEP::step1;

		//�����Ă���
		status.isAlive = true;

		//�ǐ����I��
		status.isCreate = false;
	}
}

void WallManager::ChangeColor()
{
	//�c��HP���ő�HP��20%�ȉ��̏ꍇ
	if (status.hp <= status.maxHP / 5)
	{
		status.wallNum = WALL_STEP::step2;
		int num = 0;
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			if (num > (int)WALL_STEP::step2)
			{
				(*itr)->SetState(WallObject::STATE::TRANSPARENCY);
			}
			num++;
		}
	}
	//�c��HP���ő�HP��50%�ȉ��̏ꍇ
	if (status.hp <= status.maxHP / 2)
	{
		status.wallNum = WALL_STEP::step3;
		int num = 0;
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			if (num > (int)WALL_STEP::step3 && num < (int)WALL_STEP::step2)
			{
				(*itr)->SetState(WallObject::STATE::TRANSPARENCY);
			}
			num++;
		}
	}
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
