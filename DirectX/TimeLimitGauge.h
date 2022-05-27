#pragma once
#include"Sprite.h"

class TimeLimitGauge
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
	/// �������ԉ񕜗p�Q�[�W����
	/// </summary>
	/// <returns>����Ռ��g�p�Q�[�W</returns>
	static TimeLimitGauge* Create(int timeTexNum, int frameTexNum, int barTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TimeLimitGauge();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int timeTexNum, int frameTexNum, int barTexNum);

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
	bool GetIsCountDownEnd() { return isCountDownEnd; }
	bool GetIsMoveGamePosEnd() { return isMoveGamePosEnd; }
	int GetTimer() { return timer; }

private:
	/// <summary>
	/// �J�E���g�_�E��
	/// </summary>
	void CountDown();

	/// <summary>
	/// �J�E���g�_�E���ɂ��o�[�̒����ύX
	/// </summary>
	void CountDownLengthBar();

	/// <summary>
	/// �񕜒��o�[�̒�����ύX
	/// </summary>
	void RecoveryLengthBar();

	/// <summary>
	/// �񕜒��o�[�̒����ύX���Z�b�g
	/// </summary>
	void SetRecoveryLengthBar();

	/// <summary>
	/// �Q�[���V�[���̍��W�Ɉړ�
	/// </summary>
	void MoveGamePos();

	/// <summary>
	/// ���U���g�V�[���̍��W�Ɉړ�
	/// </summary>
	void MoveResultPos();

private:
	//TIME�����X�v���C�g
	Sprite* timeSprite = nullptr;
	//�|�C���g�\��(�g)�X�v���C�g
	Sprite* frameSprite = nullptr;
	//�|�C���g�\��(�o�[)�X�v���C�g
	Sprite* barSprite = nullptr;
	//�������ԍő�
	const int timeLimitMax = 3600;
	//���Ԍv���^�C�}�[
	int timer = timeLimitMax;
	//�J�E���g�_�E�����邩
	bool isCountDown = false;
	//�񕜒���
	bool isRecovery = false;
	//�J�E���g�_�E�����Ō�܂ōs������
	bool isCountDownEnd = false;


	//�o�[�̒����ő�l
	const float lengthMax = 896;
	//�񕜒��o�[�̒����ύX�^�C�}�[
	int recoveryLengthTimer = 0;
	//�񕜒��o�[�̒����ύX�O�̒���
	float recoveryLengthBefore = 0;
	//�񕜒��o�[�̒����ύX��̒���
	float recoveryLengthAfter = 0;

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