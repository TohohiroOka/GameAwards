#include "PauseUI.h"
#include "SafeDelete.h"
#include "Input.h"
#include "XInputManager.h"

PauseUI* PauseUI::Create(int plainTexNum, int pauseTexNum, int selectBackTexNum)
{
	//�C���X�^���X�𐶐�
	PauseUI* instance = new PauseUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(plainTexNum, pauseTexNum, selectBackTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

PauseUI::~PauseUI()
{
	safe_delete(backSprite);
	safe_delete(pauseSprite);
	safe_delete(selectBackSprite);
}

bool PauseUI::Initialize(int plainTexNum, int pauseTexNum, int selectBackTexNum)
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
	pauseSprite->SetTexSize({ 700, 200 });
	//�傫�����Z�b�g
	pauseSprite->SetSize({ 350, 100 });
	//�X�v���C�g�X�V
	pauseSprite->Update();


	//�߂�V�[���I���X�v���C�g����
	selectBackSprite = Sprite::Create(selectBackTexNum);
	if (selectBackSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	selectBackSprite->SetPosition({ 640, 600 });
	//�e�N�X�`���T�C�Y���Z�b�g
	selectBackSprite->SetTexSize({ 700, 200 });
	//�傫�����Z�b�g
	selectBackSprite->SetSize({ 350, 100 });
	//�X�v���C�g�X�V
	selectBackSprite->Update();

	return true;
}

void PauseUI::Update()
{
	//�Q�[���ɖ߂邩�^�C�g���V�[���ɖ߂邩�I��
	SelectBack();

	//�X�v���C�g�X�V
	backSprite->Update();
	pauseSprite->Update();
	selectBackSprite->Update();
}

void PauseUI::Draw()
{
	//�X�v���C�g�`��
	backSprite->Draw();
	pauseSprite->Draw();
	selectBackSprite->Draw();
}

void PauseUI::Reset()
{
	//�Q�[���ɖ߂邩
	isBackGame = true;
	//�I��������
	isSelect = false;

	//�X�v���C�g������
	selectBackSprite->SetTexLeftTop({ 0, 0 });
	selectBackSprite->Update();
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

			float texsizeX = selectBackSprite->GetTexSize().x;
			selectBackSprite->SetTexLeftTop({ texsizeX, 0 });
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

			
			selectBackSprite->SetTexLeftTop({ 0, 0 });
		}
	}
}
