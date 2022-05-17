#include "PauseUI.h"
#include "SafeDelete.h"
#include "Input.h"
#include "XInputManager.h"

PauseUI* PauseUI::Create(int plainTexNum, int pauseTexNum, int backGameTexNum, int backTitleTexNum)
{
	//�C���X�^���X�𐶐�
	PauseUI* instance = new PauseUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(plainTexNum, pauseTexNum, backGameTexNum, backTitleTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

PauseUI::~PauseUI()
{
	safe_delete(backSprite);
	safe_delete(pauseSprite);
	safe_delete(backGameSprite);
	safe_delete(backTitleSprite);
}

bool PauseUI::Initialize(int plainTexNum, int pauseTexNum, int backGameTexNum, int backTitleTexNum)
{
	//�w�i�X�v���C�g����
	backSprite = Sprite::Create(plainTexNum, { 0, 0 });
	if (backSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	backSprite->SetPosition({ 0, 0 });
	//�e�N�X�`���T�C�Y���Z�b�g
	backSprite->SetTexSize({ 1, 1 });
	//�傫�����Z�b�g
	backSprite->SetSize({ 1280, 720 });
	//�F���Z�b�g
	backSprite->SetColor({ 1, 1, 1, 0.4f });
	//�X�v���C�g�X�V
	backSprite->Update();


	//�|�[�Y�X�v���C�g����
	pauseSprite = Sprite::Create(pauseTexNum);
	if (pauseSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	pauseSprite->SetPosition({ 640, 200 });
	//�e�N�X�`���T�C�Y���Z�b�g
	pauseSprite->SetTexSize({ 233, 63 });
	//�傫�����Z�b�g
	pauseSprite->SetSize({ 233, 63 });
	//�X�v���C�g�X�V
	pauseSprite->Update();


	//�Q�[���ɖ߂�V�[���I���X�v���C�g����
	backGameSprite = Sprite::Create(backGameTexNum);
	if (backGameSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	backGameSprite->SetPosition({ 400, 600 });
	//�e�N�X�`���T�C�Y���Z�b�g
	backGameSprite->SetTexSize({ 194, 63 });
	//�傫�����Z�b�g
	backGameSprite->SetSize({ 194, 63 });
	//�X�v���C�g�X�V
	backGameSprite->Update();

	//�^�C�g���ɖ߂�V�[���I���X�v���C�g����
	backTitleSprite = Sprite::Create(backTitleTexNum);
	if (backTitleSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	backTitleSprite->SetPosition({ 880, 600 });
	//�e�N�X�`���T�C�Y���Z�b�g
	backTitleSprite->SetTexSize({ 207, 63 });
	//�傫�����Z�b�g
	backTitleSprite->SetSize({ 207, 63 });
	//�X�v���C�g�X�V
	backTitleSprite->Update();

	return true;
}

void PauseUI::Update()
{
	//�Q�[���ɖ߂邩�^�C�g���V�[���ɖ߂邩�I��
	SelectBack();

	//�X�v���C�g�X�V
	backSprite->Update();
	pauseSprite->Update();
	backGameSprite->Update();
	backTitleSprite->Update();
}

void PauseUI::Draw()
{
	//�X�v���C�g�`��
	backSprite->Draw();
	pauseSprite->Draw();
	backGameSprite->Draw();
	backTitleSprite->Draw();
}

void PauseUI::Reset()
{
	//�Q�[���ɖ߂邩
	isBackGame = true;
	//�I��������
	isSelect = false;

	//�X�v���C�g������
	backGameSprite->SetColor({ 1, 0, 0, 1 });
	backGameSprite->Update();
	backTitleSprite->SetColor({ 1, 1, 1, 1 });
	backTitleSprite->Update();
}

void PauseUI::SelectBack()
{
	//�m�肵�Ă����甲����
	if (isSelect) { return; }

	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//�Q�[���ɖ߂��I�����Ă���Ƃ�
	if (isBackGame)
	{
		//�E�����ɃX�e�B�b�N��|����
		if (input->TriggerKey(DIK_RIGHT) || Xinput->TriggerLeftStickX(false))
		{
			//�^�C�g���V�[���ɖ߂��ԂɕύX
			isBackGame = false;

			backGameSprite->SetColor({ 1, 1, 1, 1 });
			backTitleSprite->SetColor({ 1, 0, 0, 1 });
		}
	}
	//�^�C�g���V�[���ɖ߂��I�����Ă���Ƃ�
	else
	{
		//�������ɃX�e�B�b�N��|����
		if (input->TriggerKey(DIK_LEFT) || Xinput->TriggerLeftStickX(true))
		{
			//�Q�[���ɖ߂��ԂɕύX
			isBackGame = true;

			backGameSprite->SetColor({ 1, 0, 0, 1 });
			backTitleSprite->SetColor({ 1, 1, 1, 1 });
		}
	}
}
