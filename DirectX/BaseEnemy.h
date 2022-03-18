#pragma once
#include "Object3d.h"

class BaseEnemy
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
	virtual ~BaseEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
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
	/// �m�b�N�o�b�N�̏����Z�b�g
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <param name="power">�m�b�N�o�b�N�̋���</param>
	void SetKnockBack(float angle, int power);

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
	bool GetIsExistence() { return isExistence; }
	bool GetIsInScreen() { return isInScreen; }
	int GetKillBulletPower() { return killBulletPower; }
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

	/// <summary>
	/// �m�b�N�o�b�N
	/// </summary>
	void KnockBack();

protected:
	//�G�X�v���C�g
	Object3d *enemyObject = nullptr;
	//�ړ��p�x
	float moveAngle = 0.0f;
	//�̗�
	int HP = 20;
	//�����Ă��邩
	bool isAlive = true;
	//���݂��Ă��邩(�m�b�N�o�b�N���I��)
	bool isExistence = true;
	//��ʓ��ɂ��邩
	bool isInScreen = true;
	//�m�b�N�o�b�N�^�C�}�[
	int knockBackTimer = 0;
	//�m�b�N�o�b�N�̊p�x
	float knockBackAngle = 0.0f;
	//�m�b�N�o�b�N�̋���
	int killBulletPower = 0;
	//�e�𔭎˂��邩
	bool isBulletShot = false;
	//�e���˂���̎���
	int bulletShotTimer = 0;
};