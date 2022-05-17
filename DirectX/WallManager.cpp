#include "WallManager.h"
#include "SafeDelete.h"
#include "Easing.h"
#include <random>
#include <cstdlib>
#include "StageEffect.h"
#include "XInputManager.h"

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
	//�ǂ��j�󂳂�A�ǂ𐶐����Ă��Ȃ��ꍇ
	if (!status.isCreate && !status.isAlive)
	{
		//���ŏ�ԂȂ�܂��������Ȃ�
		if (!((*endItr)->GetState() == WallObject::STATE::TRANSPARENCY))
		{
			//�ǐ�����Ԃɂ���
			status.isCreate = true;

			isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_PLAY;
		}
	}
	//�ǂ��j�󂳂�A�ǂ𐶐����Ă���ꍇ
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
	//�I�u�W�F�N�g�`��
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr)->Draw();
	}
}

void WallManager::Reset(bool allReset)
{
	//�I�u�W�F�N�g��
	status.wallNum = WALL_STEP::step1;
	//�ǐ����t���O
	isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_START;
	if (!allReset)
	{
		isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_PLAY;
	}
	//�ǂ̍ő�HP
	status.maxHP = baseMaxHP;
	//�ǂ�HP
	status.hp = status.maxHP;
	//�ǐ�����
	breakCount = 0;
	//�ǐ�������
	status.isCreate = true;
	//���U���g�V�[���p�ɓ����������ԃ^�C�}�[
	changeResultTimer = 0;
	//�ǂ����U���g�V�[���p�ɓ������Ă��邩
	isChangeResult = false;
	//�󂳂ꂽ��
	status.isBreak = false;
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
	PercentageDestruction();

	//HP��0�ȉ��ɂȂ�����j��
	if (status.hp <= 0)
	{
		//�j�󂷂�
		status.isBreak = true;

		//�����Ă��Ȃ�
		status.isAlive = false;
	}
}

bool WallManager::GetTriggerBreak()
{
	//�j�󂳂�Ă�����
	if (status.isBreak)
	{
		//�g���K�[�Ȃ̂�false�ɖ߂�
		status.isBreak = false;

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

void WallManager::SetHitEffect(XMFLOAT3 enemyPos)
{
	XMFLOAT2 maxPosition = WallObject::GetWallMaxPosition();
	XMFLOAT2 minPosition = WallObject::GetWallMinPosition();

	XMFLOAT2 range = { maxPosition.x + enemyPos.x,maxPosition.y + enemyPos.y };
	if (range.x > minPosition.x + enemyPos.x)
	{
		range.x = minPosition.x + enemyPos.x;
	}
	if (range.y > minPosition.y + enemyPos.y)
	{
		range.y = minPosition.y + enemyPos.y;
	}

	//���W�A��
	float radius = atan2f(range.y - enemyPos.y, range.x - enemyPos.x);
	//�p�x
	float angle = DirectX::XMConvertToDegrees(radius);


	StageEffect::SetHitWall(enemyPos, angle);
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

	//�ǃI�u�W�F�N�g�̐���
	const XMFLOAT2 min = WallObject::GetWallMinPosition();
	const XMFLOAT2 max = WallObject::GetWallMaxPosition();

	//���f���̔ԍ�
	int modelNum = 0;

	//�I�u�W�F�N�g�z��̃T�C�Y�ύX
	object.resize((int)status.wallNum);
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr) = WallObject::Create(model[modelNum % 10]);
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

	//����̕ǐ���
	status.isCreate = true;
	isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_START;

	return true;
}

void WallManager::SetUpEffect()
{
	effectTime++;

	//0�ȊO�Ȃ�I�u�W�F�N�g���Z�b�g���Ȃ�
	if (effectTime % 4 != 0) { return; }

	XMFLOAT2 maxPosition = WallObject::GetWallMaxPosition();
	XMFLOAT2 minPosition = WallObject::GetWallMinPosition();

	for (int i = 0; i < 8; i++)
	{
		//�㉺��68
		//32
		WallObject::STATE state = WallObject::STATE::NONE;
		DirectX::XMFLOAT3 startPos = {};

		//�㉺�ƍ��E�Ō���2 : 1�ɂ���
		if (createCount % 2 == 1) { continue; }

		//����
		if (i == 0 || i == 1)
		{
			startPos = { minPosition.x,maxPosition.y,0 };
			if (i % 2 == 0)
			{
				state = WallObject::STATE::MOVE_UP_RIGHT;
				if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START)
				{
					startPos.y += 8.0f;
				}
			} else
			{
				state = WallObject::STATE::MOVE_LEFT_DOWN;
			}
		}
		//����
		else if (i == 2 || i == 3)
		{
			startPos = { minPosition.x,minPosition.y,0 };
			if (i % 2 == 0)
			{
				state = WallObject::STATE::MOVE_DOWN_RIGHT;
				if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START)
				{
					startPos.y += 8.0f;
				}
			} else
			{
				state = WallObject::STATE::MOVE_LEFT_UP;
			}
		}
		//�E��
		else if (i == 4 || i == 5)
		{
			startPos = { maxPosition.x,maxPosition.y,0 };
			if (i % 2 == 0)
			{
				state = WallObject::STATE::MOVE_UP_LEFT;
				if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START)
				{
					startPos.y += 8.0f;
				}
			} else
			{
				state = WallObject::STATE::MOVE_RIGHT_DOWN;
			}
		}
		//�E��
		else if (i == 6 || i == 7)
		{
			startPos = { maxPosition.x,minPosition.y,0 };
			if (i % 2 == 0)
			{
				state = WallObject::STATE::MOVE_DOWN_LEFT;
				if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START)
				{
					startPos.y += 8.0f;
				}
			} else
			{
				state = WallObject::STATE::MOVE_RIGHT_UP;
			}
		}

		//���̃Z�b�g
		(*nowItr)->Reset();
		(*nowItr)->SetState(state);
		(*nowItr)->SetPosition(startPos);

		//�C�e���[�^�ƃJ�E���g��i�߂�
		nowItr++;

		//�S�ăZ�b�g���I������Z�b�g�I��
		if (nowItr == object.end())
		{
			//�g�p�����l�̏�����
			nowItr = object.begin();
			effectTime = 0;
			isSetEffect = EFFECT_NUM::WAIT;
			createCount = 0;
			return;
		}
	}

	createCount++;
}

