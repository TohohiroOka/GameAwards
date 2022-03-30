#pragma once
#include "HageEnemy.h"

class Hagetata : public HageEnemy
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
	/// �n�Q�^�^����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <param name="targetPosition">�W�I�̍��W</param>
	/// <returns>�n�Q�^�^</returns>
	static Hagetata *Create(Model *model, XMFLOAT3 position, XMFLOAT3 targetPosition);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <returns>����</returns>
	bool Initialize(Model *model, XMFLOAT3 position) override;

	/// <summary>
	/// �Ǐ]�p�x��ݒ�
	/// </summary>
	/// <param name="targetPosition">�W�I�̍��W</param>
	void SetMoveAngle(XMFLOAT3 targetPosition);
};