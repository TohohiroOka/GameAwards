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
	frameSprite = Sprite::Create(frameTexNum, { 0, 0.5f });
	if (frameSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	frameSprite->SetSize({ 568 / 5, 104 });
	frameSprite->SetTexSize({ 568, 52 });
	frameSprite->SetPosition({ 640, 120 });
	frameSprite->SetRotation(270);


	//�|�C���g�\��(�o�[)�X�v���C�g����
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	barSprite->SetSize({ 0, 80 });
	barSprite->SetTexSize({ 556, 40 });
	barSprite->SetPosition({ 640, 120 });
	barSprite->SetRotation(270);
	barSprite->SetColor({ 0, 1, 0, 1 });

	return true;
}

void TimeLimitGauge::Update()
{
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
	frameSprite->Draw();
	barSprite->Draw();
}

void TimeLimitGauge::Reset()
{
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
	size.x = Easing::OutQuad(changeLengthBefore, changeLengthAftar, easeTimer);
	//�X�V�����T�C�Y���Z�b�g
	barSprite->SetSize(size);

	//�Q�[�W���ő�ɂȂ�����t���O��true��
	if (size.x >= lengthMax)
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
	changeLengthBefore = barSprite->GetSize().x;
	//�C�[�W���O�p�ɕύX��̒������Z�b�g
	changeLengthAftar = lengthMax * ((float)recoveryPoint / recoveryPointMax);

	//�o�[�̒�����ύX��Ԃɂ���
	isChangeLengthBar = true;
}
