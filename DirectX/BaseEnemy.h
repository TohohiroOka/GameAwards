#pragma once
#include "Sprite.h"

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
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <returns>����</returns>
	virtual bool Initialize(XMFLOAT2 position) = 0;

	/// <summary>
	/// ���t���[������
	/// </summary>
	virtual void Update() = 0;

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


	//getter
	XMFLOAT2 GetPosition() { return enemySprite->GetPosition(); }
	XMFLOAT2 GetSize() { return enemySprite->GetSize(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	static int GetDeadCount() { return deadCount; }
	int GetDeadNum() { return deadNum; }

private:
	static int deadCount;

protected:
	//�G�X�v���C�g
	Sprite *enemySprite = nullptr;
	//���ˊp�x
	float angle = 0.0f;
	//�̗�
	int HP = 100;
	//�e�������Ă��邩
	bool isAlive = true;
	//���S��
	int deadNum;
};