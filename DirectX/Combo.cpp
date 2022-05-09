#include "Combo.h"
#include "SafeDelete.h"
#include "Easing.h"

Combo* Combo::Create(int numberTexNum, int comboTexNum)
{
	//�C���X�^���X�𐶐�
	Combo* instance = new Combo();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(numberTexNum, comboTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Combo::~Combo()
{
	for (int i = 0; i < comboDigits; i++)
	{
		safe_delete(numberSprite[i]);
	}
	safe_delete(comboSprite);
}

bool Combo::Initialize(int numberTexNum, int comboTexNum)
{
	//�R���{�̌�������
	for (int i = 0; i < comboDigits; i++)
	{
		//�R���{���\���p�X�v���C�g����
		numberSprite[i] = Sprite::Create(numberTexNum);
		if (numberSprite[i] == nullptr) {
			return false;
		}

		//�傫�����Z�b�g
		XMFLOAT2 size = { 32, 64 };
		numberSprite[i]->SetSize(size);

		//�e�N�X�`���T�C�Y���Z�b�g
		XMFLOAT2 texSize = { 48, 64 };
		numberSprite[i]->SetTexSize(texSize);

		//���W���Z�b�g
		XMFLOAT2 pos = { 777, 55 };
		pos.x -= size.x * i;
		numberSprite[i]->SetPosition(pos);
	}

	//COMBO�X�v���C�g����
	comboSprite = Sprite::Create(comboTexNum);
	if (comboSprite == nullptr) {
		return false;
	}
	//�傫�����Z�b�g
	XMFLOAT2 size = { 164, 38 };
	comboSprite->SetSize(size);
	//�e�N�X�`���T�C�Y���Z�b�g
	XMFLOAT2 texSize = { 273, 63 };
	comboSprite->SetTexSize(texSize);
	//���W���Z�b�g
	XMFLOAT2 pos = { 880, 58 };
	comboSprite->SetPosition(pos);


	//�R���{����0�Ő���
	combo = 0;
	//�R���{�I���^�C�}�[������
	lostComboTimer = 0;

	return true;
}

void Combo::Update()
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

	//�R���{����1�ȏ�̂Ƃ��ɃR���{�I���^�C�}�[���X�V
	if (combo >= 1)
	{
		LostTimerUpdate();
	}

	//�X�v���C�g�X�V
	for (int i = 0; i < comboDigits; i++)
	{
		numberSprite[i]->Update();
	}
	comboSprite->Update();
}

void Combo::Draw()
{
	//�X�v���C�g�`��
	if (combo >= 100)
	{
		numberSprite[2]->Draw();
	}
	if (combo >= 10)
	{
		numberSprite[1]->Draw();
	}
	if (combo >= 1)
	{
		numberSprite[0]->Draw();
		comboSprite->Draw();
	}
}

void Combo::Reset()
{
	//�R���{��
	combo = 0;
	//�ő�R���{��
	maxCombo = 0;
	//�R���{�I���^�C�}�[
	lostComboTimer = 0;
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
	//�\���p�X�v���C�g��0�ɖ߂�
	ChangeComboSprite();
	//�X�v���C�g�X�V
	for (int i = 0; i < comboDigits; i++)
	{
		numberSprite[i]->Update();
	}
}

void Combo::AddCombo()
{
	//�R���{���𑝂₷
	combo++;
	const int comboMax = 999;

	//�R���{�͍ő�l���z���Ȃ�
	if (combo >= comboMax)
	{
		combo = comboMax;
	}

	//�X�V�����R���{�����ő�R���{�����z���Ă�����ő�R���{�����X�V����
	if (maxCombo < combo)
	{
		maxCombo = combo;
	}

	//�R���{���X�v���C�g�̐����X�V
	ChangeComboSprite();

	//�X�v���C�g�̓����x��1�ɂ���
	XMFLOAT4 color = comboSprite->GetColor();
	color.w = 1;
	for (int i = 0; i < comboDigits; i++)
	{
		numberSprite[i]->SetColor(color);
	}
	comboSprite->SetColor(color);

	//�R���{�I���^�C�}�[������
	lostComboTimer = 0;
}

void Combo::LostCombo()
{
	//�R���{����0�ɂ���
	combo = 0;

	//�X�v���C�g�̓����x��MAX�ɂ���
	XMFLOAT4 color = comboSprite->GetColor();
	color.w = 0;
	for (int i = 0; i < comboDigits; i++)
	{
		numberSprite[i]->SetColor(color);
	}
	comboSprite->SetColor(color);
}

void Combo::SetMoveGamePos()
{
	//�Q�[���V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveGamePosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveGamePos = true;
}

void Combo::SetMoveResultPos()
{
	//���U���g�V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[��������
	moveResultPosTimer = 0;

	//�ړ���ԂɃZ�b�g
	isMoveResultPos = true;
}

void Combo::LostTimerUpdate()
{
	//�R���{�I������
	const int lostComboTime = 300;

	//�R���{�I���^�C�}�[�X�V
	lostComboTimer++;

	//�����ȏ�^�C�}�[���i�񂾂�F�𔖂�����
	if (lostComboTimer >= lostComboTime / 2)
	{
		//�F�𔖂����鎞��
		const int colorChangeTime = lostComboTime / 2;

		//�F�𔖂�����^�C�}�[
		const int colorChangeTimer = lostComboTimer - lostComboTime / 2;;

		//�C�[�W���O�v�Z�p�̎���
		float easeTimer = (float)colorChangeTimer / colorChangeTime;

		//�X�v���C�g�̓����x�����񂾂񔖂����Ă���
		XMFLOAT4 color = comboSprite->GetColor();
		color.w = Easing::Lerp(1.0f, 0.1f, easeTimer);
		for (int i = 0; i < comboDigits; i++)
		{
			numberSprite[i]->SetColor(color);
		}
		comboSprite->SetColor(color);
	}

	//�R���{�I���^�C�}�[���w�莞�Ԃɂ�������
	if (lostComboTimer >= lostComboTime)
	{
		LostCombo();
	}
}

void Combo::ChangeComboSprite()
{
	//3���̐��������ꂼ��o�͂���
	int digit[comboDigits];
	digit[0] = combo % 10;			//001
	digit[1] = (combo / 10) % 10;	//010
	digit[2] = (combo / 100) % 10;	//100

	//���ꂼ��̌��̐������X�v���C�g�̃e�N�X�`���؂�o�������炷
	for (int i = 0; i < comboDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = numberSprite[i]->GetTexSize().x * digit[i];
		numberSprite[i]->SetTexLeftTop(leftTop);
	}
}

void Combo::MoveGamePos()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveGamePosTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveGamePosTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	for (int i = 0; i < comboDigits; i++)
	{
		XMFLOAT2 numberPos = numberSprite[i]->GetPosition();
		numberPos.y = Easing::OutQuint(-100, 55, easeTimer);
		numberSprite[i]->SetPosition(numberPos);
	}
	XMFLOAT2 comboPos = comboSprite->GetPosition();
	comboPos.y = Easing::OutQuint(-100, 58, easeTimer);
	comboSprite->SetPosition(comboPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveGamePosTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveGamePos = false;

		//�ړ�����
		isMoveGamePosEnd = true;
	}
}

void Combo::MoveResultPos()
{
	//�ړ����s������
	const int moveTime = 60;

	//�^�C�}�[���X�V
	moveResultPosTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)moveResultPosTimer / moveTime;

	//�X�v���C�g�̍��W��ύX
	for (int i = 0; i < comboDigits; i++)
	{
		XMFLOAT2 numberPos = numberSprite[i]->GetPosition();
		numberPos.y = Easing::OutQuint(55, -100, easeTimer);
		numberSprite[i]->SetPosition(numberPos);
	}
	XMFLOAT2 comboPos = comboSprite->GetPosition();
	comboPos.y = Easing::OutQuint(58, -100, easeTimer);
	comboSprite->SetPosition(comboPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveResultPosTimer >= moveTime)
	{
		//�ړ���ԏI��
		isMoveResultPos = false;

		//�ړ�����
		isMoveResultPosEnd = true;
	}
}
