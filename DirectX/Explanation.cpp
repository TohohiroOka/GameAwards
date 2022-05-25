#include "Explanation.h"
#include "SafeDelete.h"
#include "Easing.h"

Explanation* Explanation::Create(int explanationTexNum, int XBunttonTexNum)
{
	//�C���X�^���X�𐶐�
	Explanation* instance = new Explanation();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(explanationTexNum, XBunttonTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Explanation::~Explanation()
{
	safe_delete(explanationSprite);
	safe_delete(XButtonSprite);
}

bool Explanation::Initialize(int explanationTexNum, int XBunttonTexNum)
{
	//�Q�[�������X�v���C�g����
	explanationSprite = Sprite::Create(explanationTexNum);
	if (explanationSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	explanationSprite->SetTexSize({ 746, 75 });
	explanationSprite->SetSize({ 373, 38 });
	explanationSprite->SetPosition({ 1500, 250 });
	//�X�v���C�g�X�V
	explanationSprite->Update();


	//X�{�^���X�v���C�g����
	XButtonSprite = Sprite::Create(XBunttonTexNum);
	if (explanationSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	XButtonSprite->SetTexSize({ 300, 112 });
	XButtonSprite->SetSize({ 150, 66 });
	XButtonSprite->SetPosition({ 1500, 320 });
	//�X�v���C�g�X�V
	XButtonSprite->Update();

	return true;
}

void Explanation::Update()
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

	//�X�v���C�g�X�V
	explanationSprite->Update();
	XButtonSprite->Update();
}

void Explanation::Draw()
{
	//�X�v���C�g�`��
	explanationSprite->Draw();
	XButtonSprite->Draw();
}

void Explanation::Reset()
{
	//��ʓ��Ɉړ�����
	isMoveInScreen = false;
	//��ʓ��Ɉړ����鎞�ԃ^�C�}�[
	moveInScreenTimer = 0;
	//��ʊO�Ɉړ�����
	isMoveOutScreen = false;
	//��ʊO�Ɉړ����鎞�ԃ^�C�}�[
	moveOutScreenTimer = 0;

	//�X�v���C�g������
	explanationSprite->SetPosition({ 1500, 250 });
	explanationSprite->Update();
	XButtonSprite->SetPosition({ 1500, 320 });
	XButtonSprite->Update();
}

void Explanation::SetMoveInScreen()
{
	//�^�C�}�[��������
	moveInScreenTimer = 0;

	//��ʓ��Ɉړ���
	isMoveInScreen = true;
}

void Explanation::SetMoveOutScreen()
{
	//�^�C�}�[��������
	moveOutScreenTimer = 0;

	//��ʊO�Ɉړ���
	isMoveOutScreen = true;
}

void Explanation::MoveInScreen()
{
	//�ړ����s������
	const int moveTime = 30;

	//�^�C�}�[���X�V
	moveInScreenTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveInScreenTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 explanationPos = explanationSprite->GetPosition();
	explanationPos.x = Easing::OutQuint(1500, 1000, easeTimer);
	XMFLOAT2 XButtonPos = XButtonSprite->GetPosition();
	XButtonPos.x = Easing::OutQuint(1500, 1000, easeTimer);
	//�X�V�������W���Z�b�g
	explanationSprite->SetPosition(explanationPos);
	XButtonSprite->SetPosition(XButtonPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveInScreenTimer >= moveTime)
	{
		//��ʓ��Ɉړ���ԏI��
		isMoveInScreen = false;
	}
}

void Explanation::MoveOutScreen()
{
	//�ړ����s������
	const int moveTime = 30;

	//�^�C�}�[���X�V
	moveOutScreenTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveOutScreenTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 explanationPos = explanationSprite->GetPosition();
	explanationPos.x = Easing::InQuint(1000, 1500, easeTimer);
	XMFLOAT2 XButtonPos = XButtonSprite->GetPosition();
	XButtonPos.x = Easing::InQuint(1000, 1500, easeTimer);
	//�X�V�������W���Z�b�g
	explanationSprite->SetPosition(explanationPos);
	XButtonSprite->SetPosition(XButtonPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveOutScreenTimer >= moveTime)
	{
		//��ʓ��Ɉړ���ԏI��
		isMoveOutScreen = false;
	}
}
