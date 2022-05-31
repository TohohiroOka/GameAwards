#include "WallManager.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "StageEffect.h"
#include "XInputManager.h"
#include "Audio.h"
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

	for (auto& i : object)
	{
		safe_delete(i);
	}
}

void WallManager::Update()
{
	Audio* audio = Audio::GetInstance();

	bool wallBreak = PercentageDestruction();

	//�ǂ��j�󂳂�A�ǂ𐶐����Ă��Ȃ��ꍇ
	if (!status.isCreate && !status.isAlive)
	{
		int count = 0;
		//���ŏ�ԂȂ�܂��������Ȃ�
		for (const auto& i : object)
		{
			count += (i->GetState() == WallObject::STATE::NONE);
		}
		if (wallBreak && count == (int)WALL_STEP::step1)
		{
			//�T�E���h�̃t���O�����Z�b�g
			isSound = false;

			//�ǐ�����Ԃɂ���
			status.isCreate = true;

			isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_PLAY;
		}
	}
	//�ǂ��j�󂳂�A�ǂ𐶐����Ă���ꍇ
	else if (status.isCreate)
	{
		//�T�E���h�̍Đ�
		if (!isSound)
		{
			//��x������Ȃ��悤�ɂ���
			isSound = true;
			audio->SoundPlayWava(sound[0], false);
		}

		CreateWall();
	}

	if (isOutScreen && isSetEffect == EFFECT_NUM::WAIT)
	{
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			(*itr)->SetState(WallObject::STATE::OUT_SCREEN);
		}
	}

	//�I�u�W�F�N�g�X�V
	for (auto& i : object)
	{
		i->Update();
	}
}

void WallManager::Draw()
{
	//�I�u�W�F�N�g�`��
	for (auto& i : object)
	{
		i->Draw();
	}
}

void WallManager::Reset(bool allReset)
{
	//�I�u�W�F�N�g��
	status.wallNum = WALL_STEP::step1;
	//�^�C�g�����̏�����
	if (allReset)
	{
		//�ǐ����t���O
		isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_START;
	}
	//�^�C�g������v���C�V�[���֐؂�ւ��鎞�̏�����
	else
	{
		//�ǐ����t���O
		isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_PLAY;
	}
	//�ǂ̍ő�HP
	status.maxHP = baseMaxHP;
	//�ǂ�HP
	status.hp = status.maxHP;
	//�ǐ�������
	status.isCreate = true;
	//�󂳂ꂽ��
	status.isBreak = false;
	//�����Ă��邩
	status.isAlive = false;
	//�ǐ������̃t���O
	isSound = false;
	//������
	createCount = 0;
	effectCount = 0;
	objectCount = 0;
	//�Q�[���I�����ɕǂ���ʊO�ɍs��
	isOutScreen = false;
	//�I�u�W�F�N�g�P�̂̏�����
	for (auto& i : object)
	{
		i->Reset();
	}
	WallObject::staticReset();
}

