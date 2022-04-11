#include "Score.h"
#include "SafeDelete.h"
#include "Easing.h"

using namespace DirectX;

Score* Score::Create(int textureNum)
{
	//�C���X�^���X�𐶐�
	Score* instance = new Score();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(textureNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Score::~Score()
{
	for (int i = 0; i < scoreDigits; i++)
	{
		safe_delete(scoreSprite[i]);
	}
}

bool Score::Initialize(int textureNum)
{
	//�X�R�A�̌�������
	for (int i = 0; i < scoreDigits; i++)
	{
		//�X�R�A�\���p�X�v���C�g����
		scoreSprite[i] = Sprite::Create(textureNum);
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
		XMFLOAT2 pos = { 752, 55 };
		pos.x -= size.x * i;
		scoreSprite[i]->SetPosition(pos);
	}

	//�X�R�A������
	score = 0;

	return true;
}

void Score::Update()
{
	//�\���p�X�R�A�ύX��ԂȂ�
	if (isChangeDisplayScore)
	{
		//�X�R�A�ύX
		ChangeScore();
		ChangeScoreSprite();
	}

	//�X�v���C�g�X�V
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Update();
	}
}

void Score::Draw()
{
	//�X�v���C�g�`��
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Draw();
	}
}

void Score::Reset()
{
	//�X�R�A������
	score = 0;
	//�\���p�X�R�A������
	displayScore = 0;
	//�\���p�X�v���C�g��0�ɖ߂�
	ChangeScoreSprite();
	//�ύX�O�̕\���p�X�R�A������
	changeDisplayScoreStart = 0;
	//�ύX��̕\���p�X�R�A������
	changeDisplayScoreEnd = 0;
	//�\���p�X�R�A�ύX�p�̃^�C�}�[������
	changeDisplayScoreTimer = 0;
	//�\���p�X�R�A�ύX���ł͂Ȃ�
	isChangeDisplayScore = false;
	//�X�v���C�g�X�V
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Update();
	}
}

void Score::SetAddScore(int addScore)
{	//�X�R�A���X�V
	score += addScore;
	//�X�R�A�͍ő吔���z���Ȃ�
	const int max = 99999999;
	if (score >= max)
	{
		score = max;
	}

	//���ɕ\���p�X�R�A���J���X�g���Ă����珈�����΂�
	if (displayScore >= max) { return; }
	//���ɕύX��̕\���p�X�R�A���J���X�g���Ă����珈�����΂�
	if (changeDisplayScoreEnd >= max) { return; }

	//�ύX�O�̕\���p�X�R�A���Z�b�g
	changeDisplayScoreStart = displayScore;
	//�ύX��̕\���p�X�R�A���Z�b�g
	changeDisplayScoreEnd = score;


	//�\���p�X�R�A�ύX�^�C�}�[��������
	changeDisplayScoreTimer = 0;
	//�\���p�X�R�A��ύX���ɂ���
	isChangeDisplayScore = true;
}

void Score::ChangeScore()
{
	//�\���p�X�R�A�ύX���s������
	const int changeTime = 30;

	//�\���p�X�R�A�ύX�^�C�}�[�X�V
	changeDisplayScoreTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)changeDisplayScoreTimer / changeTime;

	//�C�[�W���O�ŃX�R�A�ύX
	float newScore = Easing::Lerp((float)changeDisplayScoreStart, (float)changeDisplayScoreEnd, easeTimer);

	//�\���p�X�R�A���X�V
	displayScore = (int)newScore;
	//�\���p�X�R�A�͍ő吔���z���Ȃ�
	if (displayScore >= changeDisplayScoreEnd)
	{
		displayScore = changeDisplayScoreEnd;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (changeDisplayScoreTimer >= changeTime)
	{
		//�\���p�X�R�A�̂�����C��
		if (displayScore != changeDisplayScoreEnd)
		{
			displayScore = changeDisplayScoreEnd;
		}

		//�\���p�X�R�A�ύX��ԏI��
		isChangeDisplayScore = false;
	}
}

void Score::ChangeScoreSprite()
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
		leftTop.x = scoreSprite[i]->GetTexSize().x * digit[i];
		scoreSprite[i]->SetTexLeftTop(leftTop);
	}
}