void WallManager::CreateWall()
{
	//��ʒu�ւ̈ړ��Z�b�g
	if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START ||
		isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_PLAY)
	{
		SetUpEffect();
	}

	//�ŏ��̃I�u�W�F�N�g����ʒu�ɒ�������ǂɂȂ�
	WallObject::STATE objState = (*endItr)->GetState();
	if (objState >= WallObject::STATE::MOVE_UP_LEFT && objState <= WallObject::STATE::MOVE_LEFT_UP)
	{
		WallObject::SetSlow(true);
	}

	//�S�ăZ�b�g���I������
	if ((*endItr)->GetState() == WallObject::STATE::WAIT)
	{
		//�ő�HP��ݒ�
		breakCount++;
		status.maxHP = baseMaxHP + (10 * breakCount);
		status.hp = status.maxHP;

		//����
		status.wallNum = WALL_STEP::step1;

		//�����Ă���
		status.isAlive = true;

		//�ǐ����I��
		status.isCreate = false;
	}
}

void WallManager::PercentageDestruction()
{
	//�c��HP���ő�HP��20%�ȉ��̏ꍇ
	if (status.hp <= status.maxHP / 5)
	{
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			(*itr)->SetColor({ 0.2f,1.0f,1.0f,1.0f });
		}
	}
	//�c��HP���ő�HP��50%�ȉ��̏ꍇ
	if (status.hp <= status.maxHP / 2)
	{
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			(*itr)->SetColor({ 1.0f,0.1f,0.1f,1.0f });
		}
	}
	//�c��HP��0�̏ꍇ
	if (status.hp <= 0)
	{
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			(*itr)->SetState(WallObject::STATE::TRANSPARENCY);
		}

		//�U��
		XInputManager* Xinput = XInputManager::GetInstance();
		XInputManager::STRENGTH strength = XInputManager::STRENGTH::SMALL;
		Xinput->StartVibration(XInputManager::STRENGTH::LARGE, 20);
		Xinput = nullptr;

	}
}