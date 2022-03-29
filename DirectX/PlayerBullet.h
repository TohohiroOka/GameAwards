#pragma once
#include "Object3d.h"
#include "PowerUpLine.h"

class PlayerBullet
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
	/// �v���C���[�e����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�v���C���[�e</returns>
	static PlayerBullet *Create(Model *model = nullptr);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlayerBullet();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>����</returns>
	bool Initialize(Model *model);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �e����
	/// </summary>
	/// <param name="position">�J�n���W</param>
	/// <param name="rotation">�e�̊p�x</param>
	void BulletStart(XMFLOAT3 position, XMFLOAT3 rotation);

	/// <summary>
	/// �e���S
	/// </summary>
	void Dead();

	/// <summary>
	/// �p���[�A�b�v
	/// </summary>
	void PowerUp();
		
	/// <summary>
	/// ��x����̔��������ׂɁA�����̐���m���Ă��邩�ǂ������肷��
	/// </summary>
	bool IsKnowLine(PowerUpLine *line);

	//getter
	XMFLOAT3 GetPosition() { return bulletObject->GetPosition(); }
	XMFLOAT3 GetScale() { return bulletObject->GetScale(); }
	float GetAngle() { return angle; }
	int GetPower() { return power; }
	bool GetIsAlive() { return isAlive; };

private:
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

private:
	//�e�I�u�W�F�N�g
	Object3d *bulletObject = nullptr;
	//���ˊp�x
	float angle = 0.0f;
	//�e�̋���
	int power = 10;
	//�������ԃ^�C�}�[
	int lifeTimer = 0;
	//�����\����
	int lifeTime = 10;
	//�e�������Ă��邩
	bool isAlive = false;
	//�e���m���Ă����
	std::list <PowerUpLine *> alreadyLines;
};
