#pragma once
#include "Object3d.h"

class Charo
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
	/// �`��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <returns>�`����</returns>
	static Charo *Create(Model *model, XMFLOAT3 spawnPosition);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Charo();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <returns>����</returns>
	bool Initialize(Model *model, XMFLOAT3 spawnPosition);

	/// <summary>
	/// ���t���[������
	/// </summary>
	/// <param name="targetPosition">�i�s�����ɂȂ�W�I�̍��W</param>
	void Update(XMFLOAT3 targetPosition);

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

	//getter
	XMFLOAT3 GetPosition() { return enemyObject->GetPosition(); }
	XMFLOAT3 GetScale() { return enemyObject->GetScale(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsDelete() { return isDelete; }

protected:
	/// <summary>
	/// �^�[�Q�b�g�̌����ɐi�s�p�x���C��
	/// </summary>
	void SetMoveAngle(XMFLOAT3 targetPosition);

	/// <summary>
	/// �ړ����x�ύX
	/// </summary>
	void ChangeMoveSpeed();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

protected:
	//�G�I�u�W�F�N�g
	Object3d *enemyObject = nullptr;
	//�ړ����x
	float moveSpeed = 1.0f;
	//�ړ����x�̃^�C�}�[
	int moveSpeedTimer = 0;
	//�ړ��p�x
	float moveAngle = 0.0f;
	//�̗�
	int HP = 20;
	//�����Ă��邩
	bool isAlive = true;
	//�폜���邩
	bool isDelete = false;
};

