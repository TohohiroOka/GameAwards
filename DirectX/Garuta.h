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
	/// <param name="garutaModel">���f��</param>
	/// <param name="stayPointModel">��~���W���f��</param>
	/// <param name="spawnPosition">�X�|�[�������W</param>
	/// <param name="stayPosition">��~���W</param>
	/// <returns>�K���^</returns>
	static Garuta *Create(Model *garutaModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="enemyModel">���f��</param>
	/// <param name="stayPointModel">��~���W���f��</param>
	/// <param name="spawnPosition">�X�|�[�������W</param>
	/// <param name="stayPosition">��~���W</param>
	/// <returns>����</returns>
	bool Initialize(Model *enemyModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition) override;
};