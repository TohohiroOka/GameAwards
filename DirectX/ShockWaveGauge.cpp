#include "ShockWaveGauge.h"
#include "SafeDelete.h"
#include "Easing.h"

ShockWaveGauge* ShockWaveGauge::Create(int gaugeTexNum)
{
	//�C���X�^���X�𐶐�
	ShockWaveGauge* instance = new ShockWaveGauge();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(gaugeTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ShockWaveGauge::~ShockWaveGauge()
{
	safe_delete(gaugeSprite);
}

bool ShockWaveGauge::Initialize(int gaugeTexNum)
{
	//�Q�[�W�X�v���C�g����
	gaugeSprite = Sprite::Create(gaugeTexNum, { 0.5f, 0 });
	if (gaugeSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	gaugeSprite->SetSize({ 191, 0 });
	gaugeSprite->SetTexSize({ 191, 192 });
	gaugeSprite->SetPosition({ 642.5f, 393 - lengthMax / 2 });
	//�X�v���C�g�X�V
	gaugeSprite->Update();

	return true;
}

void ShockWaveGauge::Update()
{
	//�X�V���Ȃ��ꍇ������
	if (!isUpdate) { return; }

	//�Q�[�W���Z�b�g
	if (isGaugeReset)
	{
		GaugeReset();
	}

	//�X�v���C�g�X�V
	gaugeSprite->Update();
}

void ShockWaveGauge::Draw()
{
	//�X�V���Ȃ��ꍇ������
	if (!isUpdate) { return; }
	//�X�v���C�g�`��
	gaugeSprite->Draw();
}

void ShockWaveGauge::Reset()
{
	//�Q�[�W�|�C���g
	gaugePoint = 0;
	//�Q�[�W���x��
	gaugeLevel = 0;
	//�X�V���邩
	isUpdate = false;

	//�X�v���C�g��������
	gaugeSprite->SetSize({ 191, 0 });
	gaugeSprite->SetTexSize({ 191, 192 });
	gaugeSprite->SetPosition({ 642.5f, 393 - lengthMax / 2 });
	gaugeSprite->Update();
}

void ShockWaveGauge::SetGaugeReset()
{
	//�Q�[�W�|�C���g
	gaugePoint = 0;
	//�Q�[�W���x��
	gaugeLevel = 0;

	//�Q�[�W���Z�b�g�ɃZ�b�g
	isGaugeReset = true;
	//�^�C�}�[��������
	gaugeResetTimer = 0;

	//���Z�b�g�O�̒������L�^���Ă���
	resetGaugeBeforeLength = gaugeSprite->GetSize();

	//�X�v���C�g��������
	gaugeSprite->SetSize({ 191, 0 });
	gaugeSprite->SetTexSize({ 191, 192 });
	gaugeSprite->SetPosition({ 642.5f, 393 - lengthMax / 2 });
	gaugeSprite->Update();
}

void ShockWaveGauge::IncreasePoint()
{
	//�X�V���Ȃ��ꍇ������
	if (!isUpdate) { return; }

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
	//�X�V���Ȃ��ꍇ������
	if (!isUpdate) { return; }

	//���ɃQ�[�W��0�Ȃ甲����
	if (gaugePoint <= 0) { return; }

	//�|�C���g������
	gaugePoint -= 2;
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

void ShockWaveGauge::ChangeLengthBar()
{
	//�Q�[�W�|�C���g�ɉ����ăQ�[�W�̒������Z�b�g
	float length = lengthMax * ((float)gaugePoint / gaugePointMax);

	//�X�v���C�g�̃T�C�Y��ύX
	XMFLOAT2 size = gaugeSprite->GetSize();
	size.y = length;
	//�X�V�����T�C�Y���Z�b�g
	gaugeSprite->SetSize(size);
	gaugeSprite->SetTexSize(size);
	XMFLOAT2 leftTop = gaugeSprite->GetTexLeftTop();
	leftTop.y = lengthMax - size.y;
	gaugeSprite->SetTexLeftTop(leftTop);
	XMFLOAT2 pos = gaugeSprite->GetPosition();
	pos.y = (393 - lengthMax / 2) + (lengthMax - size.y);
	gaugeSprite->SetPosition(pos);
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

void ShockWaveGauge::GaugeReset()
{
	//�Q�[�W���Z�b�g���鎞��
	const int resetTime = 100;

	//�^�C�}�[���X�V
	gaugeResetTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)gaugeResetTimer / resetTime;

	//�X�v���C�g�̃T�C�Y��ύX
	XMFLOAT2 size = gaugeSprite->GetSize();
	size.y = Easing::OutCubic(resetGaugeBeforeLength.y, 0, easeTimer);
	//�X�V�����T�C�Y���Z�b�g
	gaugeSprite->SetSize(size);
	gaugeSprite->SetTexSize(size);
	XMFLOAT2 leftTop = gaugeSprite->GetTexLeftTop();
	leftTop.y = lengthMax - size.y;
	gaugeSprite->SetTexLeftTop(leftTop);
	XMFLOAT2 pos = gaugeSprite->GetPosition();
	pos.y = (393 - lengthMax / 2) + (lengthMax - size.y);
	gaugeSprite->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (gaugeResetTimer >= resetTime)
	{
		//���Z�b�g�I��
		isGaugeReset = false;
	}
}
