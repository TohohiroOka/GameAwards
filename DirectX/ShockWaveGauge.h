#pragma once
#include"Sprite.h"

class ShockWaveGauge
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
	/// ����Ռ��g�p�Q�[�W����
	/// </summary>
	/// <returns>����Ռ��g�p�Q�[�W</returns>
	static ShockWaveGauge* Create(int frameTexNum, int barTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ShockWaveGauge();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int frameTexNum, int barTexNum);

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
	/// �Q�[�W�|�C���g����
	/// </summary>
	void IncreasePoint();

	/// <summary>
	/// �Q�[�W�|�C���g����
	/// </summary>
	void DecreasePoint();

	/// <summary>
	/// �Q�[���V�[���̍��W�Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveGamePos();

	/// <summary>
	/// ���U���g�V�[���̍��W�Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveResultPos();

	//getter
	int GetGaugeLevel() { return gaugeLevel; }
	bool GetIsMoveGamePosEnd() { return isMoveGamePosEnd; }

private:
	/// <summary>
	/// �o�[�̒�����ύX
	/// </summary>
	void ChangeLengthBar();

	/// <summary>
	/// �Q�[�W���x����ύX
	/// </summary>
	void ChangeGaugeLevel();

	/// <summary>
	/// �Q�[���V�[���̍��W�Ɉړ�
	/// </summary>
	void MoveGamePos();

	/// <summary>
	/// ���U���g�V�[���̍��W�Ɉړ�
	/// </summary>
	void MoveResultPos();

private:
	//�Q�[�W(�g)�X�v���C�g
	Sprite* frameSprite = nullptr;
	//�Q�[�W(�o�[)�X�v���C�g
	Sprite* barSprite = nullptr;
	//�ő�Q�[�W�|�C���g
	const int gaugePointMax = 1000;
	//�Q�[�W�|�C���g
	int gaugePoint = 0;
	//�Q�[�W���x��
	int gaugeLevel = 0;
	//�o�[�X�v���C�g�̒����ő�l
	const float lengthMax = 134;
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