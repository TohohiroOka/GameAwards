#pragma once
#include "Object3d.h"

class FixedObject
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
	/// �Œ�I�u�W�F�N�g����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <returns>�Œ�I�u�W�F�N�g</returns>
	static FixedObject *Create(Model *model, XMFLOAT3 position);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FixedObject();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <returns>����</returns>
	bool Initialize(Model *model, XMFLOAT3 position);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	XMFLOAT3 GetPosition() { return fixedObject->GetPosition(); }
	XMFLOAT3 GetScale() { return fixedObject->GetScale(); }

private:
	//�Œ�I�u�W�F�N�g
	Object3d *fixedObject = nullptr;
};