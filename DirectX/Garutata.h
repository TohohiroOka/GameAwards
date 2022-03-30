#pragma once
#include "GaruEnemy.h"

class Garutata : public GaruEnemy
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
	/// �K���^�^����
	/// </summary>
	/// <param name="garutataModel">���f��</param>
	/// <param name="stayPointModel">��~���W���f��</param>
	/// <param name="spawnPosition">�X�|�[�������W</param>
	/// <param name="stayPosition">��~���W</param>
	/// <returns>�K���^�^</returns>
	static Garutata *Create(Model *garutataModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

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