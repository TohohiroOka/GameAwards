#pragma once
#include "Object3d.h"
#include "Player.h"

class LandingPoint
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
	/// ���e�n�_����
	/// </summary>
	/// <returns>���e�n�_</returns>
	static LandingPoint* Create(Model* model);

	static void SetMoveRange(XMFLOAT2 moveRange) { LandingPoint::moveRange = moveRange; }

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~LandingPoint();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(XMFLOAT3 playerPosition, XMFLOAT3 playerRotation);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���W�X�V
	/// </summary>
	/// <param name="weaponPosition">�v���C���[�̍��W</param>
	/// <param name="weaponRotation">�v���C���[�̊p�x</param>
	void SetPosition(XMFLOAT3 position, XMFLOAT3 rotation);

private:
	static XMFLOAT2 moveRange;

private:
	//���e�n�_�I�u�W�F�N�g
	static const int pointNum = 3;
	Object3d* pointCircleObject[pointNum] = { nullptr };
};