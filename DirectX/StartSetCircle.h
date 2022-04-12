#pragma once
#include "ConnectCircle.h"

class StartSetCircle : public ConnectCircle
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
	/// ���������Z�b�g�R�l�N�g�T�[�N������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="FixedEnemy">�Œ�G</param>
	/// <param name="radius">�Œ�G</param>
	/// <returns>�R�l�N�g�T�[�N��</returns>
	static StartSetCircle* Create(Model* model, FixedEnemy* enemy, float radius);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="FixedEnemy">�Œ�G</param>
	/// <returns>����</returns>
	bool Initialize(Model* model, FixedEnemy* enemy, float radius);
};