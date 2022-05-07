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
	static TimeLimitGauge* Create(int frameTexNum, int barTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TimeLimitGauge();

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
	/// �|�C���g�𑝂₷
	/// </summary>
	/// <param name="point">������</param>
	void AddPoint(int point);

	/// <summary>
	/// �|�C���g���g��
	/// </summary>
	void UsePoint();

	/// <summary>
	/// �Q�[���V�[���̍��W�Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveGamePos();

	/// <summary>
	/// ���U���g�V�[���̍��W�Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveResultPos();

	//getter
	bool GetIsGaugeMax() { return isGaugeMax; }
	bool GetIsMoveGamePosEnd() { return isMoveGamePosEnd; }

private:
	/// <summary>
	/// �o�[�̒�����ύX
	/// </summary>
	void ChangeLengthBar();

	/// <summary>
	/// �o�[�̒����ύX���Z�b�g
	/// </summary>
	void SetChangeLength();

	/// <summary>
	/// �Q�[���V�[���̍��W�Ɉړ�
	/// </summary>
	void MoveGamePos();

	/// <summary>
	/// ���U���g�V�[���̍��W�Ɉړ�
	/// </summary>
	void MoveResultPos();

private:
	//�|�C���g�\��(�g)�X�v���C�g
	Sprite* frameSprite = nullptr;
	//�|�C���g�\��(�o�[)�X�v���C�g
	Sprite* barSprite = nullptr;
	//�񕜊J�n�|�C���g
	const int recoveryPointMax = 100;
	//�񕜃|�C���g
	int recoveryPoint = 0;
	//�o�[�X�v���C�g�̒�����ύX���邩
	bool isChangeLengthBar = false;
	//�o�[�X�v���C�g�̒����ő�l
	const float lengthMax = 100;
	//�o�[�X�v���C�g�̒����ύX�^�C�}�[
	int changeLengthTimer = 0;
	//�o�[�X�v���C�g�̒����ύX�O�̒���
	float changeLengthBefore = 0;
	//�o�[�X�v���C�g�̒����ύX��̒���
	float changeLengthAftar = 0;
	//�Q�[�W���ő傩
	bool isGaugeMax = false;
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