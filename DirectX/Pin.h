#pragma once
#include "Object3d.h"

class Pin
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
	/// �s������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�s��</returns>
	static Pin* Create(Model* model, XMFLOAT3 position);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Pin();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>����</returns>
	bool Initialize(Model* model, XMFLOAT3 position);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	void Damage(int damagePower);

	/// <summary>
	/// ���S
	/// </summary>
	void Dead();

	/// <summary>
	/// �폜
	/// </summary>
	void SetDelete();

	/// <summary>
	/// ���f�����Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(Model* model) { pinObject->SetModel(model); }

	//getter
	XMFLOAT3 GetPosition() { return pinObject->GetPosition(); }
	XMFLOAT3 GetScale() { return pinObject->GetScale(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsDelete() { return isDelete; }
	bool GetIsDamege() { return isDamage; }

protected:
	//�s���I�u�W�F�N�g
	Object3d* pinObject = nullptr;
	//�̗�
	int HP = 40;
	//�����Ă��邩
	bool isAlive = true;
	//�폜���邩
	bool isDelete = false;
	//�_���[�W�������Ă��邩
	bool isDamage = false;
	//�_���[�W�������Ă���̎���
	int damageTimer = 0;
};