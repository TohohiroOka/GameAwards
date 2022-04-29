#pragma once
#include "Object3d.h"

class GaruEnemy
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
	enum GaruGroup
	{
		Garuta,		//�K���^
		Garutata,	//�K���^�^
	};

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~GaruEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="enemyModel">���f��</param>
	/// <param name="stayPointModel">��~���W���f��</param>
	/// <param name="spawnPosition">�X�|�[�����̍��W</param>
	/// <param name="stayPosition">��~���W</param>
	/// <returns>����</returns>
	virtual bool Initialize(Model* enemyModel, Model* stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition) = 0;

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
	/// �m�b�N�o�b�N�̏����Z�b�g
	/// </summary>
	/// <param name="angle">������Ԋp�x</param>
	/// <param name="power">�m�b�N�o�b�N�̋���</param>
	void SetKnockBack(float angle, int power);

	/// <summary>
	/// ���f�����Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(Model* model) { enemyObject->SetModel(model); }

	/// <summary>
	/// �g�ɂԂ�����������
	/// </summary>
	/// <returns>�g�ɂԂ�������</returns>
	bool IsCollisionFrame(XMFLOAT2 frameLine);

	//getter
	int GetGroup() { return group; }
	XMFLOAT3 GetPosition() { return enemyObject->GetPosition(); }
	XMFLOAT3 GetScale() { return enemyObject->GetScale(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsEscape() { return isEscape; }
	bool GetIsDelete() { return isDelete; }
	bool GetIsDuringSpawn() { return isDuringSpawn; }
	int GetKnockBackPower() { return knockBackPower; }
	bool GetIsBulletShot() { return isBulletShot; }

protected:
	/// <summary>
	/// �X�|�[��
	/// </summary>
	void Spawn();

	/// <summary>
	/// �e�𔭎�
	/// </summary>
	void ShotBullet();

	/// <summary>
	/// �m�b�N�o�b�N
	/// </summary>
	void KnockBack();

	/// <summary>
	/// ����
	/// </summary>
	void Escape();

protected:
	//�g�̃��C��
	static XMFLOAT2 frameLine;

protected:
	//�ǂ���ɑ����Ă��邩
	int group;
	//�G�I�u�W�F�N�g
	Object3d* enemyObject = nullptr;
	//��~���W�I�u�W�F�N�g
	Object3d* stayPointObject = nullptr;
	//�X�|�[�������W
	XMFLOAT3 spawnPosition = { 0, 0, 0 };
	//�ړ���̍��W
	XMFLOAT3 stayPosition = { 0, 0, 0 };
	//�̗�
	int HP = 40;
	//�����Ă��邩
	bool isAlive = true;
	//���������ԃ^�C�}�[
	int aliveTimer = 0;
	//��莞�ԕ��u���ꓦ�����邩
	bool isEscape = false;
	//�������Ԍv���p�^�C�}�[
	int escapeTimer = 0;
	//�m�b�N�o�b�N����
	int effectCount = 0;
	//�X�|�[������
	bool isDuringSpawn = true;
	//�X�|�[���^�C�}�[
	int spawnTimer = 0;
	//�m�b�N�o�b�N�^�C�}�[
	int knockBackTimer = 0;
	//�m�b�N�o�b�N�̊p�x
	float knockBackAngle = 0.0f;
	//�m�b�N�o�b�N�̋���
	int knockBackPower = 0;
	//�m�b�N�o�b�N������
	int knockBackCount = 0;
	//�m�b�N�o�b�N���Ă��邩
	bool isKnockBack = false;
	//�e�𔭎˂��邩
	bool isBulletShot = false;
	//�e���˂���̎���
	int bulletShotTimer = 0;
	//�폜���邩
	bool isDelete = false;
};