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
	frameSprite->SetSize({ 229, 36 });
	frameSprite->SetTexSize({ 229, 36 });
	XMFLOAT2 frameSize = frameSprite->GetSize();
	frameSprite->SetPosition({ 640 - frameSize.x / 2, 260 });
	//�X�v���C�g�X�V
	frameSprite->Update();

	//�Q�[�W(�o�[)�X�v���C�g����
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	barSprite->SetSize({ 0, 36 });
	barSprite->SetTexSize({ 0, 36 });
	barSprite->SetPosition({ 640 - frameSize.x / 2, 260 });
	//�X�v���C�g�X�V
	barSprite->Update();

	return true;
}

void ShockWaveGauge::Update()
{
	//�X�V���Ȃ��ꍇ������
	if (!isUpdate) { return; }

	//�`�掞�Ԃ��J�E���g
	if (isDrawTimeCount)
	{
		CountDrawTimer();
	}
	//����
	else if (isTransparent)
	{
		Transparent();
	}

	//�X�v���C�g�X�V
	frameSprite->Update();
	barSprite->Update();
}

void ShockWaveGauge::Draw()
{
	//�X�V���Ȃ��ꍇ������
	if (!isUpdate) { return; }
	//�`�悵�Ȃ��ꍇ������
	if (!isDraw) { return; }
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
	//�O�̃t���[���̃Q�[�W���x��
	oldGaugeLevel = 0;
	//�O�̃t���[���ƃQ�[�W���x�����Ⴄ��
	isChangeGaugeLevel = false;
	//�X�V���邩
	isUpdate = false;
	//�`�悷�邩
	isDraw = false;
	//�`�掞�ԃJ�E���g���邩
	isDrawTimeCount = false;
	//�`�掞�ԃJ�E���g�^�C�}�[
	drawTimer = 0;
	//���߂����邩
	isTransparent = false;
	//���߂����鎞��
	transparentTimer = 0;

	//�X�v���C�g��������
	frameSprite->SetColor({ 1, 1, 1, 0 });
	frameSprite->Update();
	barSprite->SetSize({ 0, 36 });
	barSprite->SetTexSize({ 0, 36 });
	barSprite->SetColor({ 1, 1, 1, 0 });
	barSprite->Update();
}

void ShockWaveGauge::GaugeReset()
{
	//�Q�[�W�|�C���g
	gaugePoint = 0;
	//�Q�[�W���x��
	gaugeLevel = 0;
	//�O�̃t���[���̃Q�[�W���x��
	oldGaugeLevel = 0;
	//�O�̃t���[���ƃQ�[�W���x�����Ⴄ��
	isChangeGaugeLevel = false;
	//�`�悷�邩
	isDraw = false;
	//�`�掞�ԃJ�E���g���邩
	isDrawTimeCount = false;
	//�`�掞�ԃJ�E���g�^�C�}�[
	drawTimer = 0;
	//���߂����邩
	isTransparent = false;
	//���߂����鎞��
	transparentTimer = 0;

	//�X�v���C�g��������
	barSprite->SetSize({ 0, 36 });
	barSprite->SetTexSize({ 0, 36 });
	barSprite->Update();
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
	//�`�悷��
	DrawStart();
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
	//�Q�[�W���x�����ύX���Ă�����`�悷��
	if (isChangeGaugeLevel)
	{
		DrawStart();
	}
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

	//���t���[���ύX�͂Ȃ��̂Ńt���O�������Ă���
	isChangeGaugeLevel = false;
	//�O�̃t���[���ƃQ�[�W���x�����������t���O�𗧂Ă�
	if (gaugeLevel != oldGaugeLevel) { isChangeGaugeLevel = true; }
	//���̃t���[���p��old���X�V
	oldGaugeLevel = gaugeLevel;
}

void ShockWaveGauge::DrawStart()
{
	//�`�悷��
	isDraw = true;
	//�`�掞�ԃJ�E���g���邩
	isDrawTimeCount = true;
	//���߂��Ȃ�
	isTransparent = false;

	//�^�C�}�[��������
	drawTimer = 0;
	transparentTimer = 0;

	//���߂��Ă����ꍇ�߂�
	XMFLOAT4 color = barSprite->GetColor();
	color.w = 1;
	frameSprite->SetColor(color);
	barSprite->SetColor(color);
}

void ShockWaveGauge::CountDrawTimer()
{
	//�`�悷�鎞��
	const int drawCountTime = 60;

	//�^�C�}�[���X�V
	drawTimer++;

	//�^�C�}�[���w�肵�����Ԃɓ��B������
	if (drawTimer >= drawCountTime)
	{
		//�`�掞�ԃJ�E���g�I��
		isDrawTimeCount = false;
		//���ߊJ�n
		isTransparent = true;
	}
}

void ShockWaveGauge::Transparent()
{
	//���߂����鎞��
	const int transparentTime = 60;

	//�^�C�}�[���X�V
	transparentTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)transparentTimer / transparentTime;

	//�C�[�W���O�œ��߂�����
	XMFLOAT4 color = barSprite->GetColor();
	color.w = Easing::Lerp(1.0f, 0.1f, easeTimer);;
	frameSprite->SetColor(color);
	barSprite->SetColor(color);

	//�^�C�}�[���w�肵�����Ԃɓ��B������
	if (transparentTimer >= transparentTime)
	{
		//���ߏI��
		isTransparent = false;
		//�`��I��
		isDraw = false;
	}
}
