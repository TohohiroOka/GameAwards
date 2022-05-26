#pragma once
#include "Object3d.h"

class HealingZone
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
	/// �Q�[�W�񕜒n�_����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�Q�[�W�񕜒n�_</returns>
	static HealingZone* Create(Model* model);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~HealingZone();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
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

	//getter
	XMFLOAT3 GetPosition() { return healingZoneObject->GetPosition(); }
	float GetRadius() { return healingZoneObject->GetScale().x; }

private:
	//�񕜒n�_�I�u�W�F�N�g
	Object3d* healingZoneObject = nullptr;
};