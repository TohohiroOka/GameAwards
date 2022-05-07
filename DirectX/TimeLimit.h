#pragma once
#include"Sprite.h"

class TimeLimit
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// �������Ԑ���
	/// </summary>
	/// <returns>��������</returns>
	static TimeLimit* Create(int textureNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TimeLimit();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int textureNum);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

	/// <summary>
	/// �������ԉ�
	/// </summary>
	/// <param name="second"></param>
	void Recovery(int second);

	/// <summary>
	/// �Q�[���V�[���̍��W�Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveGamePos();

	/// <summary>
	/// ���U���g�V�[���̍��W�Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveResultPos();

	//setter
	void SetIsCountDown(bool isCountDown) { this->isCountDown = isCountDown; }

	//getter
	int GetTime() { return timer; }
	bool GetIsMoveGamePosEnd() { return isMoveGamePosEnd; }

private:
	/// <summary>
	/// �J�E���g�_�E��
	/// </summary>
	void CountDown();

	/// <summary>
	/// �\���p�������ԃX�v���C�g�ύX
	/// </summary>
	void ChangeTimeSprite();

	/// <summary>
	/// �Q�[���V�[���̍��W�Ɉړ�
	/// </summary>
	void MoveGamePos();

	/// <summary>
	/// ���U���g�V�[���̍��W�Ɉړ�
	/// </summary>
	void MoveResultPos();

private:
	//�������ԃX�v���C�g
	static const int timeDigits = 2;
	Sprite* timeSprite[timeDigits] = { nullptr };
	//�������ԍő�
	const int timeLimitMax = 5999;
	//���Ԍv���^�C�}�[
	int timer = timeLimitMax;
	//�J�E���g�_�E�����邩
	bool isCountDown = false;
	//�Q�[���V�[���̍��W�Ɉړ�����
	bool isMoveGamePos = false;
	//�Q�[���V�[���̍��W�Ɉړ��I��������
	bool isMoveGamePosEnd = false;
	//�Q�[���V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[
	int moveGamePosTimer = 0;
	//���U���g�V�[���̍��W�Ɉړ�����
	bool isMoveResultPos = false;
	//���U���g�V�[���̍��W�Ɉړ��I��������
	bool isMoveResultPosEnd = false;
	//���U���g�V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[
	int moveResultPosTimer = 0;
};