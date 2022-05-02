#include "BreakScore.h"
#include "SafeDelete.h"
#include "Easing.h"

using namespace DirectX;

BreakScore* BreakScore::Create(int numberTexNum, int breakTexNum)
{
	//�C���X�^���X�𐶐�
	BreakScore* instance = new BreakScore();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(numberTexNum, breakTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

BreakScore::~BreakScore()
{
	for (int i = 0; i < scoreDigits; i++)
	{
		safe_delete(scoreSprite[i]);
	}

	safe_delete(breakSprite);
}

bool BreakScore::Initialize(int numberTexNum, int breakTexNum)
{
	//�X�R�A�̌�������
	for (int i = 0; i < scoreDigits; i++)
	{
		//�X�R�A�\���p�X�v���C�g����
		scoreSprite[i] = Sprite::Create(numberTexNum);
		if (scoreSprite[i] == nullptr) {
			return false;
		}

		//�傫�����Z�b�g
		XMFLOAT2 size = { 32, 64 };
		scoreSprite[i]->SetSize(size);

		//�e�N�X�`���T�C�Y���Z�b�g
		XMFLOAT2 texSize = { 32, 64 };
		scoreSprite[i]->SetTexSize(texSize);

		//���W���Z�b�g
		XMFLOAT2 pos = { 400, 55 };
		pos.x -= size.x * i;
		scoreSprite[i]->SetPosition(pos);
	}

	//BREAK�X�v���C�g����
	breakSprite = Sprite::Create(breakTexNum);
	if (breakSprite == nullptr) {
		return false;
	}

	//�傫�����Z�b�g
	XMFLOAT2 size = { 250, 50 };
	breakSprite->SetSize(size);

	//�e�N�X�`���T�C�Y���Z�b�g
	XMFLOAT2 texSize = { 300, 64 };
	breakSprite->SetTexSize(texSize);

	//���W���Z�b�g
	XMFLOAT2 pos = { 152, 55 };
	breakSprite->SetPosition(pos);

	//�X�R�A������
	score = 0;

	return true;
}

void BreakScore::Update()
{
	//�X�v���C�g�X�V
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Update();
	}
	breakSprite->Update();
}

void BreakScore::Draw()
{
	//�X�v���C�g�`��
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Draw();
	}
	breakSprite->Draw();
}

void BreakScore::Reset()
{
	//�X�R�A������
	score = 0;
	//�\���p�X�v���C�g��0�ɖ߂�
	ChangeScoreSprite();
	//�X�v���C�g�X�V
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Update();
	}
}

void BreakScore::AddScore()
{	//�X�R�A���X�V
	score++;
	//�X�R�A�͍ő吔���z���Ȃ�
	const int max = 9999;
	if (score >= max)
	{
		score = max;
	}

	//�ύX�O�̕\���p
	ChangeScoreSprite();
}

void BreakScore::ChangeScoreSprite()
{
	//8���̐��������ꂼ��o�͂���
	int digit[scoreDigits];
	digit[0] = score % 10;				//0001
	digit[1] = (score / 10) % 10;		//0010
	digit[2] = (score / 100) % 10;		//0100
	digit[3] = (score / 1000) % 10;		//1000

	//���ꂼ��̌��̐������X�v���C�g�̃e�N�X�`���؂�o�������炷
	for (int i = 0; i < scoreDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = scoreSprite[i]->GetTexSize().x * digit[i];
		scoreSprite[i]->SetTexLeftTop(leftTop);
	}
}
