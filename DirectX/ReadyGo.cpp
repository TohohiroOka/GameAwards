#include "ReadyGo.h"
#include "SafeDelete.h"
#include "Easing.h"

ReadyGo* ReadyGo::Create(int readyTexNum, int goTexNum)
{
	//�C���X�^���X�𐶐�
	ReadyGo* instance = new ReadyGo();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(readyTexNum, goTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ReadyGo::~ReadyGo()
{
	safe_delete(readySprite);
	safe_delete(goSprite);
}

bool ReadyGo::Initialize(int readyTexNum, int goTexNum)
{
	//Ready�X�v���C�g����
	readySprite = Sprite::Create(readyTexNum);
	if (readySprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	readySprite->SetPosition({ 640, -150 });
	//�e�N�X�`���T�C�Y���Z�b�g
	readySprite->SetTexSize({ 900, 195 });
	//�傫�����Z�b�g
	readySprite->SetSize({ 900, 195 });
	//�X�v���C�g�X�V
	readySprite->Update();


	//Go�X�v���C�g����
	goSprite = Sprite::Create(goTexNum);
	if (readySprite == nullptr) {
		return false;
	}
	//���W���Z�b�g
	goSprite->SetPosition({ 640, -150 });
	//�e�N�X�`���T�C�Y���Z�b�g
	goSprite->SetTexSize({ 384, 192 });
	//�傫�����Z�b�g
	goSprite->SetSize({ 384, 192 });
	//�X�v���C�g�X�V
	goSprite->Update();

	//Ready�X�v���C�g�𓮂�����ԂŃZ�b�g���Ă���
	SetReadySpriteMove();

	return true;
}

void ReadyGo::Update()
{
	if (!isReadySpriteMove && !isGoSpriteMove) { return; }

	//Ready�X�v���C�g�𓮂���
	if (isReadySpriteMove)
	{
		ReadySpriteMove();
	}
	//Go�X�v���C�g�𓮂���
	if (isGoSpriteMove)
	{
		GoSpriteMove();
	}

	//�X�v���C�g�X�V
	readySprite->Update();
	goSprite->Update();
}

void ReadyGo::Draw()
{
	if (!isReadySpriteMove && !isGoSpriteMove) { return; }

	//�X�v���C�g�`��
	readySprite->Draw();
	goSprite->Draw();
}

void ReadyGo::Reset()
{
	//Ready�X�v���C�g�𓮂�����
	isReadySpriteMove = false;
	//Go�X�v���C�g�𓮂�����
	isGoSpriteMove = false;
	//Ready�X�v���C�g�𓮂������ԃ^�C�}�[
	readySpriteMoveTimer = 0;
	//Go�X�v���C�g�𓮂������ԃ^�C�}�[
	goSpriteMoveTimer = 0;
	//ReadyGo���I�������
	isReadyGoEnd = false;

	//Ready�X�v���C�g�𓮂�����ԂŃZ�b�g���Ă���
	SetReadySpriteMove();
}

void ReadyGo::SetReadySpriteMove()
{
	//Ready�X�v���C�g�𓮂�����ԂɃZ�b�g
	isReadySpriteMove = true;
}

void ReadyGo::SetGoSpriteMove()
{
	//Go�X�v���C�g�𓮂�����ԂɃZ�b�g
	isGoSpriteMove = true;
}

void ReadyGo::ReadySpriteMove()
{
	//����������
	const int moveTime = 120;

	//�^�C�}�[���X�V
	readySpriteMoveTimer++;

	//�C�[�W���O�œ�����
	if (readySpriteMoveTimer < moveTime / 3 * 2)
	{
		//�C�[�W���O�v�Z�p�̎���
		const int fallTime = moveTime / 3 * 2;
		float easeTimer = (float)readySpriteMoveTimer / fallTime;

		//�C�[�W���O�ŕǃI�u�W�F�N�g�̑傫���ύX
		XMFLOAT2 pos = readySprite->GetPosition();
		pos.y = Easing::OutQuint(-150, 340, easeTimer);
		//Ready�X�v���C�g�𓮂���
		readySprite->SetPosition(pos);
	}
	else if (readySpriteMoveTimer > moveTime / 3)
	{
		//�C�[�W���O�v�Z�p�̎���
		const int endTime = moveTime;
		float easeTimer = (float)(readySpriteMoveTimer - moveTime / 3 * 2) / endTime;

		//�C�[�W���O�ŕǃI�u�W�F�N�g�̑傫���ύX
		XMFLOAT2 pos = readySprite->GetPosition();
		pos.y = Easing::OutQuint(340, 870, easeTimer);
		//Ready�X�v���C�g�𓮂���
		readySprite->SetPosition(pos);
	}

	//�^�C�}�[���w��̎��Ԃɓ��B������
	if (readySpriteMoveTimer >= moveTime)
	{
		//�ړ��I��
		isReadySpriteMove = false;

		//Go�X�v���C�g�𓮂���
		isGoSpriteMove = true;
	}
}

void ReadyGo::GoSpriteMove()
{
	//����������
	const int moveTime = 120;

	//�^�C�}�[���X�V
	goSpriteMoveTimer++;

	//�C�[�W���O�œ�����
	if (goSpriteMoveTimer < moveTime / 3 * 2)
	{
		//�C�[�W���O�v�Z�p�̎���
		const int fallTime = moveTime / 3 * 2;
		float easeTimer = (float)goSpriteMoveTimer / fallTime;

		//�C�[�W���O�ŕǃI�u�W�F�N�g�̑傫���ύX
		XMFLOAT2 pos = goSprite->GetPosition();
		pos.y = Easing::OutQuint(-150, 340, easeTimer);
		//Ready�X�v���C�g�𓮂���
		goSprite->SetPosition(pos);
	}
	else if (goSpriteMoveTimer > moveTime / 3)
	{
		//�C�[�W���O�v�Z�p�̎���
		const int endTime = moveTime;
		float easeTimer = (float)(goSpriteMoveTimer - moveTime / 3 * 2) / endTime;

		//�C�[�W���O�ŕǃI�u�W�F�N�g�̑傫���ύX
		XMFLOAT2 pos = goSprite->GetPosition();
		pos.y = Easing::OutQuint(340, 870, easeTimer);
		//Ready�X�v���C�g�𓮂���
		goSprite->SetPosition(pos);
	}

	//�^�C�}�[���w��̎��Ԃɓ��B������
	if (goSpriteMoveTimer >= moveTime)
	{
		//�ړ��I��
		isGoSpriteMove = false;

		//ReadyGo�I��
		isReadyGoEnd = true;
	}
}
