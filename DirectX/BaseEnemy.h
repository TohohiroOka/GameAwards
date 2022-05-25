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
		None,		//���ݒ�
		Chaser,		//�ǐ�
		Division,	//����
		Releaser,	//���o
		TitleLogo	//�^�C�g�����S
	};

public:
	/// <summary>
	/// �ǂ̃��C�����Z�b�g
	/// </summary>
	/// <param name="wallLineMin">�ǂ̍ŏ����W</param>
	/// <param name="wallLineMax">�ǂ̍ő���W</param>
	static void SetWallLine(XMFLOAT2 wallLineMin, XMFLOAT2 wallLineMax) {
		BaseEnemy::wallLineMin = wallLineMin;
		BaseEnemy::wallLineMax = wallLineMax;
	}

	/// <summary>
	/// �^�[�Q�b�g���W���Z�b�g
	/// </summary>
	/// <param name="targetPos"></param>
	static void SetTargetPos(XMFLOAT3 targetPos) { BaseEnemy::targetPos = targetPos; };

	/// <summary>
	/// ���U���g�V�[���p�̓��������邩�Z�b�g
	/// </summary>
	/// <param name="isResultMove"></param>
	static void SetIsResultMove(bool isResultMove) { BaseEnemy::isResultMove = isResultMove; }

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BaseEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="spawnPosition">�X�|�[�����̍��W</param>
	/// <returns>����</returns>
	virtual bool Initialize(XMFLOAT3 spawnPosition, float moveDegree) = 0;

	/// <summary>
	/// ���t���[������
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	void Damage();

	/// <summary>
	/// �폜
	/// </summary>
	void SetDelete();

	/// <summary>
	/// �m�b�N�o�b�N�̏����Z�b�g
	/// </summary>
	/// <param name="angle">������Ԋp�x</param>
	/// <param name="powerLevel">�m�b�N�o�b�N�̋���</param>
	/// <param name="powerMagnification">�m�b�N�o�b�N�̋����̔{��</param>
	/// <param name="shockWaveGroup">�Ռ��g�̎��</param>
	virtual void SetKnockBack(float angle, int powerLevel, float powerMagnification, int shockWaveGroup);

	/// <summary>
	/// ���f�����Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(Model* model) { enemyObject->SetModel(model); }

	/// <summary>
	///	�ǂɂԂ�����������
	/// </summary>
	/// <returns>�ǂɂԂ�������</returns>
	virtual bool IsCollisionWall();

	//getter
	int GetGroup() { return group; }
	XMFLOAT3 GetPosition() { return enemyObject->GetPosition(); }
	XMFLOAT3 GetRotation() { return enemyObject->GetRotation(); }
	XMFLOAT3 GetScale() { return enemyObject->GetScale(); }
	float GetMoveDegree() { return moveDegree; }
	int GetDamagePower() { return damagePower; }
	bool GetIsKnockBack() { return isKnockBack; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsCreateEnemy() { return isCreateEnemy; }
	bool GetIsDelete() { return isDelete; }
	int GetLastCollisionShockWave() { return lastCollisionShockWave; }

protected:
	/// <summary>
	/// �ړ�����
	/// </summary>
	virtual void Move() = 0;

	/// <summary>
	/// ���U���g�V�[���p�̈ړ�����
	/// </summary>
	virtual void ResultMove() = 0;

	/// <summary>
	/// �ړ��p�x��ݒ�
	/// </summary>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	void SetMoveAngle(float moveDegree);

	/// <summary>
	/// �m�b�N�o�b�N
	/// </summary>
	virtual void KnockBack();

	/// <summary>
	/// ���E����
	/// </summary>
	void ReflectionX();

	/// <summary>
	/// �㉺����
	/// </summary>
	void ReflectionY();

protected:
	//�g�̃��C��
	static XMFLOAT2 wallLineMin;
	static XMFLOAT2 wallLineMax;
	//�^�[�Q�b�g���W
	static XMFLOAT3 targetPos;
	//���U���g�V�[���p�̓��������邩
	static bool isResultMove;

protected:
	//�����O���[�v
	int group = None;
	//�G�I�u�W�F�N�g
	Object3d* enemyObject = nullptr;
	//HP
	int HP = 1;
	//�����Ă��邩
	bool isAlive = true;
	//�ړ��p�x
	float moveDegree = 0;
	//�ړ��p�x
	float moveAngle = 0.0f;
	//�ړ����x
	float moveSpeed = 1.0f;
	//�ړ���
	XMFLOAT2 vel = {};
	//��ʓ��ɂ��邩
	bool isInScreen = false;
	//�ړ��p�x�ύX���x
	float changeAngleSpeed = 0;
	//�m�b�N�o�b�N���鎞��
	int knockBackTime = 0;
	//�m�b�N�o�b�N�^�C�}�[
	int knockBackTimer = 0;
	//�m�b�N�o�b�N�̊p�x
	float knockBackAngle = 0.0f;
	//�m�b�N�o�b�N�̋���
	float knockBackPower = 0;
	//�m�b�N�o�b�N���Ă��邩
	bool isKnockBack = false;
	//�ǂɗ^����_���[�W�̋���
	int damagePower = 0;
	//�Ō�ɓ��������Ռ��g�̎��
	int lastCollisionShockWave = 0;
	//�V���ɓG�𐶐����邩
	bool isCreateEnemy = false;
	//�G�t�F�N�g
	int effectCount = 0;
	//�폜���邩
	bool isDelete = false;
};