void WallManager::Damage(int damagePower)
{
	Audio* audio = Audio::GetInstance();

	//�����Ŏw�肵�������̕�HP�����炷
	status.hp -= damagePower;

	//HP��0�ȉ��ɂȂ�����j��
	if (status.hp <= 0)
	{
		//�T�E���h�̍Đ�
		audio->SoundPlayWava(sound[1], false);

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

void WallManager::SetBreakCount(const unsigned int count)
{
	breakCount = count;
	status.maxHP = baseMaxHP + (2 * breakCount);
	status.hp = status.maxHP;
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
	for (auto& i : object)
	{
		i = WallObject::Create(model[modelNum % 10]);
		i->SetScale({ 8.0f,8.0f,8.0f });
		modelNum++;
	}

	//������
	Reset(true);

	//�T�E���h�̓ǂݍ���
	Audio* audio = Audio::GetInstance();
	sound[0] = audio->SoundLoadWave("Resources/sound/wallSet.wav");//�ǐ�����
	sound[1] = audio->SoundLoadWave("Resources/sound/wallBreak.wav");//�ǔj��

	//�T�E���h�̍Đ�
	audio->SoundPlayWava(sound[0], false);

	return true;
}

void WallManager::SetUpEffect()
{
	effectCount++;

	//0�ȊO�Ȃ�I�u�W�F�N�g���Z�b�g���Ȃ�
	if (effectCount % 2 != 0) { return; }

	XMFLOAT2 maxPosition = WallObject::GetWallMaxPosition();
	XMFLOAT2 minPosition = WallObject::GetWallMinPosition();

	for (int i = 0; i < 8; i++)
	{
		//�㉺�ƍ��E�Ō���2 : 1�ɂ���
		if (createCount % 2 == 1) { continue; }


		WallObject::STATE state = WallObject::STATE::NONE;
		DirectX::XMFLOAT3 startPos = {};

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
		object[objectCount]->Reset();
		object[objectCount]->SetState(state);
		object[objectCount]->SetPosition(startPos);
		objectCount++;

		//�S�ăZ�b�g���I������Z�b�g�I��
		if (objectCount == (int)WALL_STEP::step1)
		{
			//�g�p�����l�̏�����
			objectCount = 0;
			effectCount = 0;
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
	WallObject::STATE objState = object[(int)(WALL_STEP::step1)-1]->GetState();
	if (objState >= WallObject::STATE::MOVE_UP_LEFT && objState <= WallObject::STATE::MOVE_LEFT_UP)
	{
		WallObject::SetSlow(true);
	}

	//�S�ăZ�b�g���I������
	if (objState >= WallObject::STATE::DOWN && objState <= WallObject::STATE::RIGHT)
	{
		//�ő�HP��ݒ�
		breakCount++;
		SetBreakCount(breakCount);

		//����
		status.wallNum = WALL_STEP::step1;

		//�����Ă���
		status.isAlive = true;

		//�ǐ����I��
		status.isCreate = false;

		//���X�g�̃V���b�t��
		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());

		int size = (int)object.size();
		for (int i = 0; i < size; i++) {
			int r = engine() % (size - i) + i;
			WallObject* tmp = object[i];
			object[i] = object[r];
			object[r] = tmp;
		}
	}
}

bool WallManager::PercentageDestruction()
{
	if (effectCount == (int)WALL_STEP::step1) { return true; }

	//1��ڔj��
	if (status.hp <= status.maxHP / 1.2f && status.wallNum == WALL_STEP::step1)
	{
		int controlNum = (int)WALL_STEP::step1 - (int)WALL_STEP::step2;
		object[effectCount]->SetState(WallObject::STATE::FALL);
		effectCount++;

		//���ɍs��
		if (effectCount == controlNum) {
			status.wallNum = WALL_STEP::step2;
			//�U��
			XInputManager* Xinput = XInputManager::GetInstance();
			XInputManager::STRENGTH strength = XInputManager::STRENGTH::SMALL;
			Xinput->StartVibration(XInputManager::STRENGTH::LARGE, 20);
			Xinput = nullptr;
		}
	}
	//2��ڔj��
	else if (status.hp <= status.maxHP / 2 && status.wallNum == WALL_STEP::step2)
	{
		int controlNum = (int)WALL_STEP::step1 - (int)WALL_STEP::step3;
		object[effectCount]->SetState(WallObject::STATE::FALL);
		effectCount++;

		//���ɍs��
		if (effectCount == controlNum) {
			status.wallNum = WALL_STEP::step3;
			//�U��
			XInputManager* Xinput = XInputManager::GetInstance();
			XInputManager::STRENGTH strength = XInputManager::STRENGTH::SMALL;
			Xinput->StartVibration(XInputManager::STRENGTH::LARGE, 20);
			Xinput = nullptr;
		}
	}
	//3��ڔj��
	else if (status.hp <= 0 && status.wallNum == WALL_STEP::step3)
	{
		int controlNum = (int)WALL_STEP::step1;
		object[effectCount]->SetState(WallObject::STATE::FALL);
		effectCount++;

		//���ɍs��
		if (effectCount == controlNum) {
			status.wallNum = WALL_STEP::step4;
			isSetEffect = EFFECT_NUM::FALL_WAIT;

			//�U��
			XInputManager* Xinput = XInputManager::GetInstance();
			XInputManager::STRENGTH strength = XInputManager::STRENGTH::SMALL;
			Xinput->StartVibration(XInputManager::STRENGTH::LARGE, 20);
			Xinput = nullptr;

			return true;
		}
	}

	return false;
}