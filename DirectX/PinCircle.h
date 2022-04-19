#pragma once
#include "ConnectCircle.h"

class PinCircle : public ConnectCircle
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
	/// �s������o��R�l�N�g�T�[�N������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="garuEnemy">�s��</param>
	/// <returns>�R�l�N�g�T�[�N��</returns>
	static PinCircle* Create(Model* model, Pin* pin);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="pin">�s��</param>
	/// <returns>����</returns>
	bool Initialize(Model* model, Pin* pin);
};