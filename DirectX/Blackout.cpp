#include "Blackout.h"
#include "SafeDelete.h"
#include "Easing.h"

Blackout* Blackout::Create(int plainTexNum)
{
	//�C���X�^���X�𐶐�
	Blackout* instance = new Blackout();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(plainTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Blackout::~Blackout()
{
	safe_delete(blackoutSprite);
}

bool Blackout::Initialize(int plainTexNum)
{
	//�Ó]�p�X�v���C�g����
	blackoutSprite = Sprite::Create(plainTexNum, { 0, 0 });
	if (blackoutSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	blackoutSprite->SetPosition({ 0, 0 });
	//�e�N�X�`���T�C�Y���Z�b�g
	blackoutSprite->SetTexSize({ 1, 1 });
	//�傫�����Z�b�g
	blackoutSprite->SetSize({ 1280, 720 });
	//�F���Z�b�g
	blackoutSprite->SetColor({ 0, 0, 0, 0 });
	//�X�v���C�g�X�V
	blackoutSprite->Update();

	return true;
}

void Blackout::Update()
{
	//�Ó]
	if (isBlackout)
	{
		BlackoutUpdate();
	}
	//�Ó]����
	if (isReturn)
	{
		BlackoutReturn();
	}

	//�X�v���C�g�X�V
	blackoutSprite->Update();
}

void Blackout::Draw()
{
	//�X�v���C�g�`��
	blackoutSprite->Draw();
}

void Blackout::Reset()
{
	//�^���Â�
	isAllBlack = false;
	//�Ó]����
	isBlackout = false;
	//�Ó]��߂���
	isReturn = false;
}

void Blackout::SetBlackout()
{
	//�^�C�}�[������������
	blackoutTimer = 0;

	//�Ó]��ԂɃZ�b�g
	isBlackout = true;
}

void Blackout::SetBlackoutReturn()
{
	//�^�C�}�[������������
	returnTimer = 0;

	//�Ó]������ԂɃZ�b�g
	isReturn = true;
}

void Blackout::BlackoutUpdate()
{
	//�Ó]���s������
	const int blackoutTime = 100;

	//�^�C�}�[�X�V
	blackoutTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)blackoutTimer / blackoutTime;

	//�C�[�W���O�ňÓ]
	float alpha = Easing::OutQuad(0, 1, easeTimer);
	blackoutSprite->SetColor({ 0, 0, 0, alpha });

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (blackoutTimer >= blackoutTime)
	{
		//�Ó]�I��
		isBlackout = false;

		//�^����
		isAllBlack = true;
	}
}

void Blackout::BlackoutReturn()
{
	//�Ó]�������s������
	const int returnTime = 100;

	//�^�C�}�[�X�V
	returnTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)returnTimer / returnTime;

	//�C�[�W���O�ňÓ]
	float alpha = Easing::OutQuad(1, 0, easeTimer);
	blackoutSprite->SetColor({ 0, 0, 0, alpha });

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (returnTimer >= returnTime)
	{
		//�Ó]�����I��
		Reset();
	}
}
