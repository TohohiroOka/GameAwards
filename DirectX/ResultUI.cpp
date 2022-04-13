#include "ResultUI.h"
#include "SafeDelete.h"
#include "Easing.h"

ResultUI* ResultUI::Create(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int pressButtonTexNum)
{
	//�C���X�^���X�𐶐�
	ResultUI* instance = new ResultUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(plainTexNum, resultTexNum, scoreTexNum, numberTexNum, pressButtonTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ResultUI::~ResultUI()
{
	safe_delete(blackoutSprite);
	safe_delete(resultSprite);
	safe_delete(SCORESprite);

	for (int i = 0; i < scoreDigits; i++)
	{
		safe_delete(finalScoreSprite[i]);
	}
	safe_delete(pressButtonSprite);
}

bool ResultUI::Initialize(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int pressButtonTexNum)
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


	//���U���g�X�v���C�g����
	resultSprite = Sprite::Create(resultTexNum);
	if (resultSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	resultSprite->SetPosition({ 640, 150 });
	//�e�N�X�`���T�C�Y���Z�b�g
	resultSprite->SetTexSize({ 178, 38 });
	//�傫�����Z�b�g
	resultSprite->SetSize({ 356, 76 });


	//SCORE:�X�v���C�g����
	SCORESprite = Sprite::Create(scoreTexNum);
	if (SCORESprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	SCORESprite->SetPosition({ 500, 350 });
	//�e�N�X�`���T�C�Y���Z�b�g
	SCORESprite->SetTexSize({ 178, 38 });
	//�傫�����Z�b�g
	SCORESprite->SetSize({ 178, 38 });


	//�ŏI�X�R�A�X�v���C�g����
	//�X�R�A�̌�������
	for (int i = 0; i < scoreDigits; i++)
	{
		//�X�R�A�\���p�X�v���C�g����
		finalScoreSprite[i] = Sprite::Create(numberTexNum);
		if (finalScoreSprite[i] == nullptr) {
			return false;
		}

		//�傫�����Z�b�g
		XMFLOAT2 size = { 32, 64 };
		finalScoreSprite[i]->SetSize(size);

		//�e�N�X�`���T�C�Y���Z�b�g
		XMFLOAT2 texSize = { 32, 64 };
		finalScoreSprite[i]->SetTexSize(texSize);

		//���W���Z�b�g
		XMFLOAT2 pos = { 852, 350 };
		pos.x -= size.x * i;
		finalScoreSprite[i]->SetPosition(pos);
	}


	//pressButton�X�v���C�g����
	pressButtonSprite = Sprite::Create(pressButtonTexNum);
	if (pressButtonSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	pressButtonSprite->SetPosition({ 640, 500 });
	//�e�N�X�`���T�C�Y���Z�b�g
	pressButtonSprite->SetTexSize({ 284, 38 });
	//�傫�����Z�b�g
	pressButtonSprite->SetSize({ 284, 38 });


	//�X�v���C�g�X�V���Ă���
	blackoutSprite->Update();
	resultSprite->Update();
	SCORESprite->Update();
	for (int i = 0; i < scoreDigits; i++)
	{
		finalScoreSprite[i]->Update();
	}
	pressButtonSprite->Update();

	return true;
}

void ResultUI::Update()
{
	if (drawScene == DrawScene::BlackOut)
	{
		//�Ó]
		BlackOutUpdate();
	}
	else if (drawScene == DrawScene::ResultDraw)
	{
		//��莞�Ԍo�����玟�̕`����n�߂�
		TimeCount();
	}
	else if (drawScene == DrawScene::FinalScoreDraw)
	{
		//�\���p�X�R�A�̉��Z���o
		IncreaseDisplayScore();
		//�X�v���C�g��\���p�X�R�A�ɍ��킹��
		IncreaseScoreSprite();
	}
	else if (drawScene == DrawScene::Stay)
	{
		//��莞�Ԍo�����玟�̕`����n�߂�
		TimeCount();
	}
	else if (drawScene == DrawScene::PressButtonDraw)
	{
		isDrawAll = true;
	}

	//�X�v���C�g�X�V
	blackoutSprite->Update();
	if (drawScene >= DrawScene::ResultDraw)
	{
		resultSprite->Update();
	}
	if (drawScene >= DrawScene::FinalScoreDraw)
	{
		SCORESprite->Update();

		for (int i = 0; i < scoreDigits; i++)
		{
			finalScoreSprite[i]->Update();
		}
	}
	if (drawScene >= DrawScene::PressButtonDraw)
	{
		pressButtonSprite->Update();
	}
}

void ResultUI::Draw()
{
	//�X�v���C�g�`��
	blackoutSprite->Draw();
	if (drawScene >= DrawScene::ResultDraw)
	{
		resultSprite->Draw();
	}
	if (drawScene >= DrawScene::FinalScoreDraw)
	{
		SCORESprite->Draw();

		for (int i = 0; i < scoreDigits; i++)
		{
			finalScoreSprite[i]->Draw();
		}
	}
	if (drawScene >= DrawScene::PressButtonDraw)
	{
		pressButtonSprite->Draw();
	}
}

void ResultUI::Reset()
{
	//�Ó]����������
	blackoutSprite->SetColor({ 0, 0, 0, 0 });
	//�ŏI�X�R�A������
	finalScore = 0;
	//�\���p�X�R�A������
	displayScore = 0;
	//�\���p�X�v���C�g��0�ɖ߂�
	IncreaseScoreSprite();
	//���Ԍv���^�C�}�[������
	timer = 0;
	//�V�[����������
	drawScene = DrawScene::BlackOut;
	//�t���O��false��
	isDrawAll = false;
	//�X�v���C�g�X�V
	blackoutSprite->Update();
	resultSprite->Update();
	SCORESprite->Update();
	for (int i = 0; i < scoreDigits; i++)
	{
		finalScoreSprite[i]->Update();
	}
	pressButtonSprite->Update();
}

void ResultUI::SetFinalScore(int finalScore)
{
	this->finalScore = finalScore;
}

void ResultUI::BlackOutUpdate()
{
	//�Ó]���s������
	const int blackoutTime = 200;

	//�^�C�}�[�X�V
	timer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)timer / blackoutTime;

	//�C�[�W���O�Řg�̃��C���ύX
	float alpha = Easing::OutQuad(0, 0.6f, easeTimer);

	//�g�I�u�W�F�N�g�̑傫�����X�V
	blackoutSprite->SetColor({ 0, 0, 0, alpha });

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (timer >= blackoutTime)
	{
		//�^�C�}�[��������
		timer = 0;

		//���̕`����J�n
		drawScene++;
	}
}

void ResultUI::TimeCount()
{
	//�o�ߎ��Ԃ��X�V����
	timer++;

	//�w�肵�����ԂɂȂ�����
	const int maxTime = 60;
	if (timer >= maxTime)
	{
		//�^�C�}�[��������
		timer = 0;

		//���̕`����J�n
		drawScene++;
	}
}

void ResultUI::IncreaseDisplayScore()
{
	//�\���p�X�R�A���X�V
	displayScore += 952672;
	//�\���p�X�R�A�͍ŏI�X�R�A���z���Ȃ�
	if (displayScore >= finalScore)
	{
		displayScore = finalScore;

		//���̕`����J�n
		drawScene++;
	}
}

void ResultUI::IncreaseScoreSprite()
{
	//8���̐��������ꂼ��o�͂���
	int digit[scoreDigits];
	digit[0] = displayScore % 10;				//00000001
	digit[1] = (displayScore / 10) % 10;		//00000010
	digit[2] = (displayScore / 100) % 10;		//00000100
	digit[3] = (displayScore / 1000) % 10;		//00001000
	digit[4] = (displayScore / 10000) % 10;		//00010000
	digit[5] = (displayScore / 100000) % 10;	//00100000
	digit[6] = (displayScore / 1000000) % 10;	//01000000
	digit[7] = (displayScore / 10000000) % 10;	//10000000

	//���ꂼ��̌��̐������X�v���C�g�̃e�N�X�`���؂�o�������炷
	for (int i = 0; i < scoreDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = finalScoreSprite[i]->GetTexSize().x * digit[i];
		finalScoreSprite[i]->SetTexLeftTop(leftTop);
	}
}

