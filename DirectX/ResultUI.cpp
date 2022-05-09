#include "ResultUI.h"
#include "SafeDelete.h"
#include "Easing.h"

ResultUI* ResultUI::Create(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int maxComboTexNum, int retryTexNum, int pressATexNum)
{
	//�C���X�^���X�𐶐�
	ResultUI* instance = new ResultUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(plainTexNum, resultTexNum, scoreTexNum, numberTexNum, maxComboTexNum, retryTexNum, pressATexNum)) {
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
	safe_delete(maxComboSprite);
	for (int i = 0; i < maxComboDigits; i++)
	{
		safe_delete(maxComboNumSprite[i]);
	}
	safe_delete(retrySprite);
	safe_delete(pressASprite);
}

bool ResultUI::Initialize(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int maxComboTexNum, int retryTexNum, int pressATexNum)
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


	//MAXCOMBO�X�v���C�g����
	maxComboSprite = Sprite::Create(maxComboTexNum, { 1, 0.5f });
	if (maxComboSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	maxComboSprite->SetPosition({ -100, 400 });
	//�e�N�X�`���T�C�Y���Z�b�g
	maxComboSprite->SetTexSize({ 391, 63 });
	//�傫�����Z�b�g
	maxComboSprite->SetSize({ 391, 63 });
	//�X�v���C�g�X�V
	maxComboSprite->Update();

	//�ő�R���{���X�v���C�g����
	//���̌�������
	for (int i = 0; i < maxComboDigits; i++)
	{
		//�ő�R���{���X�v���C�g����
		maxComboNumSprite[i] = Sprite::Create(numberTexNum);
		if (maxComboNumSprite[i] == nullptr) {
			return false;
		}
		//�傫�����Z�b�g
		XMFLOAT2 size = { 32, 64 };
		maxComboNumSprite[i]->SetSize(size);
		//�e�N�X�`���T�C�Y���Z�b�g
		XMFLOAT2 texSize = { 48, 64 };
		maxComboNumSprite[i]->SetTexSize(texSize);
		//���W���Z�b�g
		XMFLOAT2 pos = { 1400, 400 };
		pos.x -= size.x * i;
		maxComboNumSprite[i]->SetPosition(pos);
		//�X�v���C�g�X�V
		maxComboNumSprite[i]->Update();
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

	//PRESS A�X�v���C�g����
	pressASprite = Sprite::Create(pressATexNum, { 0, 0.5f });
	if (pressASprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	pressASprite->SetPosition({ 1400, 550 });
	//�e�N�X�`���T�C�Y���Z�b�g
	pressASprite->SetTexSize({ 318, 83 });
	//�傫�����Z�b�g
	pressASprite->SetSize({ 318, 83 });
	//�X�v���C�g�X�V
	pressASprite->Update();

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
	//�ő�R���{���X�v���C�g�𓮂���
	if (isMoveMaxComboSprite)
	{
		MoveMaxComboSprite();
	}
	//���g���C�X�v���C�g�𓮂���
	if (isMoveRetrySprite)
	{
		MoveRetrySprite();
	}

	//�X�v���C�g�X�V
	blackoutSprite->Update();
	resultSprite->Update();
	breakSprite->Update();
	for (int i = 0; i < breakDigits; i++)
	{
		breakNumSprite[i]->Update();
	}
	maxComboSprite->Update();
	for (int i = 0; i < maxComboDigits; i++)
	{
		maxComboNumSprite[i]->Update();
	}
	retrySprite->Update();
	pressASprite->Update();
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
	maxComboSprite->Draw();
	for (int i = 0; i < maxComboDigits; i++)
	{
		maxComboNumSprite[i]->Draw();
	}
	retrySprite->Draw();
	pressASprite->Draw();
}

void ResultUI::Reset()
{
	//�ǔj�󖇐�
	breakWallNum = 0;
	//�ő�R���{�X�R�A
	maxCombo = 0;

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
	//�ő�R���{���X�v���C�g�𓮂�����
	isMoveMaxComboSprite = false;
	//�ő�R���{���X�v���C�g�𓮂������ԃ^�C�}�[
	moveMaxComboSpriteTimer = 0;
	//���g���C�X�v���C�g�𓮂�����
	isMoveRetrySprite = false;
	//���g���C�X�v���C�g�𓮂������ԃ^�C�}�[
	moveRetrySpriteTimer = 0;

	//�S�ĕ`�悵����
	isDrawAll = false;



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
	maxComboSprite->SetPosition({ -100, 400 });
	//�X�v���C�g�X�V
	maxComboSprite->Update();

	for (int i = 0; i < maxComboDigits; i++)
	{
		XMFLOAT2 size = maxComboNumSprite[i]->GetSize();
		//���W���Z�b�g
		XMFLOAT2 pos = { 1400, 400 };
		pos.x -= size.x * i;
		maxComboNumSprite[i]->SetPosition(pos);
		//�X�v���C�g�X�V
		maxComboNumSprite[i]->Update();
	}

	//���W���Z�b�g
	retrySprite->SetPosition({ -100, 550 });
	//�X�v���C�g�X�V
	retrySprite->Update();

	//���W���Z�b�g
	pressASprite->SetPosition({ 1400, 550 });
	//�X�v���C�g�X�V
	pressASprite->Update();


	//�Ó]��ԂɃZ�b�g���Ă���
	SetBlackOut();
}

void ResultUI::SetBreakWallNum(int breakWallNum)
{
	this->breakWallNum = breakWallNum;

	//�X�v���C�g�̐������X�V
	ChangeBreakNumSprite();
}

void ResultUI::SetMaxCombo(int maxCombo)
{
	this->maxCombo = maxCombo;

	//�X�v���C�g�̐������X�V
	ChangeMaxComboSprite();
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

void ResultUI::SetMoveMaxComboSprite()
{
	//�^�C�}�[������������
	moveMaxComboSpriteTimer = 0;

	//�ő�R���{���X�v���C�g�𓮂�����ԂɃZ�b�g
	isMoveMaxComboSprite = true;
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
	for (int i = 0; i < maxComboDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = breakNumSprite[i]->GetTexSize().x * digit[i];
		breakNumSprite[i]->SetTexLeftTop(leftTop);
	}
}

void ResultUI::ChangeMaxComboSprite()
{
	//���������ꂼ��o�͂���
	int digit[maxComboDigits];
	digit[0] = maxCombo % 10;			//0001
	digit[1] = (maxCombo / 10) % 10;	//0010
	digit[2] = (maxCombo / 100) % 10;	//0100
	digit[3] = (maxCombo / 1000) % 10;	//1000

	//���ꂼ��̌��̐������X�v���C�g�̃e�N�X�`���؂�o�������炷
	for (int i = 0; i < maxComboDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = maxComboNumSprite[i]->GetTexSize().x * digit[i];
		maxComboNumSprite[i]->SetTexLeftTop(leftTop);
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

	//�C�[�W���O�Řg�̃��C���ύX
	float alpha = Easing::OutQuad(0, 0.6f, easeTimer);

	//�g�I�u�W�F�N�g�̑傫�����X�V
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

		//�ő�R���{���X�v���C�g�𓮂�����ԂɃZ�b�g
		SetMoveMaxComboSprite();
	}
}

void ResultUI::MoveMaxComboSprite()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveMaxComboSpriteTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveMaxComboSpriteTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 maxComboPos = maxComboSprite->GetPosition();
	maxComboPos.x = Easing::OutQuint(-50, 600, easeTimer);
	//�X�V�������W���Z�b�g
	maxComboSprite->SetPosition(maxComboPos);

	for (int i = 0; i < maxComboDigits; i++)
	{
		XMFLOAT2 maxComboPos = maxComboNumSprite[i]->GetPosition();
		XMFLOAT2 maxComboSize = maxComboNumSprite[i]->GetSize();
		maxComboPos.x = Easing::OutQuint(1330 - i * maxComboSize.x, 800 - i * maxComboSize.x, easeTimer);
		//�X�V�������W���Z�b�g
		maxComboNumSprite[i]->SetPosition(maxComboPos);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveMaxComboSpriteTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveMaxComboSprite = false;

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

	XMFLOAT2 pressAPos = pressASprite->GetPosition();
	pressAPos.x = Easing::OutQuint(1330, 680, easeTimer);
	//�X�V�������W���Z�b�g
	pressASprite->SetPosition(pressAPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveRetrySpriteTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveRetrySprite = false;

		//�S�Ă̕`�抮��
		isDrawAll = true;
	}
}
