#include "ConnectCircle.h"
#include "Easing.h"
#include "SafeDelete.h"

ConnectCircle::~ConnectCircle()
{
	safe_delete(circleObject);
}

void ConnectCircle::Update()
{
	//�T�C�Y�ύX���s������
	const int changeTime = 20;
	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)changeRadiusTimer / changeTime;

	//�C�[�W���O�ŃT�C�Y�ύX
	float newRadius = Easing::OutCubic(changeRadiusStart, changeRadiusEnd, easeTimer);

	//�T�C�Y���X�V
	radius = newRadius;
	//�X�V�����T�C�Y���Z�b�g
	circleObject->SetScale({ radius, radius, 1 });

	//�T�C�Y�ύX�^�C�}�[�X�V
	changeRadiusTimer++;
	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (changeRadiusTimer >= changeTime)
	{
		//�T�C�Y�ύX��ԏI��
		isChangeRadius = false;
	}

	//�I�u�W�F�N�g�X�V
	circleObject->Update();
}

void ConnectCircle::Draw()
{
	//�I�u�W�F�N�g�`��
	circleObject->Draw();
}

void ConnectCircle::BigRadius()
{
	//��������o�Ă�����̃J�E���g�𑝂₷
	countLine++;

	//�T�C�Y�ύX
	ChangeRadius();
}

void ConnectCircle::SmallRadius()
{
	//��������o�Ă�����̃J�E���g�����炷
	countLine--;

	//�T�C�Y�ύX
	ChangeRadius();
}

bool ConnectCircle::CheckUseEnemy(GaruEnemy *enemy)
{
	//�n�_���I�_�ň����̉~���g�p���Ă�����true��Ԃ�
	if (this->enemy == enemy)
	{
		return true;
	}

	//�g�p���Ă��Ȃ����false��Ԃ�
	return false;
}

void ConnectCircle::SetDelete()
{
	//�폜����
	isDelete = true;
}

void ConnectCircle::ChangeRadius()
{
	//�ύX�O�̉~�̃T�C�Y���Z�b�g
	changeRadiusStart = radius;
	//�ύX��̉~�̃T�C�Y���Z�b�g
	changeRadiusEnd = baseRadius + ((float)countLine / 3);
	//�T�C�Y�ύX�^�C�}�[��������
	changeRadiusTimer = 0;
	//�T�C�Y��ύX���ɂ���
	isChangeRadius = true;
}
