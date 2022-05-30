#include "Rush.h"
#include "SafeDelete.h"
#include "Easing.h"

Rush* Rush::Create(int rushTexNum)
{
	//�C���X�^���X�𐶐�
	Rush* instance = new Rush();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(rushTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Rush::~Rush()
{
	safe_delete(rushSprite);
}

bool Rush::Initialize(int rushTexNum)
{
	//�Q�[�������X�v���C�g����
	rushSprite = Sprite::Create(rushTexNum);
	if (rushSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	rushSprite->SetTexSize({ 574, 183 });
	rushSprite->SetSize({ 287, 91.5f });
	rushSprite->SetPosition({ 1500, 190 });
	//�X�v���C�g�X�V
	rushSprite->Update();

	return true;
}

void Rush::Update()
{
	//��ʓ��Ɉړ����
	if (isMoveInScreen)
	{
		MoveInScreen();
	}
	//��ʊO�Ɉړ����
	else if (isMoveOutScreen)
	{
		MoveOutScreen();
	}

	//�F�ύX
	if (isChangeColor)
	{
		ChangeColor();
	}

	//�X�v���C�g�X�V
	rushSprite->Update();
}

void Rush::Draw()
{
	//�X�v���C�g�`��
	rushSprite->Draw();
}

void Rush::Reset()
{
	//��ʓ��Ɉړ�����
	isMoveInScreen = false;
	//��ʓ��Ɉړ����鎞�ԃ^�C�}�[
	moveInScreenTimer = 0;
	//��ʊO�Ɉړ�����
	isMoveOutScreen = false;
	//��ʊO�Ɉړ����鎞�ԃ^�C�}�[
	moveOutScreenTimer = 0;
	//��ʓ��ɂ��邩
	isInScreen = false;
	//�F��ς��邩
	isChangeColor = false;
	//�F��ς��鎞��
	changeColorTimer = 0;
	//�������邩
	isAlphaDown = true;

	//�X�v���C�g������
	rushSprite->SetPosition({ 1500, 190 });
	rushSprite->SetColor({ 1,1,1,1 });
	rushSprite->Update();
}

void Rush::SetMoveInScreen()
{
	//�^�C�}�[��������
	moveInScreenTimer = 0;
	//��ʓ��Ɉړ���
	isMoveInScreen = true;
	//��ʓ��ɂ���
	isInScreen = true;

	//�F��ς��鎞�ԃ^�C�}�[������
	changeColorTimer = 0;
	//��������
	isAlphaDown = true;
	//�X�v���C�g�̐F��������
	rushSprite->SetColor({ 1,1,1,1 });
	rushSprite->Update();
}

void Rush::SetMoveOutScreen()
{
	//�^�C�}�[��������
	moveOutScreenTimer = 0;
	//��ʊO�Ɉړ���
	isMoveOutScreen = true;
	//��ʓ��ɂ��Ȃ�
	isInScreen = false;
	//�F�ύX�I��
	isChangeColor = false;
}

void Rush::MoveInScreen()
{
	//�ړ����s������
	const int moveTime = 30;

	//�^�C�}�[���X�V
	moveInScreenTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveInScreenTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 pos = rushSprite->GetPosition();
	pos.x = Easing::OutQuint(1500, 1000, easeTimer);
	//�X�V�������W���Z�b�g
	rushSprite->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveInScreenTimer >= moveTime)
	{
		//��ʓ��Ɉړ���ԏI��
		isMoveInScreen = false;

		//�F�ύX�J�n
		isChangeColor = true;
	}
}

void Rush::MoveOutScreen()
{
	//�ړ����s������
	const int moveTime = 30;

	//�^�C�}�[���X�V
	moveOutScreenTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveOutScreenTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 pos = rushSprite->GetPosition();
	pos.x = Easing::InQuint(1000, 1500, easeTimer);
	//�X�V�������W���Z�b�g
	rushSprite->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveOutScreenTimer >= moveTime)
	{
		//��ʓ��Ɉړ���ԏI��
		isMoveOutScreen = false;
	}
}

void Rush::ChangeColor()
{
	//�F��ς��鎞��
	const int changeTime = 100;

	//�^�C�}�[���X�V
	changeColorTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)changeColorTimer / changeTime;

	//�X�v���C�g�̐F��ύX
	XMFLOAT4 color = rushSprite->GetColor();

	//��������ꍇ
	if (isAlphaDown)
	{
		color.w = Easing::OutQuint(1, 0.2f, easeTimer);
	}
	//�Z������ꍇ
	else
	{
		color.w = Easing::OutQuint(0.2f, 1, easeTimer);
	}
	//�X�V�����F���Z�b�g
	rushSprite->SetColor(color);

	//�^�C�}�[���w�肵�����Ԃɓ��B������
	if (changeColorTimer >= changeTime)
	{
		//�Z������̂Ɣ�������̂𔽓]
		if (isAlphaDown)
		{
			isAlphaDown = false;
		}
		else
		{
			isAlphaDown = true;
		}

		//�^�C�}�[��������
		changeColorTimer = 0;
	}
}
