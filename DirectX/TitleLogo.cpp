#include "TitleLogo.h"
#include "SafeDelete.h"
#include "Easing.h"

TitleLogo *TitleLogo::Create(int textureNum)
{
	//�C���X�^���X�𐶐�
	TitleLogo *instance = new TitleLogo();
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

TitleLogo::~TitleLogo()
{
	safe_delete(titleSprite);
}

bool TitleLogo::Initialize(int textureNum)
{
	//�^�C�g�����S�X�v���C�g����
	titleSprite = Sprite::Create(textureNum);
	if (titleSprite == nullptr) {
		return false;
	}

	//�������W���Z�b�g
	titleSprite->SetPosition({ -1000, -1000 });
	titleSprite->SetSize({ 600, 120 });
	titleSprite->SetTexSize({ 64, 64 });

	return true;
}

void TitleLogo::Update()
{
	//������
	if (isFall)
	{
		Fall();
	}

	//�X�v���C�g�X�V
	titleSprite->Update();
}

void TitleLogo::Draw()
{
	//�X�v���C�g�`��
	titleSprite->Draw();
}

void TitleLogo::FallStart(XMFLOAT2 fallStartPosition, XMFLOAT2 fallEndPosition)
{
	//�����J�n���W���Z�b�g
	this->fallStartPosition = fallStartPosition;
	//�����n�_���Z�b�g
	this->fallEndPosition = fallEndPosition;
	//��������Ԃɂ���
	isFall = true;
}

void TitleLogo::Fall()
{
	//�������s������
	const int fallTime = 200;

	//�����^�C�}�[�X�V
	fallTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)fallTimer / fallTime;
	//�������̉�ʊO����̍��W�ړ�
	XMFLOAT2 pos = {};
	pos.x = Easing::InQuint(fallStartPosition.x, fallEndPosition.x, easeTimer);
	pos.y = Easing::InQuint(fallStartPosition.y, fallEndPosition.y, easeTimer);
	//�X�V�����A���t�@�l���Z�b�g
	titleSprite->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (fallTimer >= fallTime)
	{
		//�����I��
		isFall = false;
	}
}
