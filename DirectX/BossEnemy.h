#pragma once
#include "Object3d.h"

class BossEnemy
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
	enum BossName 
	{
		Tuff,	//�^�b�t
		Buff,
	};

public:
	/// <summary>
	/// �g�̃��C�����Z�b�g
	/// </summary>
	/// <param name="frameLine">�g�̃��C��</param>
	static void SetFrameLine(XMFLOAT2 frameLine) { BossEnemy::frameLine = frameLine; }

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BossEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>����</returns>
	virtual bool Initialize(Model* model);

	/// <summary>
	/// ���t���[������
	/// </summary>
	/// <param name="targetPosition">�W�I�̍��W</param>
	virtual void Update(XMFLOAT3 targetPosition) = 0;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	virtual void Damage(int damagePower);

	/// <summary>
	/// ���S
	/// </summary>
	virtual void Dead();

	/// <summary>
	/// �폜
	/// </summary>
	virtual void SetDelete();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	virtual void Reset();

	/// <summary>
	/// �����Ռ���^����u�Ԃ�
	/// </summary>
	bool TriggerImpact();

	//getter
	XMFLOAT3 GetPosition() { return bossObject->GetPosition(); }
	XMFLOAT3 GetScale() { return bossObject->GetScale(); }
	int GetHP() { return HP; }
	int GetAction() { return action; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsDelete() { return isDelete; }
	bool GetIsDuringSpawn() { return isDuringSpawn; }
	bool GetIsBulletShot() { return isBulletShot; }

protected:
	/// <summary>
	/// �X�|�[��
	/// </summary>
	virtual void Spawn() = 0;

protected:
	//�g�̃��C��
	static XMFLOAT2 frameLine;

protected:
	//���O
	int name;
	//�{�X�I�u�W�F�N�g
	Object3d* bossObject = nullptr;
	//�̗�
	int HP = 4000;
	//�s��
	int action = 0;
	//�s���V�[��
	int actionScene = 0;
	//���̍s���V�[���ɂ��邩
	bool isChangeActionScene = false;
	//�s���������Ԃ��v������^�C�}�[
	int actionTimer = 0;
	//�����Ă��邩
	bool isAlive = true;
	//�폜���邩
	bool isDelete = false;
	//�X�|�[������
	bool isDuringSpawn = true;
	//�X�|�[���^�C�}�[
	int spawnTimer = 0;
	//�e�𔭎˂��邩
	bool isBulletShot = false;
	//����e�𔭎˂�����
	int shotCount = 0;
	//�e���˂���̎���
	int bulletShotTimer = 0;
	//�����Ռ���^���邩
	bool isImpact = false;
};