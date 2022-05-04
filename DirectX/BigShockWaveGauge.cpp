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
	frameSprite->SetSize({ 568 / 4, 52 });
	frameSprite->SetTexSize({ 568, 52 });
	frameSprite->SetPosition({ 1150 - frameSprite->GetSize().x / 2, 55 });


	//�|�C���g�\��(�o�[)�X�v���C�g����
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	barSprite->SetSize({ 0, 40 });
	barSprite->SetTexSize({ 556, 40 });
	barSprite->SetPosition({ 1150 - lengthMax / 2, 55 });


	return true;
}

void BigShockWaveGauge::Update(int combo)
{
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
	const int gaugeComboMax = 16;
	if (gaugeCombo >= gaugeComboMax)
	{
		gaugeCombo = gaugeComboMax;
	}
	changeLengthAftar = lengthMax * ((float)gaugeCombo / gaugeComboMax);

	//�o�[�̒�����ύX��Ԃɂ���
	isChangeLengthBar = true;
}
