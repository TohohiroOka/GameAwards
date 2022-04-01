#pragma once
#include "Object3d.h"

class EnemyBullet
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
	/// �G�e����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�G�e</returns>
	static EnemyBullet *Create(Model *model = nullptr);

	/// <summary>
	/// �폜������W���Z�b�g
	/// </summary>
	/// <param name="deadPos">�폜������W</param>
	static void SetDeadPos(XMFLOAT2 deadPos) { EnemyBullet::deadPos = deadPos; }

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~EnemyBullet();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>����</returns>
	bool Initialize(Model *model);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �e����
	/// </summary>
	/// <param name="position">�J�n���W</param>
	/// <param name="targetPosition">�W�I�̍��W</param>
	void BulletStart(XMFLOAT3 position, XMFLOAT3 targetPosition);

	/// <summary>
	/// �e���S
	/// </summary>
	void Dead();

	//getter
	XMFLOAT3 GetPosition() { return bulletObject->GetPosition(); }
	XMFLOAT3 GetScale() { return bulletObject->GetScale(); }
	float GetAngle() { return angle; }
	bool GetIsAlive() { return isAlive; };

private:
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

private:
	//�폜������W
	static XMFLOAT2 deadPos;

private:
	//�e�I�u�W�F�N�g
	Object3d *bulletObject = nullptr;
	//���ˊp�x
	float angle = 0.0f;
	//�e�������Ă��邩
	bool isAlive = false;
};
