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
	static ShockWaveGauge* Create(int gaugeTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ShockWaveGauge();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int gaugeTexNum);

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
	/// �Q�[�W���Z�b�g���Z�b�g
	/// </summary>
	void SetGaugeReset();

	/// <summary>
	/// �Q�[�W�|�C���g����
	/// </summary>
	void IncreasePoint();

	/// <summary>
	/// �Q�[�W�|�C���g����
	/// </summary>
	void DecreasePoint();

	//setter
	void SetIsUpdate(bool isUpdate) { this->isUpdate = isUpdate; }

	//getter
	int GetGaugeLevel() { return gaugeLevel; }

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
	/// �Q�[�W���Z�b�g
	/// </summary>
	void GaugeReset();

private:
	//�Q�[�W�X�v���C�g
	Sprite* gaugeSprite = nullptr;
	//�ő�Q�[�W�|�C���g
	const int gaugePointMax = 1000;
	//�Q�[�W�|�C���g
	int gaugePoint = 0;
	//�Q�[�W���x��
	int gaugeLevel = 0;
	//�Q�[�W�X�v���C�g�̒����ő�l
	const float lengthMax = 192;
	//�X�V���邩
	bool isUpdate = false;
	//�Q�[�W���Z�b�g���邩
	bool isGaugeReset = false;
	//�Q�[�W���Z�b�g���鎞�ԃ^�C�}�[
	int gaugeResetTimer = 0;
	//�Q�[�W���Z�b�g����O�̃Q�[�W�̒���
	XMFLOAT2 resetGaugeBeforeLength = {};
};