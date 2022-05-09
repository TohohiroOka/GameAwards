#pragma once
#include "Object3d.h"

class WallObject : public Object3d
{
private:

	struct EasingNum {
		XMFLOAT3 start = {};
		XMFLOAT3 end = {};
	};

public:

	enum class STATE {
		NONE,//��\��
		WAIT,//�ҋ@���
		DIRECTING_LEFT_UP,//����̒�ʒu�ړ�
		DIRECTING_RIGHT_UP,//�E��̒�ʒu�ړ�
		DIRECTING_RIGHT_DOWN,//�E���̒�ʒu�ړ�
		DIRECTING_LEFT_DOWN,//�����̒�ʒu�ړ�
		MOVE_UP,//��ǎ��̈ړ�
		MOVE_RIGHT,//�E�ǎ��̈ړ�
		MOVE_DOWN,//���ǎ��̈ړ�
		MOVE_LEFT,//���ǎ��̈ړ�
		DISPERSE,//���ݒn���班���΂炯������
		HIT,//������΂��ꂽ�K���N�^�Ɠ���������
	};

public:

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static WallObject* Create(Model* model);

private:

	/// <summary>
	/// ��ʒu�܂ł̈ړ�
	/// </summary>
	void Directing();

	/// <summary>
	/// �ǈړ�
	/// </summary>
	void WallMove();

public:

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize() override;

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

private://�Œ�l

	//���o�J�n���ʒu
	static const XMFLOAT3 startPosition;
	//�ő厞��
	static const float directingMaxTime;
	//��ʒ[�̍ŏ�
	static const XMFLOAT2 minPosition;
	//��ʒ[�̍ő�
	static const XMFLOAT2 maxPosition;
	//��ʒu�ւ̈ړ����̉�]��
	static const float initDistance;
	//����΂ߎ��Ԃ̍ő�
	static const float disperseMaxTime;

private:

	//�I�u�W�F�N�g�̌��݂̏��
	STATE state = STATE::NONE;
	//�I�u�W�F�N�g�̑O�̏��
	STATE oldState = STATE::NONE;
	//�X�^�[�g���̉��o����
	float time = 0;
	//��]�̉��o���Ɏg�p����p�x
	int angle = 0;
	//��]���̕�
	float distance = 0;
	//�C�[�W���O�Ŏg�����W
	EasingNum easingPos;
	//�C�[�W���O�Ŏg����]
	EasingNum easingRota;
	//����΂ߎ��̈ړ�����
	XMFLOAT3 disperseMovePos = {};
	//����΂ߎ��̉�]�p
	XMFLOAT3 disperseMoveRota = {};

public:

	/// <summary>
	/// ���o�J�n
	/// </summary>
	void SetState(STATE state) { this->state = state; }

	/// <summary>
	/// lerp�̏I�_�Z�b�g
	/// </summary>
	void SetLerpEndPosition(XMFLOAT3 EndPos) { this->easingPos.end = EndPos; }

	/// <summary>
	/// ����΂ߎ��̈ړ������Z�b�g
	/// </summary>
	void SetDisperseMovePos(XMFLOAT3 disperseMovePos) { this->disperseMovePos = disperseMovePos; }

	/// <summary>
	/// ����΂ߎ��̉�]�p�Z�b�g
	/// </summary>
	void SetDisperseMoveRota(XMFLOAT3 disperseMoveRota) { this->disperseMoveRota = disperseMoveRota; }

	/// <summary>
	/// ��ʒ[�擾
	/// </summary>
	float GetTime() { return time; }

	/// <summary>
	/// ��ʒ[�̍ŏ��擾
	/// </summary>
	static XMFLOAT2 GetWallMinPosition() { return minPosition; }

	/// <summary>
	/// ��ʒ[�̍ŏ��擾
	/// </summary>
	static XMFLOAT2 GetWallMaxPosition() { return maxPosition; }
};