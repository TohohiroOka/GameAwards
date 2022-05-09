#pragma once
#include "Object3d.h"

class PlayerBullet
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
	/// �v���C���[�e����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�v���C���[�e</returns>
	static PlayerBullet* Create(Model* model = nullptr);

	/// <summary>
	/// �폜������W���Z�b�g
	/// </summary>
	/// <param name="deadPos">�폜������W</param>
	static void SetDeadPos(XMFLOAT2 deadPosMin, XMFLOAT2 deadPosMax) {
		PlayerBullet::deadPosMin = deadPosMin;
		PlayerBullet::deadPosMax = deadPosMax;
	}

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlayerBullet();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>����</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

	/// <summary>
	/// �e����
	/// </summary>
	/// <param name="position">�J�n���W</param>
	/// <param name="rotation">�e�̊p�x</param>
	void BulletStart(XMFLOAT3 position, float angle);

	/// <summary>
	/// �e���S
	/// </summary>
	void Dead();

	//getter
	XMFLOAT3 GetPosition() { return bulletObject->GetPosition(); }
	float GetAngle() { return angle; }
	bool GetIsAlive() { return isAlive; };
	bool GetIsShockWaveStart() { return isShockWaveStart; }

private:
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// �������ԍX�V
	/// </summary>
	void AliveTimeUpdate();

private:
	//�폜������W
	static XMFLOAT2 deadPosMin;
	static XMFLOAT2 deadPosMax;

private:
	//�e�I�u�W�F�N�g
	Object3d* bulletObject = nullptr;
	//���ˊp�x
	float angle = 0.0f;
	//�e�̐������ԃ^�C�}�[
	int aliveTimer = 0;
	//�e�������Ă��邩
	bool isAlive = false;
	//�Ռ��g�𔭎˂��邩
	bool isShockWaveStart = false;
};
