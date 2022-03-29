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
	/// <param name="position">���W</param>
	/// <param name="radius">�~�̔��a</param>
	/// <returns>�R�l�N�g�T�[�N��</returns>
	static StartSetCircle *Create(Model *model, XMFLOAT3 position, float radius);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <param name="radius">�~�̔��a</param>
	/// <returns>����</returns>
	bool Initialize(Model *model, XMFLOAT3 position, float radius);
};