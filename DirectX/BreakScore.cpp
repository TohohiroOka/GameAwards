#include "BreakScore.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "Input.h"
#include "XInputManager.h"

BreakScore* BreakScore::Create(int breakTexNum, int numberTexNum)
{
	//�C���X�^���X�𐶐�
	BreakScore* instance = new BreakScore();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(breakTexNum, numberTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

BreakScore::~BreakScore()
{
	safe_delete(breakSprite);
	for (int i = 0; i < breakDigits; i++)
	{
		safe_delete(breakNumSprite[i]);
	}
}

bool BreakScore::Initialize(int breakTexNum, int numberTexNum)
{
	//BREAK�X�v���C�g����
	breakSprite = Sprite::Create(breakTexNum);
	if (breakSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	breakSprite->SetPosition({ 600, -100 });
	//�e�N�X�`���T�C�Y���Z�b�g
	breakSprite->SetTexSize({ 234, 64 });
	//�傫�����Z�b�g
	breakSprite->SetSize({ 175.5f, 48 });
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
		XMFLOAT2 size = { 36, 48 };
		breakNumSprite[i]->SetSize(size);
		//�e�N�X�`���T�C�Y���Z�b�g
		XMFLOAT2 texSize = { 48, 64 };
		breakNumSprite[i]->SetTexSize(texSize);
		//���W���Z�b�g
		XMFLOAT2 pos = { 700, -100 };
		pos.x -= size.x * i;
		breakNumSprite[i]->SetPosition(pos);
		//�X�v���C�g�X�V
		breakNumSprite[i]->Update();
	}

	return true;
}

void BreakScore::Update()
{
	//�X�v���C�g�𓮂���
	if (isMoveSprite)
	{
		//�~��
		if (moveSpriteScene == MoveSpriteScene::Fall)
		{
			SpriteFall();
		}
		//��~
		else if (moveSpriteScene == MoveSpriteScene::Stay)
		{
			SpriteStay();
		}
		//�㏸
		else if (moveSpriteScene == MoveSpriteScene::Rize)
		{
			SpriteRize();
		}
	}

	//�X�v���C�g�X�V
	breakSprite->Update();
	for (int i = 0; i < breakDigits; i++)
	{
		breakNumSprite[i]->Update();
	}
}

void BreakScore::Draw()
{
	//�X�v���C�g�`��
	breakSprite->Draw();

	if (breakScore >= 1000)
	{
		breakNumSprite[3]->Draw();
	}
	if (breakScore >= 100)
	{
		breakNumSprite[2]->Draw();
	}
	if (breakScore >= 10)
	{
		breakNumSprite[1]->Draw();
	}
	breakNumSprite[0]->Draw();
}

void BreakScore::Reset()
{
	//�ǔj�󖇐�
	breakScore = 0;

	//�X�v���C�g�𓮂�����
	isMoveSprite = false;
	//�X�v���C�g�̍s��
	moveSpriteScene = MoveSpriteScene::None;
	//�X�v���C�g�~���^�C�}�[
	spriteFallTimer = 0;
	//�X�v���C�g��~�^�C�}�[
	spriteStayTimer = 0;
	//�X�v���C�g�㏸�^�C�}�[
	spriteRizeTimer = 0;

	//�X�v���C�g������
	//�X�v���C�g�̐�����ǔj�󐔂ɍ��킹��
	ChangeBreakNumSprite();
	breakSprite->SetPosition({ 600, -100 });
	breakSprite->Update();
	for (int i = 0; i < breakDigits; i++)
	{
		//�傫�����Z�b�g
		XMFLOAT2 size = breakNumSprite[i]->GetSize();
		//���W���Z�b�g
		XMFLOAT2 pos = { 700, -100 };
		pos.x -= size.x * i;
		breakNumSprite[i]->SetPosition(pos);
		//�X�v���C�g�X�V
		breakNumSprite[i]->Update();
	}
}

void BreakScore::AddScore()
{
	//�ǔj�󐔂�1���₷
	breakScore++;
	//�ő�X�R�A���͉z���Ȃ�
	const int breakScoreMax = 9999;
	if (breakScore > breakScoreMax)
	{
		breakScore = breakScoreMax;
	}
	//�X�v���C�g�̐�����ǔj�󐔂ɍ��킹��
	ChangeBreakNumSprite();

	//�X�v���C�g�𓮂�����Ԃɂ���
	SetMoveSprite();
}

void BreakScore::ChangeBreakNumSprite()
{
	//���������ꂼ��o�͂���
	int digit[breakDigits];
	digit[0] = breakScore % 10;			//0001
	digit[1] = (breakScore / 10) % 10;	//0010
	digit[2] = (breakScore / 100) % 10;	//0100
	digit[3] = (breakScore / 1000) % 10;//1000

	//���ꂼ��̌��̐������X�v���C�g�̃e�N�X�`���؂�o�������炷
	for (int i = 0; i < breakDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = breakNumSprite[i]->GetTexSize().x * digit[i];
		breakNumSprite[i]->SetTexLeftTop(leftTop);

		//�\�������ɂ����W�����炷
		XMFLOAT2 pos = breakNumSprite[i]->GetPosition();
		XMFLOAT2 size = breakNumSprite[i]->GetSize();
		const float basePosX = 720;
		if (breakScore >= 1000) { pos.x = basePosX + size.x * 3; }
		else if (breakScore >= 100) { pos.x = basePosX + size.x * 2; }
		else if (breakScore >= 10) { pos.x = basePosX + size.x; }
		else { pos.x = basePosX; }

		pos.x -= size.x * i;
		breakNumSprite[i]->SetPosition(pos);
	}
}

void BreakScore::SetMoveSprite()
{
	//�^�C�}�[��������
	//�X�v���C�g�~���^�C�}�[
	spriteFallTimer = 0;
	//�X�v���C�g��~�^�C�}�[
	spriteStayTimer = 0;
	//�X�v���C�g�㏸�^�C�}�[
	spriteRizeTimer = 0;

	//�s�����~����ԂɃZ�b�g
	moveSpriteScene = MoveSpriteScene::Fall;

	//��������ԂɃZ�b�g
	isMoveSprite = true;
}

void BreakScore::SpriteFall()
{
	//�~�����s������
	const int fallTime = 60;

	//�^�C�}�[���X�V
	spriteFallTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)spriteFallTimer / fallTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 breakPos = breakSprite->GetPosition();
	breakPos.y = Easing::OutQuint(-100, 170, easeTimer);
	//�X�V�������W���Z�b�g
	breakSprite->SetPosition(breakPos);

	for (int i = 0; i < breakDigits; i++)
	{
		XMFLOAT2 breakNumPos = breakNumSprite[i]->GetPosition();
		breakNumPos.y = Easing::OutQuint(-100, 170, easeTimer);
		//�X�V�������W���Z�b�g
		breakNumSprite[i]->SetPosition(breakNumPos);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (spriteFallTimer >= fallTime)
	{
		//��~��Ԃɂ���
		moveSpriteScene = MoveSpriteScene::Stay;
	}
}

void BreakScore::SpriteStay()
{
	//��~���s������
	const int stayTime = 90;

	//�^�C�}�[���X�V
	spriteStayTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (spriteStayTimer >= stayTime)
	{
		//�㏸��Ԃɂ���
		moveSpriteScene = MoveSpriteScene::Rize;
	}
}

void BreakScore::SpriteRize()
{
	//�㏸���s������
	const int rizeTime = 60;

	//�^�C�}�[���X�V
	spriteRizeTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)spriteRizeTimer / rizeTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 breakPos = breakSprite->GetPosition();
	breakPos.y = Easing::OutQuint(170, -100, easeTimer);
	//�X�V�������W���Z�b�g
	breakSprite->SetPosition(breakPos);

	for (int i = 0; i < breakDigits; i++)
	{
		XMFLOAT2 breakNumPos = breakNumSprite[i]->GetPosition();
		breakNumPos.y = Easing::OutQuint(170, -100, easeTimer);
		//�X�V�������W���Z�b�g
		breakNumSprite[i]->SetPosition(breakNumPos);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (spriteRizeTimer >= rizeTime)
	{
		//�X�v���C�g�𓮂�����Ԃ�����
		isMoveSprite = false;

		//�s�����~
		moveSpriteScene = MoveSpriteScene::None;
	}
}
