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
	/// ���Z�b�g
	/// </summary>
	virtual void Reset();

	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	virtual void Damage(int damagePower);

	/// <summary>
	/// ���S
	/// </summary>
	virtual void Dead();

	/// <summary>
	/// ���S���ăT�C�Y��ύX��Ԃ��Z�b�g
	/// </summary>
	void SetDeadChangeScale();

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
	bool GetIsDuringSpawn() { return isDuringSpawn; }
	bool GetIsBulletShot() { return isBulletShot; }
	bool GetIsExistence() { return isExistence; }

protected:
	/// <summary>
	/// �X�|�[��
	/// </summary>
	virtual void Spawn() = 0;

	/// <summary>
	/// ���S���ăT�C�Y��ύX
	/// </summary>
	void DeadChangeScale();

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
	//���S���ăT�C�Y��ύX��Ԃ�
	bool isDeadChangeScale = false;
	//�c���O�̃T�C�Y
	float changeStartScale = 0;
	//�c����̃T�C�Y
	float changeEndScale = 0;
	//�T�C�Y�ύX�V�[��
	int changeScaleScene = 0;
	//�T�C�Y�ύX�^�C�}�[
	int changeScaleTimer = 0;
	//���݂��Ă��邩(�T�C�Y��0�ɂȂ��Ă��犮�S����)
	bool isExistence = true;
};