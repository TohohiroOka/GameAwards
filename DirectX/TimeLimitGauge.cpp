#include "TimeLimitGauge.h"
#include "SafeDelete.h"
#include "Easing.h"

TimeLimitGauge* TimeLimitGauge::Create(int timeTexNum, int frameTexNum, int barTexNum)
{
	//�C���X�^���X�𐶐�
	TimeLimitGauge* instance = new TimeLimitGauge();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(timeTexNum, frameTexNum, barTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

TimeLimitGauge::~TimeLimitGauge()
{
	safe_delete(timeSprite);
	safe_delete(frameSprite);
	safe_delete(barSprite);
}

bool TimeLimitGauge::Initialize(int timeTexNum, int frameTexNum, int barTexNum)
{
	//TIME�X�v���C�g����
	timeSprite = Sprite::Create(timeTexNum);
	if (timeSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	timeSprite->SetSize({ 51, 20 });
	timeSprite->SetTexSize({ 51, 20 });
	timeSprite->SetPosition({ 40, -100 });
	//�X�v���C�g�X�V
	timeSprite->Update();

	//�|�C���g�\��(�g)�X�v���C�g����
	frameSprite = Sprite::Create(frameTexNum, { 0, 0.5f });
	if (frameSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	frameSprite->SetSize({ 604, 17 });
	frameSprite->SetTexSize({ 604, 17 });
	frameSprite->SetPosition({ 80, -100 });
	//�X�v���C�g�X�V
	frameSprite->Update();


	//�|�C���g�\��(�o�[)�X�v���C�g����
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	barSprite->SetSize({ 600, 13 });
	barSprite->SetTexSize({ 600, 13 });
	barSprite->SetPosition({ 82, -100 });
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

	//�񕜏��
	if (isRecovery)
	{
		RecoveryLengthBar();
	}
	//�J�E���g�_�E��
	else if (isCountDown)
	{
		CountDown();
	}

	//�X�v���C�g�X�V
	timeSprite->Update();
	barSprite->Update();
	frameSprite->Update();
}

void TimeLimitGauge::Draw()
{
	//�X�v���C�g�`��
	timeSprite->Draw();
	barSprite->Draw();
	frameSprite->Draw();
}

void TimeLimitGauge::Reset()
{
	//���Ԍv���^�C�}�[
	timer = timeLimitMax;
	//�J�E���g�_�E�����邩
	isCountDown = false;
	//�񕜒���
	isRecovery = false;
	//�J�E���g�_�E�����Ō�܂ōs������
	isCountDownEnd = false;


	//�񕜒��o�[�̒����ύX�^�C�}�[
	recoveryLengthTimer = 0;
	//�񕜒��o�[�̒����ύX�O�̒���
	recoveryLengthBefore = 0;
	//�񕜒��o�[�̒����ύX��̒���
	recoveryLengthAfter = 0;

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

	//�X�v���C�g������
	timeSprite->SetPosition({ 40, -100 });
	timeSprite->Update();
	frameSprite->SetPosition({ 80, -100 });
	frameSprite->Update();
	barSprite->SetSize({ 600, 13 });
	barSprite->SetTexSize({ 600, 13 });
	barSprite->SetPosition({ 82, -100 });
	barSprite->Update();
}

void TimeLimitGauge::Recovery(int second)
{
	//�����̕b���A�������Ԃ��񕜂���
	timer += second * 60;

	//�ő�ȏ���񕜂��Ȃ�
	if (timer >= timeLimitMax)
	{
		timer = timeLimitMax;
	}

	//�o�[�̒����ύX�ύX���Z�b�g
	SetRecoveryLengthBar();

	//�񕜏��
	isRecovery = true;
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

void TimeLimitGauge::CountDown()
{
	//���t���[���^�C�}�[���J�E���g�_�E��
	timer--;

	//0�ȉ��ɂȂ�����
	if (timer <= 0)
	{
		//0�ȉ��ɂȂ�Ȃ�
		timer = 0;

		//�J�E���g�_�E���I��
		isCountDown = false;
		isCountDownEnd = true;
	}

	//�J�E���g�_�E���ɂ��o�[�̒����ύX
	CountDownLengthBar();
}

void TimeLimitGauge::CountDownLengthBar()
{
	//�C�[�W���O�p�ɕύX��̒������Z�b�g
	float length = lengthMax * ((float)timer / timeLimitMax);

	//�X�v���C�g�̃T�C�Y��ύX
	XMFLOAT2 size = barSprite->GetSize();
	size.x = length;
	//�X�V�����T�C�Y���Z�b�g
	barSprite->SetSize(size);
	barSprite->SetTexSize(size);
}

void TimeLimitGauge::RecoveryLengthBar()
{
	//�񕜂��s������
	const int changeTime = 240;

	//�����ύX�^�C�}�[�X�V
	recoveryLengthTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)recoveryLengthTimer / changeTime;

	//�X�v���C�g�̃T�C�Y��ύX
	XMFLOAT2 size = barSprite->GetSize();
	size.x = Easing::OutQuint(recoveryLengthBefore, recoveryLengthAfter, easeTimer);
	//�X�V�����T�C�Y���Z�b�g
	barSprite->SetSize(size);
	barSprite->SetTexSize(size);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (recoveryLengthTimer >= changeTime)
	{
		//�񕜏�ԏI��
		isRecovery = false;
	}
}

void TimeLimitGauge::SetRecoveryLengthBar()
{
	//�񕜒��o�[�̒����ύX�^�C�}�[��������
	recoveryLengthTimer = 0;
	//�C�[�W���O�p�ɕύX�O�̒������Z�b�g
	recoveryLengthBefore = barSprite->GetSize().x;
	//�C�[�W���O�p�ɕύX��̒������Z�b�g
	recoveryLengthAfter = lengthMax * ((float)timer / timeLimitMax);
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
	XMFLOAT2 timePos = timeSprite->GetPosition();
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 barPos = barSprite->GetPosition();
	timePos.y = Easing::OutQuint(-100, 55, easeTimer);
	framePos.y = Easing::OutQuint(-100, 55, easeTimer);
	barPos.y = Easing::OutQuint(-100, 55, easeTimer);
	//�X�V�������W���Z�b�g
	timeSprite->SetPosition(timePos);
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
	XMFLOAT2 timePos = frameSprite->GetPosition();
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 barPos = barSprite->GetPosition();
	timePos.y = Easing::OutQuint(55, -100, easeTimer);
	framePos.y = Easing::OutQuint(55, -100, easeTimer);
	barPos.y = Easing::OutQuint(55, -100, easeTimer);
	//�X�V�������W���Z�b�g
	timeSprite->SetPosition(timePos);
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
