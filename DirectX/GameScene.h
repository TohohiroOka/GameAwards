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
#include "LaserSite.h"
#include "GaruEnemy.h"
#include "Charo.h"
#include "Porta.h"
#include "BossEnemy.h"
#include "EnemyBullet.h"
#include "FixedEnemy.h"
#include "ConnectCircle.h"
#include "PowerUpLine.h"
#include "StageEffect.h"
#include "Frame.h"
#include "ShockWave.h"
#include "BuckGround.h"
#include "Core.h"
#include "Energy.h"
#include "TimeLimit.h"
#include "Pin.h"
#include "AbsorptionCircle.h"

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
	void Initialize(Camera* camera);

	/// <summary>
	/// ���t���[������
	/// </summary>
	/// <param name="camera">�J�����N���X�̃C���X�^���X</param>
	void Update(Camera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �Q�[����������
	/// </summary>
	void ResetGame();

	/// <summary>
	/// �v���C���[�e�З͗���
	/// </summary>
	void PowerUpPlayerBullet();

	/// <summary>
	/// �v���C���[�e����
	/// </summary>
	/// <param name="bulletPowerLevel">�e�̈З�</param>
	void ShotPlayerBullet(const int bulletPowerLevel);

	/// <summary>
	/// �G�����Ǘ�
	/// </summary>
	void SpawnEnemyManager(bool isBossStage, int wave);

	/// <summary>
	/// �G����
	/// </summary>
	void SpawnEnemyGroup(int spawnPattern, int spawnSet, int wave);

	/// <summary>
	/// �G(�K����)�𐶐�
	/// </summary>
	void SpawnGaruEnemy(int spawnPattern, int wave);

	/// <summary>
	/// �K�����̒e���˂��Ǘ�
	/// </summary>
	/// <param name="garuEnemy">�K����</param>
	void GaruEnemyShotBullet(GaruEnemy* garuEnemy);

	/// <summary>
	/// �G(�`�����E�|���^)�𐶐�
	/// </summary>
	void SpawnCharoPorta(int spawnPattern, int wave);

	/// <summary>
	/// �{�X��J�n
	/// </summary>
	void BossStageStart();

	/// <summary>
	///	�{�X�̒e���˂��Ǘ�
	/// </summary>
	/// <param name="moveBossNumber">�����Ă���{�X</param>
	void BossEnemyShotBullet(int moveBossNumber);

	/// <summary>
	/// �{�X�̍U���̏Ռ��ɂ��G���~�点��
	/// </summary>
	void BossImpactFallEnemy();

	/// <summary>
	/// �s�����쐬�J�n
	/// </summary>
	void CreatePinStart();

	/// <summary>
	/// �s�����쐬
	/// </summary>
	void CreatePin();

	/// <summary>
	/// �p���[�A�b�v�����쐬
	/// </summary>
	void CreatePowerUpLine(ConnectCircle* startPoint, ConnectCircle* endPoint);

	/// <summary>
	/// �J�����X�V
	/// </summary>
	/// <param name="camera">�J����</param>
	void CameraUpdate(Camera* camera);

	/// <summary>
	/// �J���������ύX���Z�b�g
	/// </summary>
	/// <param name="distance">����</param>
	void SetChangeCameraDistance(float distance);

private:// �����o�ϐ�
	//��
	Audio* audio = nullptr;
	//���C�g
	LightGroup* light = nullptr;

	//���f��
	Model* circleModel = nullptr;//�^�o�R�̃��f��
	Model* playerModel = nullptr;//�v���C���[�̃��f��
	Model* pBodyModel = nullptr;//�v���C���[�̑̂̃��f��
	Model* pHead01Model = nullptr;//�v���C���[�̓��̃��f��(HP1)
	Model* pHead02Model = nullptr;//�v���C���[�̓��̃��f��(HP2)
	Model* pHead03Model = nullptr;//�v���C���[�̓��̃��f��(HP3)
	Model* razorModel = nullptr;//���[�U�[�̃��f��
	Model* pBullModel = nullptr;//�v���C���[�̒e�̃��f��
	Model* enemy01Model = nullptr;//�G01(�K���^)�̃��f��
	Model* enemyPoint01Model = nullptr;//�G01(�K���^)�̏o���ʒu�̃��f��
	Model* enemy02Model = nullptr;//�G02(�K���^�^)�̃��f��
	Model* enemyPoint02Model = nullptr;//�G02(�K���^�^)�̏o���ʒu�̃��f��
	Model* eBullModel = nullptr;//�G�̒e�̃��f��
	Model* deadEnemyModel = nullptr;//���񂾓G�̃��f��
	Model* initialCircleCoreModel = nullptr;//�Œ�G�̃R�A�̃��f��
	Model* initialCircleSquareModel = nullptr;//�Œ�G�̊O�g�̃��f��
	Model* hexagonModel = nullptr;//�Z�p�`�̃��f��
	Model* happyModel = nullptr;//�^�o�R���f��
	Model* portaModel = nullptr;//�|���^�̃��f��
	Model* charoModel = nullptr;//�`�����̃��f��
	Model* tuffModel = nullptr;//�^�b�t�̃��f��
	Model* frameModel = nullptr;//�t���[���̃��f��
	Model* waveModel = nullptr;//�Ռ��g�̃��f��
	Model* coreModel = nullptr;//�R�A�̃��f��
	Model* RBbuttonModel = nullptr;//RB�{�^���̃��f��

	//�v���C���[
	Player* player = nullptr;
	//���[�U�[�T�C�g
	LaserSite* laserSite = nullptr;
	//�v���C���[�e
	static const int playerBulletNum = 10;
	PlayerBullet* playerBullet[playerBulletNum] = { nullptr };
	//���߂����邩
	int bulletPowerLevel = 0;
	bool isBulletPowerUp = false;
	int bulletPowerUpTimer = 0;

	//�G�l���M�[�|�C���g
	Energy* energy = nullptr;

	//�R�A
	Core* core = nullptr;
	//title�̔��j���o�o���ʒu
	XMFLOAT3 explosionPosition = {};

	//�G(�K����)
	std::list <GaruEnemy*>garuEnemys;
	//�G(�`����)
	std::list <Charo*>charoEnemys;
	//�G(�|���^)
	std::list <Porta*>portaEnemys;
	//�{�X
	static const int bossNum = 1;
	BossEnemy* bossEnemy[bossNum] = { nullptr };
	//�{�X�풆
	bool isBossStage = false;
	//�������{�X
	int moveBossNumber;
	//�G�̒e
	static const int enemyBulletNum = 100;
	EnemyBullet* enemyBullet[enemyBulletNum] = { nullptr };

	//�s��
	std::list<Pin*> pins;
	bool isCreatePin = false;
	int createPinTimer = 0;

	//�Œ�G
	std::list <FixedEnemy*> fixedEnemys;

	//�R�l�N�g�T�[�N��
	std::list <ConnectCircle*> connectCircles;

	//�p���[�A�b�v��
	std::list <PowerUpLine*> powerUpLines;

	//�Ռ��g
	std::list <ShockWave*> shockWaves;

	//�z���~
	std::list <AbsorptionCircle*> absorptionCircles;

	//�J��������
	XMFLOAT3 cameraPos = { 0, 0, -100 };
	//�J���������C�[�W���O�J�n
	float cameraDisEaseStart = 0;
	//�J���������C�[�W���O�I��
	float cameraDisEaseEnd = 0;
	//�J����������ύX����
	bool isChangecameraDis = false;
	//�J���������C�[�W���O�^�C�}�[
	int cameraDisEaseTimer = 0;
	//��ʃV�F�C�N�X�C�b�`
	bool isShake = false;
	//��ʃV�F�C�N����
	int ShakeTime = 0;

	//��ʘg
	Frame* frame = nullptr;

	//��������
	TimeLimit* timeLimit = nullptr;

	//�X�|�[���p�^�[��
	bool isSpawnTimer = false;//�X�|�[���^�C�}�[�̃J�E���g�����邩���Ȃ���
	int spawnTimer = 0;//�X�|�[���^�C�}�[(�G�̏o���p�̃J�E���g)
	int spawnInterval = 300;//�X�|�[���Ԋu�̃t���[����
	int spawnPattern = 0;//�X�|�[���p�^�[��(�G�p�^�[���v�Z�p)
	int spawnSet = 0;//�X�|�[���̗����p

	int wave = 1;

	//�G�t�F�N�g
	StageEffect* effects = nullptr;

	//�w�i
	BuckGround* buckGround = nullptr;
};
