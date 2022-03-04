#pragma once
#include "Sprite.h"

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
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <returns>�v���C���[�e</returns>
	static PlayerBullet *Create(UINT texNumber);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlayerBullet();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <returns>����</returns>
	bool Initialize(UINT texNumber);

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
	void BulletStart(XMFLOAT2 position, float rotation);

	/// <summary>
	/// �e���S
	/// </summary>
	void Dead();

	//getter
	XMFLOAT2 GetPosition() { return bulletSprite->GetPosition(); }
	XMFLOAT2 GetSize() { return bulletSprite->GetSize(); }
	int GetPower() { return power; }
	bool GetIsAlive() { return isAlive; };

private:
	//�e�X�v���C�g
	Sprite *bulletSprite = nullptr;
	//���ˊp�x
	float angle = 0.0f;
	//�e�̋���
	int power = 10;
	//�e�������Ă��邩
	bool isAlive = false;
};
