#include "BigShockWaveGauge.h"
#include "SafeDelete.h"
#include "Easing.h"

BigShockWaveGauge* BigShockWaveGauge::Create(int frameTexNum, int barTexNum)
{
	//�C���X�^���X�𐶐�
	BigShockWaveGauge* instance = new BigShockWaveGauge();
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

BigShockWaveGauge::~BigShockWaveGauge()
{
	safe_delete(frameSprite);
	safe_delete(barSprite);
}

bool BigShockWaveGauge::Initialize(int frameTexNum, int barTexNum)
{
	//�|�C���g�\��(�g)�X�v���C�g����
	frameSprite = Sprite::Create(frameTexNum, { 0, 0.5f });
	if (frameSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	frameSprite->SetSize({ 165, 30 });
	frameSprite->SetTexSize({ 165, 30 });
	frameSprite->SetPosition({ 850, -100 });
	//�X�v���C�g�X�V
	frameSprite->Update();

	//�|�C���g�\��(�o�[)�X�v���C�g����
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	barSprite->SetSize({ 0, 13 });
	barSprite->SetTexSize({ 134, 13 });
	barSprite->SetPosition({ 850 + 29, -99 });
	//�X�v���C�g�X�V
	barSprite->Update();

	return true;
}

void BigShockWaveGauge::Update()
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

void BigShockWaveGauge::Draw()
{
	//�X�v���C�g�`��
	barSprite->Draw();
	frameSprite->Draw();
}

void BigShockWaveGauge::Reset()
{
	//�|�C���g
	point = 0;
	//�Q�[�W���x��
	gaugeLevel = 0;
	//�o�[�X�v���C�g�̒�����ύX���邩
	isChangeLengthBar = false;
	//�o�[�X�v���C�g�̒����ύX�^�C�}�[
	changeLengthTimer = 0;
	//�o�[�X�v���C�g�̒����ύX�O�̒���
	changeLengthBefore = 0;
	//�o�[�X�v���C�g�̒����ύX��̒���
	changeLengthAftar = 0;
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

	//�X�v���C�g��������
	frameSprite->SetSize({ 165, 30 });
	frameSprite->SetPosition({ 850, -100 });
	frameSprite->Update();
	barSprite->SetSize({ 0, 13 });
	barSprite->SetTexSize({ 134, 13 });
	barSprite->SetPosition({ 850 + 29, -99 });
	barSprite->Update();
}

void BigShockWaveGauge::AddPoint()
{
	//�|�C���g��1���Z
	point++;

	//�ő�|�C���g���͉z���Ȃ�
	if (point > maxPoint)
	{
		point = maxPoint;

		return;
	}

	//�Q�[�W���x�����X�V
	gaugeLevel = point / (maxPoint / 3);

	//�Q�[�W�𒷂���ύX��Ԃɂ���
	SetChangeLength();
}

void BigShockWaveGauge::UsePoint()
{
	//�|�C���g��0�ɂ���
	point = 0;

	//�Q�[�W���x����0�ɂ���
	gaugeLevel = 0;

	//�Q�[�W�𒷂���ύX��Ԃɂ���
	SetChangeLength();
}

void BigShockWaveGauge::SetMoveGamePos()
{
	//�Q�[���V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveGamePosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveGamePos = true;
}

void BigShockWaveGauge::SetMoveResultPos()
{
	//���U���g�V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveResultPosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveResultPos = true;
}

void BigShockWaveGauge::ChangeLengthBar()
{
	//�ύX���s������
	const int changeTime = 40;

	//�����ύX�^�C�}�[�X�V
	changeLengthTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)changeLengthTimer / changeTime;

	//�X�v���C�g�̃T�C�Y��ύX
	XMFLOAT2 size = barSprite->GetSize();
	size.x = Easing::OutQuint(changeLengthBefore, changeLengthAftar, easeTimer);
	//�X�V�����T�C�Y���Z�b�g
	barSprite->SetSize(size);
	barSprite->SetTexSize(size);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (changeLengthTimer >= changeTime)
	{
		//�����ύX�I��
		isChangeLengthBar = false;
	}
}

void BigShockWaveGauge::SetChangeLength()
{
	//�o�[�̒����ύX�^�C�}�[��������
	changeLengthTimer = 0;
	//�C�[�W���O�p�ɕύX�O�̒������Z�b�g
	changeLengthBefore = barSprite->GetSize().x;
	//�C�[�W���O�p�ɕύX��̒������Z�b�g
	changeLengthAftar = lengthMax * ((float)point / maxPoint);

	//�o�[�̒�����ύX��Ԃɂ���
	isChangeLengthBar = true;
}

void BigShockWaveGauge::MoveGamePos()
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
	framePos.y = Easing::OutQuint(-100, 20, easeTimer);
	barPos.y = Easing::OutQuint(-99, 21, easeTimer);
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

void BigShockWaveGauge::MoveResultPos()
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
	framePos.y = Easing::OutQuint(20, -100, easeTimer);
	barPos.y = Easing::OutQuint(21, -99, easeTimer);
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
