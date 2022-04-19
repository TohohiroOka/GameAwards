#include "TimeLimit.h"
#include "SafeDelete.h"

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
		XMFLOAT2 size = { 32, 64 };
		timeSprite[i]->SetSize(size);

		//�e�N�X�`���T�C�Y���Z�b�g
		XMFLOAT2 texSize = { 32, 64 };
		timeSprite[i]->SetTexSize(texSize);

		//���W���Z�b�g
		XMFLOAT2 pos = { 656, 55 };
		pos.x -= size.x * i;
		timeSprite[i]->SetPosition(pos);
	}

	return true;
}

void TimeLimit::Update()
{
	//���t���[���������Ԃ����炵�Ă���
	CountDown();
	ChangeTimeSprite();

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
