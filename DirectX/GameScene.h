#pragma once

#include<DirectXMath.h>

#include "SafeDelete.h"
#include "Audio.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Emitter.h"
#include "LightGroup.h"
#include "Fbx.h"
#include "Easing.h"
#include "NormalMap.h"
#include "DrawLine.h"
#include "DrawLine3D.h"

#include "Player.h"
#include "PlayerBullet.h"
#include "BaseEnemy.h"
#include "EnemyBullet.h"
#include "DeadEnemyPoint.h"
#include "PowerUpLine.h"

class Input;

class GameScene
{
private:// �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:// �����o�֐�

	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	GameScene() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Camera *camera);

	/// <summary>
	/// ���t���[������
	/// </summary>
	/// <param name="camera">�J�����N���X�̃C���X�^���X</param>
	void Update(Camera *camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList *cmdList);

	/// <summary>
	/// �G�𐶐�
	/// </summary>
	void SpawnEnemy();

	/// <summary>
	/// �p���[�A�b�v�����쐬
	/// </summary>
	void CreatePowerUpLine(DeadEnemyPoint *startPoint, DeadEnemyPoint *endPoint);

private:// �����o�ϐ�
	//��
	Audio *audio = nullptr;
	//���C�g
	LightGroup *light = nullptr;
	//�X�v���C�g
	Sprite *sprite = nullptr;

	//���f��
	Model *circleModel = nullptr;//�^�o�R�̃��f��
	Model *playerModel = nullptr;//�v���C���[�̃��f��
	Model *pBullModel = nullptr;//�v���C���[�̒e�̃��f��
	Model *enemy01Model = nullptr;//�G01(�K���^)�̃��f��
	Model *enemy02Model = nullptr;//�G02(�K���^�^)�̃��f��
	Model *eBullModel = nullptr;//�G�̒e�̃��f��
	Model *deadEnemyModel = nullptr;//���񂾓G�̃��f��

	//�v���C���[
	Player *player = nullptr;
	//�v���C���[�e
	static const int playerBulletNum = 10;
	PlayerBullet *playerBullet[playerBulletNum] = { nullptr };
	//�e�̔��ˊԊu
	int bulletShotTimer = 10;

	//�G
	std::list <BaseEnemy *>enemys;
	//�G�̒e
	static const int enemyBulletNum = 100;
	EnemyBullet *enemyBullet[enemyBulletNum] = { nullptr };

	//���񂾓G�̈ʒu
	std::list <DeadEnemyPoint *> deadEnemyPoints;

	//�p���[�A�b�v��
	std::list <PowerUpLine *> powerUpLines;

	bool isShake = false;
	int ShakeTime = 0;
};
