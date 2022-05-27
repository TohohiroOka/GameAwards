#include "BackGround.h"
#include "SafeDelete.h"
#include "StageEffect.h"

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
	backGroundSprite->SetPosition({ 0, 0 });
	//�e�N�X�`���T�C�Y���Z�b�g
	backGroundSprite->SetTexSize({ 1280, 720 });
	//�傫�����Z�b�g
	backGroundSprite->SetSize({ 1280, 720 });

	return true;
}

void BackGround::Update()
{
	//�X�v���C�g�X�V
	backGroundSprite->Update();

	//�񕜃t�B�[���h�G�t�F�N�g�X�V
	StageEffect::SetHealField({ 1.0f,-11.0f,0.0f });
}

void BackGround::Draw()
{
	//�X�v���C�g�`��
	backGroundSprite->Draw();
}
