#pragma once
#include "Object3d.h"

class Porta
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
	/// �|���^����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	/// <returns>�|���^</returns>
	static Porta *Create(Model *model, XMFLOAT3 spawnPosition, float moveDegree);

	/// <summary>
	/// ���˃��C�����Z�b�g
	/// </summary>
	/// <param name="reflectionLine">���˃��C��</param>
	static void SetReflectionLine(XMFLOAT2 reflectionLine) { Porta::reflectionLine = reflectionLine; }

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Porta();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <returns>����</returns>
	bool Initialize(Model *model, XMFLOAT3 spawnPosition, float moveDegree);

	/// <summary>
	/// ���t���[������
	/// </summary>
	/// <param name="targetPosition">�i�s�����ɂȂ�W�I�̍��W</param>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

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

	//getter
	XMFLOAT3 GetPosition() { return enemyObject->GetPosition(); }
	XMFLOAT3 GetScale() { return enemyObject->GetScale(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsDelete() { return isDelete; }

protected:
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// �ړ��p�x��ݒ�
	/// </summary>
	/// <param name="moveDegree">�ړ��p�x(�^�オ0)</param>
	void SetMoveAngle(float moveDegree);

	/// <summary>
	/// ���E����
	/// </summary>
	void ReflectionX();

	/// <summary>
	/// �㉺����
	/// </summary>
	void ReflectionY();

	/// <summary>
	/// �X�s�[�h�A�b�v
	/// </summary>
	void SpeedUp();

protected:
	//���˂��郉�C��
	static XMFLOAT2 reflectionLine;

protected:
	//�G�I�u�W�F�N�g
	Object3d *enemyObject = nullptr;
	//�ړ��p�x
	float moveDegree = 0;
	//�ړ��p�x
	float moveAngle = 0.0f;
	//�ړ����x
	float moveSpeed = 0.5f;
	//�ړ���
	XMFLOAT2 vel = {};
	//�̗�
	int HP = 20;
	//�����Ă��邩
	bool isAlive = true;
	//��ʓ��ɂ��邩
	bool isInScreen = false;
	//�폜���邩
	bool isDelete = false;
};

