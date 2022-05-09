#include "BackGround.h"
#include "SafeDelete.h"

BackGround* BackGround::Create(int backGroundTexNum)
{
	//�C���X�^���X�𐶐�
	BackGround* instance = new BackGround();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(backGroundTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

BackGround::~BackGround()
{
	safe_delete(backGroundSprite);
}

bool BackGround::Initialize(int backGroundTexNum)
{
	//�w�i�X�v���C�g����
	backGroundSprite = Sprite::Create(backGroundTexNum, { 0, 0 });
	if (backGroundSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	backGroundSprite->SetPosition({ 0, 120 });
	//�e�N�X�`���T�C�Y���Z�b�g
	backGroundSprite->SetTexSize({ 1280, 720 });
	//�傫�����Z�b�g
	backGroundSprite->SetSize({ 1280, 600 });

	return true;
}

void BackGround::Update()
{
	//�X�v���C�g�X�V
	backGroundSprite->Update();
}

void BackGround::Draw()
{
	//�X�v���C�g�`��
	backGroundSprite->Draw();
}
