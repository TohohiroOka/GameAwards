#pragma once
#include "Object3d.h"

class LaserSite
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
	/// ���[�U�[�T�C�g����
	/// </summary>
	/// <returns>���[�U�[�T�C�g</returns>
	static LaserSite* Create(Model* model);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~LaserSite();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���W�X�V
	/// </summary>
	/// <param name="weaponPosition">�E�G�|���̍��W</param>
	/// <param name="weaponRotation">�E�G�|���̊p�x</param>
	void SetPosition(XMFLOAT3 weaponPosition, XMFLOAT3 weaponRotation);

private:
	//���[�U�[�I�u�W�F�N�g
	Object3d* razorObject = nullptr;
};