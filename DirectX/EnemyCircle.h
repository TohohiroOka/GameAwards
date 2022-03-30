#pragma once
#include "ConnectCircle.h"

class EnemyCircle : public ConnectCircle
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
	/// �K��������o��R�l�N�g�T�[�N������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="enemy">�K�����̃C���X�^���X</param>
	/// <param name="radius">�~�̔��a</param>
	/// <returns>�R�l�N�g�T�[�N��</returns>
	static EnemyCircle *Create(Model *model, GaruEnemy *enemy, float radius);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�K�����̃C���X�^���X</param>
	/// <param name="radius">�~�̔��a</param>
	/// <returns>����</returns>
	bool Initialize(Model *model, GaruEnemy *enemy, float radius);
};