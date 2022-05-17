#include "ResultUI.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "Input.h"
#include "XInputManager.h"

ResultUI* ResultUI::Create(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int retryTexNum, int backTitleTexNum)
{
	//�C���X�^���X�𐶐�
	ResultUI* instance = new ResultUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(plainTexNum, resultTexNum, scoreTexNum, numberTexNum, retryTexNum, backTitleTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ResultUI::~ResultUI()
{
	safe_delete(blackoutSprite);
	safe_delete(resultSprite);
	safe_delete(breakSprite);
	for (int i = 0; i < breakDigits; i++)
	{
		safe_delete(breakNumSprite[i]);
	}
	safe_delete(retrySprite);
	safe_delete(backTitleSprite);
}

bool ResultUI::Initialize(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int retryTexNum, int backTitleTexNum)
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


	//���U���g�X�v���C�g����
	resultSprite = Sprite::Create(resultTexNum);
	if (resultSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	resultSprite->SetPosition({ -200, 125 });
	//�e�N�X�`���T�C�Y���Z�b�g
	resultSprite->SetTexSize({ 268, 64 });
	//�傫�����Z�b�g
	resultSprite->SetSize({ 268, 64 });
	//�X�v���C�g�X�V
	resultSprite->Update();


	//BREAK�X�v���C�g����
	breakSprite = Sprite::Create(scoreTexNum, { 1, 0.5f });
	if (breakSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	breakSprite->SetPosition({ -100, 300 });
	//�e�N�X�`���T�C�Y���Z�b�g
	breakSprite->SetTexSize({ 234, 64 });
	//�傫�����Z�b�g
	breakSprite->SetSize({ 234, 64 });
	//�X�v���C�g�X�V
	breakSprite->Update();

	//�󂵂����X�v���C�g����
	//���̌�������
	for (int i = 0; i < breakDigits; i++)
	{
		//�󂵂����X�v���C�g����
		breakNumSprite[i] = Sprite::Create(numberTexNum);
		if (breakNumSprite[i] == nullptr) {
			return false;
		}
		//�傫�����Z�b�g
		XMFLOAT2 size = { 32, 64 };
		breakNumSprite[i]->SetSize(size);
		//�e�N�X�`���T�C�Y���Z�b�g
		XMFLOAT2 texSize = { 48, 64 };
		breakNumSprite[i]->SetTexSize(texSize);
		//���W���Z�b�g
		XMFLOAT2 pos = { 1400, 300 };
		pos.x -= size.x * i;
		breakNumSprite[i]->SetPosition(pos);
		//�X�v���C�g�X�V
		breakNumSprite[i]->Update();
	}


	//���g���C�X�v���C�g����
	retrySprite = Sprite::Create(retryTexNum, { 1, 0.5f });
	if (retrySprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	retrySprite->SetPosition({ -100, 550 });
	//�e�N�X�`���T�C�Y���Z�b�g
	retrySprite->SetTexSize({ 225, 65 });
	//�傫�����Z�b�g
	retrySprite->SetSize({ 225, 65 });
	//�X�v���C�g�X�V
	retrySprite->Update();

	//�^�C�g���ɖ߂�X�v���C�g����
	backTitleSprite = Sprite::Create(backTitleTexNum, { 0, 0.5f });
	if (backTitleSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	backTitleSprite->SetPosition({ 1400, 550 });
	//�e�N�X�`���T�C�Y���Z�b�g
	backTitleSprite->SetTexSize({ 207, 63 });
	//�傫�����Z�b�g
	backTitleSprite->SetSize({ 207, 63 });
	//�X�v���C�g�X�V
	backTitleSprite->Update();

	//�Ó]��ԂɃZ�b�g���Ă���
	SetBlackOut();

	return true;
}

void ResultUI::Update()
{
	//�Ó]
	if (isBlackout)
	{
		BlackOut();
	}
	//���U���g�X�v���C�g�𓮂���
	if (isMoveResultSprite)
	{
		MoveResultSprite();
	}
	//�ǔj�󐔃X�v���C�g�𓮂���
	if (isMoveBreakSprite)
	{
		MoveBreakSprite();
	}
	//���g���C�X�v���C�g�𓮂���
	if (isMoveRetrySprite)
	{
		MoveRetrySprite();
	}
	//�S�ĕ`�悵�I������
	if (isDrawAll)
	{
		//���g���C���邩�^�C�g���V�[���ɖ߂邩�I��
		SelectRetry();
	}

	//�X�v���C�g�X�V
	blackoutSprite->Update();
	resultSprite->Update();
	breakSprite->Update();
	for (int i = 0; i < breakDigits; i++)
	{
		breakNumSprite[i]->Update();
	}
	retrySprite->Update();
	backTitleSprite->Update();
}

void ResultUI::Draw()
{
	//�X�v���C�g�`��
	blackoutSprite->Draw();
	resultSprite->Draw();
	breakSprite->Draw();
	for (int i = 0; i < breakDigits; i++)
	{
		breakNumSprite[i]->Draw();
	}
	retrySprite->Draw();
	backTitleSprite->Draw();
}

void ResultUI::Reset()
{
	//�ǔj�󖇐�
	breakWallNum = 0;

	//�Ó]����
	isBlackout = false;
	//�Ó]���鎞�ԃ^�C�}�[
	blackoutTimer = 0;
	//���U���g�X�v���C�g�𓮂�����
	isMoveResultSprite = false;
	//���U���g�X�v���C�g�𓮂������ԃ^�C�}�[
	moveResultSpriteTimer = 0;
	//�ǔj�󐔃X�v���C�g�𓮂�����
	isMoveBreakSprite = false;
	//�ǔj�󐔃X�v���C�g�𓮂������ԃ^�C�}�[
	moveBreakSpriteTimer = 0;
	//���g���C�X�v���C�g�𓮂�����
	isMoveRetrySprite = false;
	//���g���C�X�v���C�g�𓮂������ԃ^�C�}�[
	moveRetrySpriteTimer = 0;

	//�S�ĕ`�悵����
	isDrawAll = false;

	//���g���C��Ԃ�
	isRetry = true;
	//�m�肵����
	isSelect = false;

	//���W���Z�b�g
	blackoutSprite->SetPosition({ 0, 0 });
	//�F���Z�b�g
	blackoutSprite->SetColor({ 0, 0, 0, 0 });
	//�X�v���C�g�X�V
	blackoutSprite->Update();

	//���W���Z�b�g
	resultSprite->SetPosition({ -200, 125 });
	//�X�v���C�g�X�V
	resultSprite->Update();

	//���W���Z�b�g
	breakSprite->SetPosition({ -100, 300 });
	//�X�v���C�g�X�V
	breakSprite->Update();

	for (int i = 0; i < breakDigits; i++)
	{
		//�傫�����Z�b�g
		XMFLOAT2 size = breakNumSprite[i]->GetSize();;

		//���W���Z�b�g
		XMFLOAT2 pos = { 1400, 300 };
		pos.x -= size.x * i;
		breakNumSprite[i]->SetPosition(pos);
		//�X�v���C�g�X�V
		breakNumSprite[i]->Update();
	}

	//���W���Z�b�g
	retrySprite->SetPosition({ -100, 550 });
	retrySprite->SetColor({ 1, 1, 1, 1 });
	//�X�v���C�g�X�V
	retrySprite->Update();

	//���W���Z�b�g
	backTitleSprite->SetPosition({ 1400, 550 });
	backTitleSprite->SetColor({ 1, 1, 1, 1 });
	//�X�v���C�g�X�V
	backTitleSprite->Update();

	//�w�i�Ó]��ԂɃZ�b�g���Ă���
	SetBlackOut();
}

void ResultUI::SetBreakWallNum(int breakWallNum)
{
	this->breakWallNum = breakWallNum;

	//�X�v���C�g�̐������X�V
	ChangeBreakNumSprite();
}

void ResultUI::SetBlackOut()
{
	//�^�C�}�[������������
	blackoutTimer = 0;

	//�Ó]��ԂɃZ�b�g
	isBlackout = true;
}

void ResultUI::SetMoveResultSprite()
{
	//�^�C�}�[������������
	moveBreakSpriteTimer = 0;

	//���U���g�X�v���C�g�𓮂�����ԂɃZ�b�g
	isMoveResultSprite = true;
}

void ResultUI::SetMoveBreakSprite()
{
	//�^�C�}�[������������
	moveBreakSpriteTimer = 0;

	//�ǔj�󐔃X�v���C�g�𓮂�����ԂɃZ�b�g
	isMoveBreakSprite = true;
}

void ResultUI::SetMoveRetrySprite()
{
	//�^�C�}�[������������
	moveRetrySpriteTimer = 0;

	//���g���C�X�v���C�g�𓮂�����ԂɃZ�b�g
	isMoveRetrySprite = true;
}

void ResultUI::ChangeBreakNumSprite()
{
	//���������ꂼ��o�͂���
	int digit[breakDigits];
	digit[0] = breakWallNum % 10;			//0001
	digit[1] = (breakWallNum / 10) % 10;	//0010
	digit[2] = (breakWallNum / 100) % 10;	//0100
	digit[3] = (breakWallNum / 1000) % 10;	//1000

	//���ꂼ��̌��̐������X�v���C�g�̃e�N�X�`���؂�o�������炷
	for (int i = 0; i < breakDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = breakNumSprite[i]->GetTexSize().x * digit[i];
		breakNumSprite[i]->SetTexLeftTop(leftTop);
	}
}

void ResultUI::BlackOut()
{
	//�Ó]���s������
	const int blackoutTime = 200;

	//�^�C�}�[�X�V
	blackoutTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)blackoutTimer / blackoutTime;

	//�C�[�W���O�Řg�̈Ó]
	float alpha = Easing::OutQuad(0, 0.6f, easeTimer);
	blackoutSprite->SetColor({ 0, 0, 0, alpha });

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (blackoutTimer >= blackoutTime)
	{
		//�Ó]�I��
		isBlackout = false;

		//���U���g�X�v���C�g�𓮂�����ԂɃZ�b�g
		SetMoveResultSprite();
	}
}

void ResultUI::MoveResultSprite()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveResultSpriteTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveResultSpriteTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 resultPos = resultSprite->GetPosition();
	resultPos.x = Easing::OutQuint(-50, 640, easeTimer);
	//�X�V�������W���Z�b�g
	resultSprite->SetPosition(resultPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveResultSpriteTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveResultSprite = false;

		//�ǔj�󐔃X�v���C�g�𓮂�����ԂɃZ�b�g
		SetMoveBreakSprite();
	}
}

void ResultUI::MoveBreakSprite()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveBreakSpriteTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveBreakSpriteTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 breakPos = breakSprite->GetPosition();
	breakPos.x = Easing::OutQuint(-50, 600, easeTimer);
	//�X�V�������W���Z�b�g
	breakSprite->SetPosition(breakPos);

	for (int i = 0; i < breakDigits; i++)
	{
		XMFLOAT2 breakNumPos = breakNumSprite[i]->GetPosition();
		XMFLOAT2 breakNumSize = breakNumSprite[i]->GetSize();
		breakNumPos.x = Easing::OutQuint(1330 - i * breakNumSize.x, 800 - i * breakNumSize.x, easeTimer);
		//�X�V�������W���Z�b�g
		breakNumSprite[i]->SetPosition(breakNumPos);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveBreakSpriteTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveBreakSprite = false;

		//���g���C�X�v���C�g�𓮂�����ԂɃZ�b�g
		SetMoveRetrySprite();
	}
}

void ResultUI::MoveRetrySprite()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveRetrySpriteTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveRetrySpriteTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 retryPos = retrySprite->GetPosition();
	retryPos.x = Easing::OutQuint(-50, 600, easeTimer);
	//�X�V�������W���Z�b�g
	retrySprite->SetPosition(retryPos);

	XMFLOAT2 backTitlePos = backTitleSprite->GetPosition();
	backTitlePos.x = Easing::OutQuint(1400, 680, easeTimer);
	//�X�V�������W���Z�b�g
	backTitleSprite->SetPosition(backTitlePos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveRetrySpriteTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveRetrySprite = false;

		//�S�Ă̕`�抮��
		isDrawAll = true;

		//���g���C��I����Ԃɂ��邽�߃��g���C�X�v���C�g�̐F��ύX
		retrySprite->SetColor({ 1, 0, 0, 1 });
	}
}

void ResultUI::SelectRetry()
{
	//�m�肵�Ă����甲����
	if (isSelect) { return; }

	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//���g���C��I�����Ă���Ƃ�
	if (isRetry)
	{
		//�E�����ɃX�e�B�b�N��|����
		if (input->TriggerKey(DIK_RIGHT) || Xinput->TriggerLeftStickX(false))
		{
			//�^�C�g���V�[���ɖ߂��ԂɕύX
			isRetry = false;

			retrySprite->SetColor({ 1, 1, 1, 1 });
			backTitleSprite->SetColor({ 1, 0, 0, 1 });
		}
	}
	//�^�C�g���V�[���ɖ߂��I�����Ă���Ƃ�
	else
	{
		//�������ɃX�e�B�b�N��|����
		if (input->TriggerKey(DIK_LEFT) || Xinput->TriggerLeftStickX(true))
		{
			//���g���C�����ԂɕύX
			isRetry = true;

			retrySprite->SetColor({ 1, 0, 0, 1 });
			backTitleSprite->SetColor({ 1, 1, 1, 1 });
		}
	}
}
