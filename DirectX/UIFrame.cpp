#include "UIFrame.h"
#include "SafeDelete.h"
#include "Easing.h"

UIFrame* UIFrame::Create(int UIFrameTexNum, int startTexNum)
{
	//�C���X�^���X�𐶐�
	UIFrame* instance = new UIFrame();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(UIFrameTexNum, startTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

UIFrame::~UIFrame()
{
	safe_delete(frameSprite);
	safe_delete(startSprite);
}

bool UIFrame::Initialize(int UIFrameTexNum, int startTexNum)
{
	//�g�X�v���C�g����
	frameSprite = Sprite::Create(UIFrameTexNum);
	if (frameSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	frameSprite->SetPosition({ 640, -100 });
	//�e�N�X�`���T�C�Y���Z�b�g
	frameSprite->SetTexSize({ 1280, 39 });
	//�傫�����Z�b�g
	frameSprite->SetSize({ 1280, 39 });
	//�X�v���C�g�X�V
	frameSprite->Update();


	//�X�^�[�g�{�^���X�v���C�g����
	startSprite = Sprite::Create(startTexNum);
	if (startSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	startSprite->SetPosition({ 1180, -100 });
	//�e�N�X�`���T�C�Y���Z�b�g
	startSprite->SetTexSize({ 166, 32 });
	//�傫�����Z�b�g
	startSprite->SetSize({ 132.8f, 25.6f });
	//�X�v���C�g�X�V
	startSprite->Update();

	return true;
}

void UIFrame::Update()
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

	//�X�v���C�g�X�V
	frameSprite->Update();
	startSprite->Update();
}

void UIFrame::Draw()
{
	//�X�v���C�g�`��
	frameSprite->Draw();

	if (!isDrawStart) { return; }
	startSprite->Draw();
}

void UIFrame::Reset()
{
	//�X�^�[�g�{�^����`�悷�邩
	isDrawStart = false;
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

	//�X�v���C�g������
	frameSprite->SetPosition({ 640, -100 });
	frameSprite->Update();
	startSprite->SetPosition({ 1180, -100 });
	startSprite->Update();
}

void UIFrame::SetMoveGamePos()
{
	//�Q�[���V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveGamePosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveGamePos = true;
}

void UIFrame::SetMoveResultPos()
{
	//���U���g�V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveResultPosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveResultPos = true;
}

void UIFrame::MoveGamePos()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveGamePosTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveGamePosTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 startPos = startSprite->GetPosition();
	framePos.y = Easing::OutQuint(-100, 20, easeTimer);
	startPos.y = Easing::OutQuint(-100, 20, easeTimer);
	//�X�V�������W���Z�b�g
	frameSprite->SetPosition(framePos);
	startSprite->SetPosition(startPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveGamePosTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveGamePos = false;

		//�ړ�����
		isMoveGamePosEnd = true;
	}
}

void UIFrame::MoveResultPos()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveResultPosTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveResultPosTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 startPos = startSprite->GetPosition();
	framePos.y = Easing::OutQuint(20, -100, easeTimer);
	startPos.y = Easing::OutQuint(20, -100, easeTimer);
	//�X�V�������W���Z�b�g
	frameSprite->SetPosition(framePos);
	startSprite->SetPosition(startPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveResultPosTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveResultPos = false;

		//�ړ�����
		isMoveResultPosEnd = true;
	}
}
