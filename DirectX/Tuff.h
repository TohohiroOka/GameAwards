#pragma once
#include "BossEnemy.h"

class Tuff :public BossEnemy
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
	//�s���p�^�[��
	enum MovementPattern
	{
		ThreeWayBullet,	//3�����e
		MoveRandomAngle,//�����_���Ŋp�x��ݒ肵�Ĉړ�������
		FiveWayBullet,	//5�����e
		MoveTopScreen,	//��ʏ㕔�Ɉړ�
		FallLockOn,		//�����n�_���b�N�I��
		Shake,			//�k����
		Fall,			//����
		MoveRandomPos,	//�����_���Ŏw�肵�����W�Ɉړ�������
	};

public:
	/// <summary>
	/// �^�b�t����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <returns>�^�b�t</returns>
	static Tuff* Create(Model* model);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Tuff();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <returns>����</returns>
	bool Initialize(Model* model) override;

	/// <summary>
	/// ���t���[������
	/// </summary>
	/// <param name="targetPosition">�W�I�̍��W</param>
	void Update(XMFLOAT3 targetPosition) override;

protected:
	/// <summary>
	/// �X�|�[��
	/// </summary>
	void Spawn() override;

	/// <summary>
	/// ���̍s���Ɉڂ�
	/// </summary>
	void ChangeAction();

	/// <summary>
	/// 3�����e�̔��˖��ߊǗ�
	/// </summary>
	void ShotBulletThreeWay();

	/// <summary>
	/// 5�����e�̔��˖��ߊǗ�
	/// </summary>
	void ShotBulletFiveWay();

	/// <summary>
	/// �����_���ňړ��p�x���Z�b�g
	/// </summary>
	void SetRandomAngle();

	/// <summary>
	/// �����_���ŃZ�b�g�����p�x�Ɉړ�
	/// </summary>
	void MoveAngle();

	/// <summary>
	/// ��ʏ㕔�Ɉړ������Z�b�g
	/// </summary>
	void SetMoveTop();

	/// <summary>
	/// ��ʏ㕔�Ɉړ�
	/// </summary>
	void MoveTop();

	/// <summary>
	/// �^�[�Q�b�g�����b�N�I�����ē���œ�����
	/// </summary>
	/// <param name="targetPosition">�W�I�̍��W</param>
	void LockOnTargetHead(XMFLOAT3 targetPosition);

	/// <summary>
	/// �����_���ňړ����W���Z�b�g
	/// </summary>
	void SetRandomPos();

	/// <summary>
	/// �V�F�C�N�����Z�b�g
	/// </summary>
	void SetShake();

	/// <summary>
	/// �I�u�W�F�N�g���V�F�C�N������
	/// </summary>
	void ShakeObject();

	/// <summary>
	/// ���������Z�b�g
	/// </summary>
	void SetFall();

	/// <summary>
	/// �I�u�W�F�N�g�𗎉�������
	/// </summary>
	void FallObject();

	/// <summary>
	/// �����_���ŃZ�b�g�������W�Ɉړ�
	/// </summary>
	void MovePos();

	/// <summary>
	/// �g�̃��C���ɓ������Ă���
	/// </summary>
	void CollisitonFrame();

protected:
	//�ړ����x
	float moveSpeed = 0;
	//�ړ��p�x
	float moveAngle = 0;
	//�C�[�W���O�ō��W�ړ����̃X�^�[�g���W
	XMFLOAT3 easeStartPos = {};
	//�C�[�W���O�ō��W�ړ����̃G���h���W
	XMFLOAT3 easeEndPos = {};
	//�V�F�C�N���̊���W
	XMFLOAT3 shakeBasePos = {};
};