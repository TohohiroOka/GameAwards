#include "Finish.h"
#include "SafeDelete.h"
#include "Easing.h"

Finish* Finish::Create(int finishTexNum)
{
	//�C���X�^���X�𐶐�
	Finish* instance = new Finish();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(finishTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Finish::~Finish()
{
	safe_delete(finishSprite);
}

bool Finish::Initialize(int finishTexNum)
{
	//Finish�X�v���C�g����
	finishSprite = Sprite::Create(finishTexNum);
	if (finishSprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	finishSprite->SetPosition({ 640, -150 });
	//�e�N�X�`���T�C�Y���Z�b�g
	finishSprite->SetTexSize({ 765, 183 });
	//�傫�����Z�b�g
	finishSprite->SetSize({ 765, 183 });
	//�X�v���C�g�X�V
	finishSprite->Update();

	//Finish�X�v���C�g�𓮂�����ԂŃZ�b�g���Ă���
	SetFinishSpriteMove();

	return true;
}

void Finish::Update()
{
	if (!isFinishSpriteMove) { return; }

	//Finish�X�v���C�g�𓮂���
	if (isFinishSpriteMove)
	{
		FinishSpriteMove();
	}

	//�X�v���C�g�X�V
	finishSprite->Update();
}

void Finish::Draw()
{
	if (!isFinishSpriteMove) { return; }

	//�X�v���C�g�`��
	finishSprite->Draw();
}

void Finish::Reset()
{
	//Finish�X�v���C�g�𓮂�����
	isFinishSpriteMove = false;
	//Finish���I�������
	isFinishSpriteMoveEnd = false;
	//Finish�X�v���C�g�𓮂������ԃ^�C�}�[
	finishSpriteMoveTimer = 0;

	//Finish�X�v���C�g�𓮂�����ԂŃZ�b�g���Ă���
	SetFinishSpriteMove();
}

void Finish::SetFinishSpriteMove()
{
	//Finish�X�v���C�g�𓮂�����ԂɃZ�b�g
	isFinishSpriteMove = true;
}

void Finish::FinishSpriteMove()
{
	//����������
	const int moveTime = 180;

	//�^�C�}�[���X�V
	finishSpriteMoveTimer++;

	//�C�[�W���O�œ�����
	if (finishSpriteMoveTimer < moveTime / 3 * 2)
	{
		//�C�[�W���O�v�Z�p�̎���
		const int fallTime = moveTime / 3 * 2;
		float easeTimer = (float)finishSpriteMoveTimer / fallTime;

		//�C�[�W���O�ŕǃI�u�W�F�N�g�̑傫���ύX
		XMFLOAT2 pos = finishSprite->GetPosition();
		pos.y = Easing::OutQuint(-150, 340, easeTimer);
		//Finish�X�v���C�g�𓮂���
		finishSprite->SetPosition(pos);
	}
	else if (finishSpriteMoveTimer > moveTime / 3)
	{
		//�C�[�W���O�v�Z�p�̎���
		const int endTime = moveTime;
		float easeTimer = (float)(finishSpriteMoveTimer - moveTime / 3 * 2) / endTime;

		//�C�[�W���O�ŕǃI�u�W�F�N�g�̑傫���ύX
		XMFLOAT2 pos = finishSprite->GetPosition();
		pos.y = Easing::OutQuint(340, 870, easeTimer);
		//Finish�X�v���C�g�𓮂���
		finishSprite->SetPosition(pos);
	}

	//�^�C�}�[���w��̎��Ԃɓ��B������
	if (finishSpriteMoveTimer >= moveTime)
	{
		//�ړ���ԏI��
		isFinishSpriteMove = false;

		//�ړ�����
		isFinishSpriteMoveEnd = true;
	}
}
