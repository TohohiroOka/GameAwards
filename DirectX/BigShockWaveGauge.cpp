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
	frameSprite->SetSize({ 306, 49 });
	frameSprite->SetTexSize({ 306, 49 });
	frameSprite->SetPosition({ 1120 - frameSprite->GetSize().x / 2, -100 });
	//�X�v���C�g�X�V
	frameSprite->Update();

	//�|�C���g�\��(�o�[)�X�v���C�g����
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	barSprite->SetSize({ 0, 25 });
	barSprite->SetTexSize({ 258, 25 });
	barSprite->SetPosition({ 1120 - lengthMax / 2 + 22, -101 });
	//�X�v���C�g�X�V
	barSprite->Update();

	return true;
}

void BigShockWaveGauge::Update(int combo)
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

	//�R���{�����ύX����Ă����ꍇ�R���{�����擾
	if (this->combo != combo)
	{
		this->combo = combo;

		//�Q�[�W�̒�����ύX��Ԃɂ���
		SetChangeLength();
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
	frameSprite->Draw();
	barSprite->Draw();
}

void BigShockWaveGauge::Reset()
{
	//�R���{��
	combo = 0;
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

	//�o�[�X�v���C�g�̒�����������
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
	int gaugeCombo = combo;
	const int gaugeComboMax = 30;
	if (gaugeCombo >= gaugeComboMax)
	{
		gaugeCombo = gaugeComboMax;
	}
	changeLengthAftar = lengthMax * ((float)gaugeCombo / gaugeComboMax);

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
	framePos.y = Easing::OutQuint(-100, 55, easeTimer);
	barPos.y = Easing::OutQuint(-101, 56, easeTimer);
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
	framePos.y = Easing::OutQuint(55, -100, easeTimer);
	barPos.y = Easing::OutQuint(56, -101, easeTimer);
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
