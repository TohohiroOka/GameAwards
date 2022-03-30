#pragma once
#include "Object3d.h"

class HageEnemy
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
	/// �f�X�g���N�^
	/// </summary>
	virtual ~HageEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <returns>����</returns>
	virtual bool Initialize(Model *model, XMFLOAT3 position) = 0;

	/// <summary>
	/// ���t���[������
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

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
	void SetModel(Model *model) { enemyObject->SetModel(model); }

	//getter
	XMFLOAT3 GetPosition() { return enemyObject->GetPosition(); }
	XMFLOAT3 GetScale() { return enemyObject->GetScale(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsInScreen() { return isInScreen; }
	bool GetIsDelete() { return isDelete; }
	bool GetIsBulletShot() { return isBulletShot; }

protected:
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// �e�𔭎�
	/// </summary>
	void ShotBullet();

protected:
	//�G�I�u�W�F�N�g
	Object3d *enemyObject = nullptr;
	//�ړ��p�x
	float moveAngle = 0.0f;
	//�̗�
	int HP = 20;
	//�����Ă��邩
	bool isAlive = true;
	//�e�𔭎˂��邩
	bool isBulletShot = false;
	//��ʓ��ɂ��邩
	bool isInScreen = true;
	//�e���˂���̎���
	int bulletShotTimer = 0;
	//�폜���邩
	bool isDelete = false;
};

