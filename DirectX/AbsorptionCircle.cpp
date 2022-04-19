#include "AbsorptionCircle.h"
#include "SafeDelete.h"
#include "Easing.h"

AbsorptionCircle* AbsorptionCircle::Create(Model* model, XMFLOAT3 position)
{
	//�C���X�^���X�𐶐�
	AbsorptionCircle* instance = new AbsorptionCircle();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	return instance;
}

AbsorptionCircle::~AbsorptionCircle()
{
	safe_delete(circleObject);
}

bool AbsorptionCircle::Initialize(Model* model, XMFLOAT3 position)
{
	//�~�I�u�W�F�N�g����
	circleObject = Object3d::Create();
	if (circleObject == nullptr) {
		return false;
	}

	//���W���Z�b�g
	circleObject->SetPosition(position);

	if (model) {
		circleObject->SetModel(model);
	}


	//�F��ɂ���
	circleObject->SetColor({ 0.2f, 0.2f, 0.2f, 0.4f });

	return true;
}

void AbsorptionCircle::Update()
{
	//�~���L����
	if (action == ActionNumber::Spread)
	{
		SpreadCircle();
	}
	//�ꎞ��~
	else if (action == ActionNumber::Stay)
	{
		StayCircle();
	}
	//�~���k�߂�
	else if (action == ActionNumber::Shrink)
	{
		ShrinkCircle();
	}

	//�I�u�W�F�N�g�X�V
	circleObject->Update();
}

void AbsorptionCircle::Draw()
{
	//�I�u�W�F�N�g�`��
	circleObject->Draw();
}

void AbsorptionCircle::SetDelete()
{
	//�폜����
	isDelete = true;
}

void AbsorptionCircle::SpreadCircle()
{
	//�g����s������
	const int spreadTime = 30;

	//�~�̃T�C�Y���L����^�C�}�[�X�V
	spreadTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)spreadTimer / spreadTime;

	//�C�[�W���O�ŃT�C�Y�ύX
	float newRadius = Easing::OutCubic(0.0f, 20.0f, easeTimer);

	//�X�V�����T�C�Y���Z�b�g
	circleObject->SetScale({ newRadius, newRadius, 1 });

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (spreadTimer >= spreadTime)
	{
		//�~���ꎞ��~������
		action = ActionNumber::Stay;
	}
}

void AbsorptionCircle::StayCircle()
{
	//�ꎞ��~���s������
	const int stayTime = 120;

	//�ꎞ��~�^�C�}�[�X�V
	stayTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (stayTimer >= stayTime)
	{
		//�~���k�߂�
		action = ActionNumber::Shrink;
	}
}

void AbsorptionCircle::ShrinkCircle()
{
	//�k�����s������
	const int shrinkTime = 30;

	//�~�̃T�C�Y���k�߂�^�C�}�[�X�V
	shrinkTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)shrinkTimer / shrinkTime;

	//�C�[�W���O�ŃT�C�Y�ύX
	float newRadius = Easing::OutCubic(20.0f, 0.0f, easeTimer);

	//�X�V�����T�C�Y���Z�b�g
	circleObject->SetScale({ newRadius, newRadius, 1 });

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (shrinkTimer >= shrinkTime)
	{
		//�폜����
		SetDelete();
	}
}
