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
#include "Score.h"
#include "Frame.h"
#include "ShockWave.h"
#include "BuckGround.h"
#include "TitleLogo.h"
#include "TitleUI.h"
#include "ResultUI.h"
#include "Core.h"

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
public:
	//�V�[���ԍ�
	enum SceneName
	{
		Title,		//�^�C�g���V�[��
		Game,		//�Q�[���v���C�V�[��
		ChangeWave,	//�E�F�[�u�ύX�V�[��
		GameOver,	//�Q�[���I�[�o�[�V�[��
		Result,		//���U���g�V�[��
	};
	//�^�C�g���V�[�����̃V�[���ԍ�
	enum TitleSceneName
	{
		SpawnEnemySet,	//�^�C�g���V�[���p�̓G�X�|�[�����Z�b�g
		SpawnEnemy,		//�^�C�g���V�[���p�̓G�X�|�[��
		CreateConnectCircle,//�R�l�N�g�T�[�N�������
		SpawnPlayerCore,//�R�A�ƃv���C���[�X�|�[��
		PlayerMove,		//�v���C���[�ړ��\
		CoreExplosion,	//�R�A����
	};
	//�E�F�[�u�ύX�V�[�����̃V�[���ԍ�
	enum ChangeWaveSceneName
	{
		DeadBossChangeScale,//���S�����{�X�̃T�C�Y��ύX
		BossShockWaveMove,	//�{�X����o��Ռ��g�𓮂���
		WaveUpdate,			//���̃E�F�[�u���Z�b�g
		PlayerReset,		//�v���C���[�����ʒu�ړ��V�[��
		FrameMove,			//�g�I�u�W�F�N�g�ړ��V�[��(��ʃT�C�Y3��1)
		CameraMove,			//�J�����ړ��V�[��(��ʃT�C�Y3��1)
		FrameCameraMove,	//�g�I�u�W�F�N�g�ƃJ�����ړ��V�[��(��ʃT�C�Y1��2��3)
	};
	//�Q�[���I�[�o�[�V�[�����̃V�[���ԍ�
	enum GameOverSceneName
	{
		DeleteBullets,	//�e���폜
		DeletePlayer,	//�v���C���[���폜
		PlayerShockWaveMove,	//�v���C���[����o��Ռ��g�𓮂���
		TitleReturn,	//�^�C�g���ɖ߂�
	};

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
	/// �Q�[����������
	/// </summary>
	void ResetGame();

	/// <summary>
	/// �^�C�g���V�[���p�̓G����
	/// </summary>
	void TitleSceneEnemySpawn();

	/// <summary>
	/// �v���C���[�e����
	/// </summary>
	void ShotPlayerBullet();

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
	/// �p���[�A�b�v�����쐬
	/// </summary>
	void CreatePowerUpLine(ConnectCircle *startPoint, ConnectCircle *endPoint);

	/// <summary>
	/// �J�����X�V
	/// </summary>
	/// <param name="camera">�J����</param>
	void CameraUpdate(Camera *camera);

	/// <summary>
	/// �J���������ύX���Z�b�g
	/// </summary>
	/// <param name="distance">����</param>
	void SetChangeCameraDistance(float distance);

private:// �����o�ϐ�
	//��
	Audio *audio = nullptr;
	//���C�g
	LightGroup *light = nullptr;
	//�X�v���C�g
	Sprite *sprite = nullptr;

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
	Player *player = nullptr;
	//�v���C���[�e
	static const int playerBulletNum = 10;
	PlayerBullet *playerBullet[playerBulletNum] = { nullptr };
	//���[�U�[�T�C�g
	LaserSite *laserSite = nullptr;

	//�R�A
	Core* core = nullptr;
	//title�̔��j���o�o���ʒu
	XMFLOAT3 explosionPosition = {};

	//�G(�K����)
	std::list <GaruEnemy *>garuEnemys;
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
	EnemyBullet *enemyBullet[enemyBulletNum] = { nullptr };

	//�Œ�G
	std::list <FixedEnemy*> fixedEnemys;

	//�R�l�N�g�T�[�N��
	std::list <ConnectCircle *> connectCircles;

	//�p���[�A�b�v��
	std::list <PowerUpLine *> powerUpLines;

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

	//�Ռ��g
	ShockWave* playerShockWave = nullptr;
	ShockWave* bossShockWave = nullptr;

	//�X�|�[���p�^�[��
	bool isSpawnTimer = false;//�X�|�[���^�C�}�[�̃J�E���g�����邩���Ȃ���
	int spawnTimer = 0;//�X�|�[���^�C�}�[(�G�̏o���p�̃J�E���g)
	int spawnInterval = 300;//�X�|�[���Ԋu�̃t���[����
	int spawnPattern = 0;//�X�|�[���p�^�[��(�G�p�^�[���v�Z�p)
	int spawnSet = 0;//�X�|�[���̗����p

	//�^�C�g�����S
	TitleLogo* titleLogo = nullptr;
	//�^�C�g���V�[���p�^�C�}�[
	int titleSceneTimer = 0;
	//�^�C�g���V�[���pUI
	TitleUI* titleUI = nullptr;

	//���U���g�V�[���pUI
	ResultUI* resultUI = nullptr;

	//�V�[��
	int scene = SceneName::Title;
	//�^�C�g���V�[��
	int titleScene = TitleSceneName::SpawnEnemySet;
	//�E�F�[�u�ύX�V�[��
	int changeWaveScene = ChangeWaveSceneName::DeadBossChangeScale;
	//�Q�[���I�[�o�[�V�[��
	int gameOverScene = GameOverSceneName::DeleteBullets;
	//�E�F�[�u
	int wave = 1;

	//�X�R�A
	Score *score = nullptr;

	//�G�t�F�N�g
	StageEffect *effects = nullptr;

	//�w�i
	BuckGround* buckGround = nullptr;
};
