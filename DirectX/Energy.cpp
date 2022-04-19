#include "Energy.h"
#include "SafeDelete.h"
#include "Easing.h"

Energy* Energy::Create(int frameTexNum, int barTexNum)
{
	//�C���X�^���X�𐶐�
	Energy* instance = new Energy();
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

Energy::~Energy()
{
	safe_delete(frameSprite);
	safe_delete(barSprite);
}

bool Energy::Initialize(int frameTexNum, int barTexNum)
{
	//�|�C���g�\��(�g)�X�v���C�g����
	frameSprite = Sprite::Create(frameTexNum, { 0, 0.5f });
	if (frameSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	frameSprite->SetSize({ 568, 52 });
	frameSprite->SetTexSize({ 568, 52 });
	frameSprite->SetPosition({ 290 - frameSprite->GetSize().x / 2, 55 });


	//�|�C���g�\��(�o�[)�X�v���C�g����
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	barSprite->SetSize({ 0, 40 });
	barSprite->SetTexSize({ 0, 40 });
	barSprite->SetPosition({ 290 - lengthMax / 2, 55 });

	return true;
}

void Energy::Update()
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

void Energy::Draw()
{
	//�X�v���C�g�`��
	frameSprite->Draw();
	barSprite->Draw();
}

void Energy::Reset()
{
}

void Energy::AddEnergyPoint(const int addPoint)
{
	//�����̐������Z����
	energyPoint += addPoint;

	//�G�l���M�[�|�C���g�̏�����z���Ȃ��悤�ɂ���
	if (energyPoint >= energyPointMax)
	{
		energyPoint = energyPointMax;
	}

	//�o�[�̒����ύX��Ԃɂ���
	SetChangeLength();
}

void Energy::LoseEnergyPoint()
{
	energyPoint -= 10;

	//�G�l���M�[�|�C���g��0�ȉ��ɂȂ�Ȃ�
	if (energyPoint <= 0)
	{
		energyPoint = 0;
	}

	//�o�[�̒����ύX��Ԃɂ���
	SetChangeLength();
}

void Energy::UseTwentyPercent()
{
	//�ő�l��20%�̃|�C���g�����炷
	energyPoint -= energyPointMax / 5;

	if (energyPoint <= 0)
	{
		energyPoint = 0;
	}

	//�o�[�̒����ύX��Ԃɂ���
	SetChangeLength();
}

bool Energy::CheckPercent(int percent)
{
	//�ő�l�́���%�̃|�C���g�������Ă�����true��Ԃ�
	float pointPer = ((float)energyPoint / energyPointMax);
	float checkPer = ((float)percent / 100);
	if (pointPer >= checkPer)
	{
		return true;
	}

	return false;
}

void Energy::ChangeLengthBar()
{
	//�ύX���s������
	const int changeTime = 60;

	//�����ύX�^�C�}�[�X�V
	changeLengthTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)changeLengthTimer / changeTime;

	//�X�v���C�g�̃T�C�Y��ύX
	XMFLOAT2 size = barSprite->GetSize();
	XMFLOAT2 texSize = barSprite->GetTexSize();
	size.x = Easing::OutQuint(changeLengthBefore, changeLengthAftar, easeTimer);
	texSize.x = Easing::OutQuint(changeLengthBefore, changeLengthAftar, easeTimer);
	//�X�V�����T�C�Y���Z�b�g
	barSprite->SetSize(size);
	barSprite->SetTexSize(texSize);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (changeLengthTimer >= changeTime)
	{
		//�����ύX�I��
		isChangeLengthBar = false;
	}
}

void Energy::SetChangeLength()
{
	//�o�[�̒����ύX�^�C�}�[��������
	changeLengthTimer = 0;
	//�C�[�W���O�p�ɕύX�O�̒������Z�b�g
	changeLengthBefore = barSprite->GetSize().x;
	//�C�[�W���O�p�ɕύX��̒������Z�b�g
	changeLengthAftar = lengthMax * ((float)energyPoint / energyPointMax);

	//�o�[�̒�����ύX��Ԃɂ���
	isChangeLengthBar = true;
}
