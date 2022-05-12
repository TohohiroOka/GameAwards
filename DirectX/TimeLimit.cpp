#include "TimeLimit.h"
#include "SafeDelete.h"
#include "Easing.h"

TimeLimit* TimeLimit::Create(int textureNum)
{
	//�C���X�^���X�𐶐�
	TimeLimit* instance = new TimeLimit();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(textureNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

TimeLimit::~TimeLimit()
{
	for (int i = 0; i < timeDigits; i++)
	{
		safe_delete(timeSprite[i]);
	}
}

bool TimeLimit::Initialize(int textureNum)
{
	//��������
	for (int i = 0; i < timeDigits; i++)
	{
		//�������ԗp�X�v���C�g����
		timeSprite[i] = Sprite::Create(textureNum);
		if (timeSprite[i] == nullptr) {
			return false;
		}

		//�傫�����Z�b�g
		XMFLOAT2 size = { 32, 48 };
		timeSprite[i]->SetSize(size);

		//�e�N�X�`���T�C�Y���Z�b�g
		XMFLOAT2 texSize = { 48, 64 };
		timeSprite[i]->SetTexSize(texSize);

		//���W���Z�b�g
		XMFLOAT2 pos = { 640 + size.x / 2, -100 };
		pos.x -= size.x * i;
		timeSprite[i]->SetPosition(pos);
	}

	//�X�v���C�g�X�V
	ChangeTimeSprite();
	for (int i = 0; i < timeDigits; i++)
	{
		timeSprite[i]->Update();
	}

	return true;
}

void TimeLimit::Update()
{
	//�Q�[���V�[���̍��W�Ɉړ�
	if (isMoveGamePos)
	{
		MoveGamePos();
	}
	//���U���g�V�[���̍��W�Ɉړ�
	else if (isMoveResultPos)
	{
		MoveResultPos();
	}

	if (isCountDown)
	{
		//���t���[���������Ԃ����炵�Ă���
		CountDown();
		ChangeTimeSprite();
	}

	//�X�v���C�g�X�V
	for (int i = 0; i < timeDigits; i++)
	{
		timeSprite[i]->Update();
	}
}

void TimeLimit::Draw()
{
	//�X�v���C�g�`��
	for (int i = 0; i < timeDigits; i++)
	{
		timeSprite[i]->Draw();
	}
}

void TimeLimit::Reset()
{
	//���Ԍv���^�C�}�[
	timer = timeLimitMax;
	//�J�E���g�_�E�����邩
	isCountDown = false;
	//�Q�[���V�[���̍��W�Ɉړ�����
	isMoveGamePos = false;
	//�Q�[���V�[���̍��W�Ɉړ��I��������
	isMoveGamePosEnd = false;
	//�Q�[���V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[
	moveGamePosTimer = 0;
	//���U���g�V�[���̍��W�Ɉړ�����
	isMoveResultPos = false;
	//���U���g�V�[���̍��W�Ɉړ��I��������
	isMoveResultPosEnd = false;
	//���U���g�V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[
	moveResultPosTimer = 0;
	//�\���p�X�v���C�g��0�ɖ߂�
	ChangeTimeSprite();
	//�X�v���C�g�X�V
	for (int i = 0; i < timeDigits; i++)
	{
		timeSprite[i]->Update();
	}
}

void TimeLimit::Recovery(int second)
{
	//�����̕b���A�������Ԃ��񕜂���
	timer += second * 60;

	//�ő�ȏ���񕜂��Ȃ�
	if (timer >= timeLimitMax)
	{
		timer = timeLimitMax;
	}
}

void TimeLimit::SetMoveGamePos()
{
	//�Q�[���V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveGamePosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveGamePos = true;
}

void TimeLimit::SetMoveResultPos()
{
	//���U���g�V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveResultPosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveResultPos = true;
}

void TimeLimit::CountDown()
{
	//���t���[���^�C�}�[���J�E���g�_�E��
	timer--;

	//0�ȉ��ɂ͂Ȃ�Ȃ��悤�ɂ��Ă���
	if (timer <= 0)
	{
		timer = 0;
	}
}

void TimeLimit::ChangeTimeSprite()
{
	//���̐��������ꂼ��o�͂���
	int digit[timeDigits];
	digit[0] = timer / 60 % 10;			//01
	digit[1] = timer / (60 * 10) % 10;	//10

	//���ꂼ��̌��̐������X�v���C�g�̃e�N�X�`���؂�o�������炷
	for (int i = 0; i < timeDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = timeSprite[i]->GetTexSize().x * digit[i];
		timeSprite[i]->SetTexLeftTop(leftTop);
	}
}

void TimeLimit::MoveGamePos()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveGamePosTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveGamePosTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	for (int i = 0; i < timeDigits; i++)
	{
		XMFLOAT2 timePos = timeSprite[i]->GetPosition();
		timePos.y = Easing::OutQuint(-83, 72, easeTimer);
		timeSprite[i]->SetPosition(timePos);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveGamePosTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveGamePos = false;

		//�ړ�����
		isMoveGamePosEnd = true;
	}
}

void TimeLimit::MoveResultPos()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveResultPosTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveResultPosTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	for (int i = 0; i < timeDigits; i++)
	{
		XMFLOAT2 timePos = timeSprite[i]->GetPosition();
		timePos.y = Easing::OutQuint(72, -83, easeTimer);
		timeSprite[i]->SetPosition(timePos);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveResultPosTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveResultPos = false;

		//�ړ�����
		isMoveResultPosEnd = true;
	}
}
