#include "Explanation.h"
#include "SafeDelete.h"
#include "Easing.h"

Explanation* Explanation::Create(int explanationTexNum)
{
	//�C���X�^���X�𐶐�
	Explanation* instance = new Explanation();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(explanationTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Explanation::~Explanation()
{
	safe_delete(explanationSprite);
}

bool Explanation::Initialize(int explanationTexNum)
{
	//�Q�[�������X�v���C�g����
	explanationSprite = Sprite::Create(explanationTexNum, { 0, 0.5f });
	if (explanationSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	explanationSprite->SetPosition({ 1280, 250 });
	explanationSprite->SetSize({ 373, 38 });
	explanationSprite->SetTexSize({ 746, 75 });
	//�X�v���C�g�X�V
	explanationSprite->Update();

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
}

void Explanation::Draw()
{
	//�X�v���C�g�`��
	explanationSprite->Draw();
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
	explanationSprite->SetPosition({ 1280, 250 });
	explanationSprite->Update();
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
	XMFLOAT2 pos = explanationSprite->GetPosition();
	pos.x = Easing::OutQuint(1280, 820, easeTimer);
	//�X�V�������W���Z�b�g
	explanationSprite->SetPosition(pos);

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
	XMFLOAT2 pos = explanationSprite->GetPosition();
	pos.x = Easing::InQuint(820, 1280, easeTimer);
	//�X�V�������W���Z�b�g
	explanationSprite->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveOutScreenTimer >= moveTime)
	{
		//��ʓ��Ɉړ���ԏI��
		isMoveOutScreen = false;
	}
}
