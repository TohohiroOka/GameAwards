#pragma once
#include "Object3d.h"
#include "Player.h"

class LandingPoint
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
	/// ���e�n�_����
	/// </summary>
	/// <returns>���e�n�_</returns>
	static LandingPoint* Create(Model* model);

	static void SetMoveRange(XMFLOAT2 moveRangeMin, XMFLOAT2 moveRangeMax) {
		LandingPoint::moveRangeMin = moveRangeMin;
		LandingPoint::moveRangeMax = moveRangeMax;
	}

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~LandingPoint();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(XMFLOAT3 playerPosition, XMFLOAT3 playerRotation);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���S
	/// </summary>
	/// <param name="pointNum">���e�n�_�̔ԍ�(����0)</param>
	void Dead(int pointNum);

	/// <summary>
	/// �Đ���
	/// </summary>
	void Revive();

	/// <summary>
	/// ���e�_���S�Ď���ł��邩
	/// </summary>
	/// <returns>�S�Ď���ł��邩</returns>
	bool CheckAllDead();

	//�v���C���[�ɒǏ]�����邩
	void SetIsChase(bool isChase) { this->isChase = isChase; }

	//getter
	bool GetIsChase() { return isChase; }

private:
	/// <summary>
	/// ����
	/// </summary>
	void Create();

	/// <summary>
	/// ���W�X�V
	/// </summary>
	/// <param name="weaponPosition">�v���C���[�̍��W</param>
	/// <param name="weaponRotation">�v���C���[�̊p�x</param>
	void Chase(XMFLOAT3 position, XMFLOAT3 rotation);


private:
	//������͈�
	static XMFLOAT2 moveRangeMin;
	static XMFLOAT2 moveRangeMax;

private:
	//���e�n�_�I�u�W�F�N�g
	static const int pointNum = 3;
	Object3d* pointCircleObject[pointNum] = { nullptr };
	//�����Ă��邩
	bool isAlive[pointNum] = { true, true, true };
	//��������
	bool isCreate = true;
	//�������鎞�ԃ^�C�}�[
	int createTimer = 0;
	//�Ǐ]���邩
	bool isChase = true;
};