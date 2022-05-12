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
		TRANSPARENCY,//������
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

	/// <summary>
	/// ������
	/// </summary>
	void Transparency();

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

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

private://�Œ�l

	//���o�J�n���ʒu
	static const XMFLOAT3 startPosition;
	//�ő厞�ԃX�^�[�g��
	static float directingMaxTime;
	//��ʒ[�̍ŏ�
	static const XMFLOAT2 minPosition;
	//��ʒ[�̍ő�
	static const XMFLOAT2 maxPosition;
	//��ʒu�ւ̈ړ����̉�]��
	static const float initDistance;
	//����΂ߎ��Ԃ̍ő�
	static const float disperseMaxTime;
	//����������
	static const float transparentMaxTime;
	//�����t���O
	static bool isSlow;

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
	//������
	float slow = 0.0f;

public:

	/// <summary>
	/// ���o�Z�b�g
	/// </summary>
	void SetState(STATE state) { this->state = state; }

	/// <summary>
	/// ���o����
	/// </summary>
	static void SetEffectTime(float time) { directingMaxTime = time; }

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
	/// ���o�󋵎擾
	/// </summary>
	STATE GetState() { return state; }

	/// <summary>
	/// �����x�Z�b�g
	/// </summary>
	static void SetSlow(bool isSlow) { WallObject::isSlow = isSlow; }

	/// <summary>
	/// ��ʒ[�擾
	/// </summary>
	float GetTime() { return time; }

	/// <summary>
	/// ���o����
	/// </summary>
	static float GetEffectTime() { return directingMaxTime; }

	/// <summary>
	/// ��ʒ[�̍ŏ��擾
	/// </summary>
	static XMFLOAT2 GetWallMinPosition() { return minPosition; }

	/// <summary>
	/// ��ʒ[�̍ŏ��擾
	/// </summary>
	static XMFLOAT2 GetWallMaxPosition() { return maxPosition; }
};