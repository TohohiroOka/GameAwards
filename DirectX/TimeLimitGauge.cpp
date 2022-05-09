#include "TimeLimitGauge.h"
#include "SafeDelete.h"
#include "Easing.h"

TimeLimitGauge* TimeLimitGauge::Create(int frameTexNum, int barTexNum)
{
	//�C���X�^���X�𐶐�
	TimeLimitGauge* instance = new TimeLimitGauge();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(frameTexNum, barTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

TimeLimitGauge::~TimeLimitGauge()
{
	safe_delete(frameSprite);
	safe_delete(barSprite);
}

bool TimeLimitGauge::Initialize(int frameTexNum, int barTexNum)
{
	//�|�C���g�\��(�g)�X�v���C�g����
	frameSprite = Sprite::Create(frameTexNum, { 0.5f, 0 });
	if (frameSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	frameSprite->SetSize({ 100, 100 });
	frameSprite->SetTexSize({ 100, 100 });
	frameSprite->SetPosition({ 640, -100 });
	//�X�v���C�g�X�V
	frameSprite->Update();


	//�|�C���g�\��(�o�[)�X�v���C�g����
	barSprite = Sprite::Create(barTexNum, { 0.5f, 0 });
	if (barSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	barSprite->SetSize({ 100, 0 });
	barSprite->SetTexSize({ 100, 100 });
	barSprite->SetPosition({ 640, -100 - lengthMax / 2 });
	//�X�v���C�g�X�V
	barSprite->Update();

	return true;
}

void TimeLimitGauge::Update()
{
	//�Q�[���V�[���̍��W�Ɉړ�
	if (isMoveGamePos)
	{
		MoveGamePos();
	}
	//���U���g�V�[���̍��W�Ɉړ�
	else if (isMoveResultPos)
	{
		MoveResultPos();
	}

	//������ύX
	if (isChangeLengthBar)
	{
		ChangeLengthBar();
	}

	//�X�v���C�g�X�V
	frameSprite->Update();
	barSprite->Update();
}

void TimeLimitGauge::Draw()
{
	//�X�v���C�g�`��
	barSprite->Draw();
	frameSprite->Draw();
}

void TimeLimitGauge::Reset()
{
	//�񕜃|�C���g
	recoveryPoint = 0;
	//�o�[�X�v���C�g�̒�����ύX���邩
	isChangeLengthBar = false;
	//�o�[�X�v���C�g�̒����ύX�^�C�}�[
	changeLengthTimer = 0;
	//�o�[�X�v���C�g�̒����ύX�O�̒���
	changeLengthBefore = 0;
	//�o�[�X�v���C�g�̒����ύX��̒���
	changeLengthAftar = 0;
	//�Q�[�W���ő傩
	isGaugeMax = false;
	//�Q�[���V�[���̍��W�Ɉړ�����
	isMoveGamePos = false;
	//�Q�[���V�[���̍��W�Ɉړ��I��������
	isMoveGamePosEnd = false;
	//�Q�[���V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[
	moveGamePosTimer = 0;
	//���U���g�V�[���̍��W�Ɉړ�����
	isMoveResultPos = false;
	//���U���g�V�[���̍��W�Ɉړ��I��������
	isMoveResultPosEnd = false;
	//���U���g�V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[
	moveResultPosTimer = 0;

	//�o�[�X�v���C�g�̒�����������
	barSprite->SetSize({ 100, 0 });
	barSprite->SetPosition({ 640, -100 - lengthMax / 2 });
	barSprite->Update();
}

void TimeLimitGauge::AddPoint(int point)
{
	//�񕜃|�C���g�����Z
	recoveryPoint += point;
	if (recoveryPoint >= recoveryPointMax)
	{
		recoveryPoint = recoveryPointMax;
	}

	//�Q�[�W�̒�����ύX��Ԃɂ���
	SetChangeLength();
}

void TimeLimitGauge::UsePoint()
{
	//�񕜃|�C���g�����Z
	recoveryPoint = 0;

	//�Q�[�W�̒�����ύX��Ԃɂ���
	SetChangeLength();
}

void TimeLimitGauge::SetMoveGamePos()
{
	//�Q�[���V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveGamePosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveGamePos = true;
}

void TimeLimitGauge::SetMoveResultPos()
{
	//���U���g�V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveResultPosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveResultPos = true;
}

void TimeLimitGauge::ChangeLengthBar()
{
	//�ύX���s������
	const int changeTime = 20;

	//�����ύX�^�C�}�[�X�V
	changeLengthTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)changeLengthTimer / changeTime;

	//�X�v���C�g�̃T�C�Y��ύX
	XMFLOAT2 size = barSprite->GetSize();
	size.y = Easing::OutQuad(changeLengthBefore, changeLengthAftar, easeTimer);
	//�X�V�����T�C�Y���Z�b�g
	barSprite->SetSize(size);
	barSprite->SetTexSize(size);
	XMFLOAT2 leftTop = barSprite->GetTexLeftTop();
	leftTop.y = 100 - size.y;
	barSprite->SetTexLeftTop(leftTop);
	XMFLOAT2 pos = barSprite->GetPosition();
	pos.y = (55 - lengthMax / 2) + leftTop.y;
	barSprite->SetPosition(pos);

	//�Q�[�W���ő�ɂȂ�����t���O��true��
	if (size.y >= lengthMax)
	{
		isGaugeMax = true;
	}
	//����ȊO��false��
	else
	{
		isGaugeMax = false;
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (changeLengthTimer >= changeTime)
	{
		//�����ύX�I��
		isChangeLengthBar = false;
	}
}

void TimeLimitGauge::SetChangeLength()
{
	//�o�[�̒����ύX�^�C�}�[��������
	changeLengthTimer = 0;
	//�C�[�W���O�p�ɕύX�O�̒������Z�b�g
	changeLengthBefore = barSprite->GetSize().y;
	//�C�[�W���O�p�ɕύX��̒������Z�b�g
	changeLengthAftar = lengthMax * ((float)recoveryPoint / recoveryPointMax);

	//�o�[�̒�����ύX��Ԃɂ���
	isChangeLengthBar = true;
}

void TimeLimitGauge::MoveGamePos()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveGamePosTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveGamePosTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 barPos = barSprite->GetPosition();
	framePos.y = Easing::OutQuint(-100 - lengthMax / 2, 55 - lengthMax / 2, easeTimer);
	XMFLOAT2 size = barSprite->GetSize();
	XMFLOAT2 leftTop = barSprite->GetTexLeftTop();
	leftTop.y = 100 - size.y;
	barPos.y = Easing::OutQuint((-100 - lengthMax / 2) + leftTop.y, (55 - lengthMax / 2) + leftTop.y, easeTimer);
	//�X�V�������W���Z�b�g
	frameSprite->SetPosition(framePos);
	barSprite->SetPosition(barPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveGamePosTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveGamePos = false;

		//�ړ�����
		isMoveGamePosEnd = true;
	}
}

void TimeLimitGauge::MoveResultPos()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveResultPosTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveResultPosTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 barPos = barSprite->GetPosition();
	framePos.y = Easing::OutQuint(55 - lengthMax / 2, -100 - lengthMax / 2, easeTimer);
	XMFLOAT2 size = barSprite->GetSize();
	XMFLOAT2 leftTop = barSprite->GetTexLeftTop();
	leftTop.y = 100 - size.y;
	barPos.y = Easing::OutQuint((55 - lengthMax / 2) + leftTop.y, (-100 - lengthMax / 2) + leftTop.y, easeTimer);
	//�X�V�������W���Z�b�g
	frameSprite->SetPosition(framePos);
	barSprite->SetPosition(barPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveResultPosTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveResultPos = false;

		//�ړ�����
		isMoveResultPosEnd = true;
	}
}
