#pragma once
#include "Object3d.h"

class BaseEnemy
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	enum EnemyGroup
	{
		Straighter,	//���i
		Division,	//����
		Releaser,	//���o
		Chaser		//�ǐ�
	};

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BaseEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="enemyModel">���f��</param>
	/// <param name="spawnPosition">�X�|�[�����̍��W</param>
	/// <returns>����</returns>
	virtual bool Initialize(Model* enemyModel, XMFLOAT3 spawnPosition, float moveDegree) = 0;

	/// <summary>
	/// ���t���[������
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���S
	/// </summary>
	void Dead();

	/// <summary>
	/// �폜
	/// </summary>
	void SetDelete();

	/// <summary>
	/// �m�b�N�o�b�N�̏����Z�b�g
	/// </summary>
	/// <param name="angle">������Ԋp�x</param>
	/// <param name="power">�m�b�N�o�b�N�̋���</param>
	virtual void SetKnockBack(float angle, int powerLevel);

	/// <summary>
	/// ���f�����Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(Model* model) { enemyObject->SetModel(model); }

	/// <summary>
	///	�ǂɂԂ�����������
	/// </summary>
	/// <returns>�ǂɂԂ�������</returns>
	bool IsCollisionWall(XMFLOAT2 wallLine);

	//getter
	int GetGroup() { return group; }
	XMFLOAT3 GetPosition() { return enemyObject->GetPosition(); }
	XMFLOAT3 GetScale() { return enemyObject->GetScale(); }
	int GetPower() { return power; }
	float GetMoveDegree() { return moveDegree; }
	int GetKnockBackPowerLevel() { return knockBackPowerLevel; }
	bool GetIsKnockBack() { return isKnockBack; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsCreateEnemy() { return isCreateEnemy; }
	bool GetIsDelete() { return isDelete; }

protected:
	/// <summary>
	/// �ړ�����
	/// </summary>
	virtual void Move() = 0;

	/// <summary>
	/// �ړ��p�x��ݒ�
	/// </summary>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	void SetMoveAngle(float moveDegree);

	/// <summary>
	/// �m�b�N�o�b�N
	/// </summary>
	virtual void KnockBack();

protected:
	//�g�̃��C��
	static XMFLOAT2 wallLine;

protected:
	//�����O���[�v
	int group;
	//�G�I�u�W�F�N�g
	Object3d* enemyObject = nullptr;
	//�U����
	int power = 0;
	//�����Ă��邩
	bool isAlive = true;
	//�ړ��p�x
	float moveDegree = 0;
	//�ړ��p�x
	float moveAngle = 0.0f;
	//�ړ����x
	float moveSpeed = 0.5f;
	//�ړ���
	XMFLOAT2 vel = {};
	//��ʓ��ɂ��邩
	bool isInScreen = false;
	//�m�b�N�o�b�N�^�C�}�[
	int knockBackTimer = 0;
	//�m�b�N�o�b�N�̊p�x
	float knockBackAngle = 0.0f;
	//�m�b�N�o�b�N�̋���
	int knockBackPowerLevel = 0;
	//�m�b�N�o�b�N���Ă��邩
	bool isKnockBack = false;
	//�V���ɓG�𐶐����邩
	bool isCreateEnemy = false;
	//�G�t�F�N�g
	int effectCount = 0;
	//�폜���邩
	bool isDelete = false;
};