#pragma once
#include "BaseEnemy.h"

class Garuta : public BaseEnemy
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
	/// �K���^����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <returns>�K���^</returns>
	static Garuta *Create(Model *model, XMFLOAT3 position);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <returns>����</returns>
	bool Initialize(Model *model, XMFLOAT3 position) override;
};