#pragma once
#include "Object3d.h"

class ShockWave
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
	/// �Ռ��g����
	/// </summary>
	/// <param name="waveModel">���f��</param>
	/// <returns>�Ռ��g</returns>
	static ShockWave* Create(Model* waveModel);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ShockWave();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="waveModel">���f��</param>
	/// <returns>����</returns>
	bool Initialize(Model* waveModel);

	/// <summary>
	/// �X�V����
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
	/// �Ռ��g�J�n���Z�b�g
	/// </summary>
	void SetWaveStart(XMFLOAT3 startPos);

	//getter
	XMFLOAT3 GetPosition() { return position; }
	float GetRadius() { return radius; }

private:	
	/// <summary>
	/// �Ռ��g���L����
	/// </summary>
	void WaveSpread();

private:
	//�Ռ��g�I�u�W�F�N�g
	Object3d* shockWaveObject = nullptr;
	//���W
	XMFLOAT3 position = {};
	//�Ռ��g�~�̔��a
	float radius = 0;
	//�Ռ����L���鑬�x
	float spreadSpeed = 0;
	//�Ռ����L���鑬�x�̑��x
	float spreadSpeedAccle = 0;
	//�Ռ��g���L���邩
	bool isSpreadWave = false;
};