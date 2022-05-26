#include "ShockWaveGauge.h"
#include "SafeDelete.h"
#include "Easing.h"

ShockWaveGauge* ShockWaveGauge::Create(int frameTexNum, int barTexNum)
{
	//�C���X�^���X�𐶐�
	ShockWaveGauge* instance = new ShockWaveGauge();
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

ShockWaveGauge::~ShockWaveGauge()
{
	safe_delete(frameSprite);
	safe_delete(barSprite);
}

bool ShockWaveGauge::Initialize(int frameTexNum, int barTexNum)
{
	//�Q�[�W(�g)�X�v���C�g����
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

	//�Q�[�W(�o�[)�X�v���C�g����
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

void ShockWaveGauge::Update()
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

	//�X�v���C�g�X�V
	frameSprite->Update();
	barSprite->Update();
}

void ShockWaveGauge::Draw()
{
	//�X�v���C�g�`��
	barSprite->Draw();
	frameSprite->Draw();
}

void ShockWaveGauge::Reset()
{
	//�Q�[�W�|�C���g
	gaugePoint = 0;
	//�Q�[�W���x��
	gaugeLevel = 0;
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

void ShockWaveGauge::IncreasePoint()
{
	//���ɃQ�[�W���ő�Ȃ甲����
	if (gaugePoint >= gaugePointMax) { return; }

	//�|�C���g�����Z
	gaugePoint += 3;
	//�ő�|�C���g���͉z���Ȃ�
	if (gaugePoint > gaugePointMax)
	{
		gaugePoint = gaugePointMax;
	}

	//�Q�[�W�𒷂���ύX
	ChangeLengthBar();
	//�Q�[�W���x����ύX
	ChangeGaugeLevel();
}

void ShockWaveGauge::DecreasePoint()
{
	//���ɃQ�[�W��0�Ȃ甲����
	if (gaugePoint <= 0) { return; }

	//�|�C���g������
	gaugePoint--;
	//0�ȉ��ɂȂ�Ȃ�
	if (gaugePoint < 0)
	{
		gaugePoint = 0;
	}

	//�Q�[�W�𒷂���ύX
	ChangeLengthBar();
	//�Q�[�W���x����ύX
	ChangeGaugeLevel();
}

void ShockWaveGauge::SetMoveGamePos()
{
	//�Q�[���V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveGamePosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveGamePos = true;
}

void ShockWaveGauge::SetMoveResultPos()
{
	//���U���g�V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveResultPosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveResultPos = true;
}

void ShockWaveGauge::ChangeLengthBar()
{
	//�Q�[�W�|�C���g�ɉ����ăQ�[�W�̒������Z�b�g
	float length = lengthMax * ((float)gaugePoint / gaugePointMax);

	//�X�v���C�g�̃T�C�Y��ύX
	XMFLOAT2 size = barSprite->GetSize();
	size.x = length;
	//�X�V�����T�C�Y���Z�b�g
	barSprite->SetSize(size);
	barSprite->SetTexSize(size);
}

void ShockWaveGauge::ChangeGaugeLevel()
{
	//�Q�[�W��1/3�ȉ��̂Ƃ��̓Q�[�W���x��1
	if (gaugePoint < gaugePointMax / 3) { gaugeLevel = 1; }
	//�Q�[�W��2/3�ȉ��̂Ƃ��̓Q�[�W���x��2
	else if (gaugePoint < (gaugePointMax / 3) * 2) { gaugeLevel = 2; }
	//����ȊO(2/3����ő�)�Ƃ��̓Q�[�W���x��3
	else { gaugeLevel = 3; }
}

void ShockWaveGauge::MoveGamePos()
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

void ShockWaveGauge::MoveResultPos()
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
