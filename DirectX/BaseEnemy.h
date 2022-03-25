#pragma once
#include "Object3d.h"

class StageEffect;

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
	/// <param name="enemyModel">���f��</param>
	/// <param name="stayPointModel">��~���W���f��</param>
	/// <param name="spawnPosition">�X�|�[�����̍��W</param>
	/// <param name="stayPosition">��~���W</param>
	/// <returns>����</returns>
	virtual bool Initialize(Model *enemyModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition) = 0;

	/// <summary>
	/// ���t���[������
	/// </summary>
	/// <param name="effect">particle�N���X�̃C���X�^���X</param>
	virtual void Update(StageEffect* effect);

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
	bool GetIsEscape() { return isEscape; }
	bool GetIsEscapeCompleted() { return isEscapeCompleted; }
	bool GetIsExistence() { return isExistence; }
	bool GetIsDuringSpawn() { return isDuringSpawn; }
	int GetKillBulletPower() { return killBulletPower; }
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
	/// <param name="effect">particle�N���X�̃C���X�^���X</param
	void KnockBack(StageEffect* effect);

	/// <summary>
	/// ����
	/// </summary>
	void Escape();

protected:
	//�G�I�u�W�F�N�g
	Object3d *enemyObject = nullptr;
	//��~���W�I�u�W�F�N�g
	Object3d *stayPointObject = nullptr;
	//�X�|�[�������W
	XMFLOAT3 spawnPosition = { 0, 0, 0 };
	//�ړ���̍��W
	XMFLOAT3 stayPosition = { 0, 0, 0 };
	//�̗�
	int HP = 20;
	//�����Ă��邩
	bool isAlive = true;
	//���������ԃ^�C�}�[
	int aliveTimer = 0;
	//��莞�ԕ��u���ꓦ�����邩
	bool isEscape = false;
	//��������������
	bool isEscapeCompleted = false;
	//�������Ԍv���p�^�C�}�[
	int escapeTimer = 0;
	//���o���s���Ă��邩
	bool isEffect = false;
	//�m�b�N�o�b�N����
	int EffectCount = 0;
	//���݂��Ă��邩(�m�b�N�o�b�N���I��)
	bool isExistence = true;
	//�X�|�[������
	bool isDuringSpawn = true;
	//�X�|�[���^�C�}�[
	int spawnTimer = 0;
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