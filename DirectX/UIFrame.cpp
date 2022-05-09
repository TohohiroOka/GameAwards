#include "UIFrame.h"
#include "SafeDelete.h"

UIFrame* UIFrame::Create(int UIFrameTexNum)
{
	//�C���X�^���X�𐶐�
	UIFrame* instance = new UIFrame();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(UIFrameTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

UIFrame::~UIFrame()
{
	safe_delete(frameSprite);
}

bool UIFrame::Initialize(int UIFrameTexNum)
{
	//�g�X�v���C�g����
	frameSprite = Sprite::Create(UIFrameTexNum, { 0, 0 });
	if (frameSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	frameSprite->SetPosition({ 0, 0 });
	//�e�N�X�`���T�C�Y���Z�b�g
	frameSprite->SetTexSize({ 1280, 100 });
	//�傫�����Z�b�g
	frameSprite->SetSize({ 1280, 120 });

	return true;
}

void UIFrame::Update()
{
	//�X�v���C�g�X�V
	frameSprite->Update();
}

void UIFrame::Draw()
{
	//�X�v���C�g�`��
	frameSprite->Draw();
}
