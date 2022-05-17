#pragma once
#include "Object3d.h"

class WallObject : public Object3d
{
public:

	enum class STATE {
		NONE,//��\��
		WAIT,//�ҋ@���
		MOVE_UP_LEFT,//��ǎ��̍��ړ�
		MOVE_UP_RIGHT,//��ǎ��̉E�ړ�
		MOVE_RIGHT_DOWN,//�E�ǎ��̉��ړ�
		MOVE_RIGHT_UP,//�E�ǎ��̏�ړ�
		MOVE_DOWN_LEFT,//���ǎ��̍��ړ�
		MOVE_DOWN_RIGHT,//���ǎ��̉E�ړ�
		MOVE_LEFT_DOWN,//���ǎ��̉��ړ�
		MOVE_LEFT_UP,//���ǎ��̏�ړ�
		TRANSPARENCY,//������
		OUT_SCREEN,//��ʊO�ɍs��
	};

public:

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static WallObject* Create(Model* model);

private:

	/// <summary>
	/// �ǈړ�
	/// </summary>
	void WallMove();

	/// <summary>
	/// ������
	/// </summary>
	void Transparency();

	/// <summary>
	/// ��ʊO�ɍs������
	/// </summary>
	void OutScreen();

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

	//��ʒ[�̍ŏ�
	static const XMFLOAT2 minPosition;
	//��ʒ[�̍ő�
	static const XMFLOAT2 maxPosition;
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
	//������
	float slow = 0.0f;
	//�ړ����x
	XMFLOAT3 moveSpeed = {};

public:

	/// <summary>
	/// ���o�Z�b�g
	/// </summary>
	void SetState(STATE state) { this->state = state; }

	/// <summary>
	/// ���o�󋵎擾
	/// </summary>
	STATE GetState() { return state; }

	/// <summary>
	/// �����x�Z�b�g
	/// </summary>
	static void SetSlow(bool isSlow) { WallObject::isSlow = isSlow; }

	/// <summary>
	/// ��ʒ[�̍ŏ��擾
	/// </summary>
	static XMFLOAT2 GetWallMinPosition() { return minPosition; }

	/// <summary>
	/// ��ʒ[�̍ŏ��擾
	/// </summary>
	static XMFLOAT2 GetWallMaxPosition() { return maxPosition; }
};