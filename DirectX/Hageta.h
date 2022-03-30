#pragma once
#include "HageEnemy.h"

class Hageta : public HageEnemy
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
	/// �n�Q�^����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	/// <returns>�n�Q�^</returns>
	static Hageta *Create(Model *model, XMFLOAT3 position, float moveDegree);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <returns>����</returns>
	bool Initialize(Model *model, XMFLOAT3 position) override;

	/// <summary>
	/// �ړ��p�x��ݒ�
	/// </summary>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	void SetMoveAngle(float moveDegree);
};