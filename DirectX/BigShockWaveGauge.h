#pragma once
#include"Sprite.h"

class BigShockWaveGauge
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
	static BigShockWaveGauge* Create(int frameTexNum, int barTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BigShockWaveGauge();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int frameTexNum, int barTexNum);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(int combo);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

private:
	/// <summary>
	/// �o�[�̒�����ύX
	/// </summary>
	void ChangeLengthBar();

	/// <summary>
	/// �o�[�̒����ύX���Z�b�g
	/// </summary>
	void SetChangeLength();

private:
	//�|�C���g�\��(�g)�X�v���C�g
	Sprite* frameSprite = nullptr;
	//�|�C���g�\��(�o�[)�X�v���C�g
	Sprite* barSprite = nullptr;
	//�R���{��
	int combo = 0;

	//�o�[�X�v���C�g�̒�����ύX���邩
	bool isChangeLengthBar = false;
	//�o�[�X�v���C�g�̒����ő�l
	const float lengthMax = 556 / 4;
	//�o�[�X�v���C�g�̒����ύX�^�C�}�[
	int changeLengthTimer = 0;
	//�o�[�X�v���C�g�̒����ύX�O�̒���
	float changeLengthBefore = 0;
	//�o�[�X�v���C�g�̒����ύX��̒���
	float changeLengthAftar = 0;
};