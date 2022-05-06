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
#include "LandingPoint.h"
#include "BaseEnemy.h"
#include "StageEffect.h"
#include "Wall.h"
#include "ShockWave.h"
#include "BuckGround.h"
#include "Combo.h"
#include "TimeLimit.h"
#include "BreakScore.h"
#include "BigShockWaveGauge.h"
#include "TimeLimitGauge.h"

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
	/// �v���C���[������Ԋu�ŏo��Ռ��g����
	/// </summary>
	void PlayerShockWaveStart(XMFLOAT3 pos);

	/// <summary>
	/// �|�C�̂ďՌ��g����
	/// </summary>
	void LitteringShockWaveStart(XMFLOAT3 pos);

	/// <summary>
	/// ����Ռ��g����
	/// </summary>
	void BigShockWaveStart(XMFLOAT3 pos);

	/// <summary>
	/// �v���C���[�|�C�̂Ēe����
	/// </summary>
	void ShotPlayerBullet();

	/// <summary>
	/// ���i�G�𐶐�
	/// </summary>
	void SpawnStraighter();

	/// <summary>
	/// �����G�𐶐�
	/// </summary>
	void SpawnDivision();

	/// <summary>
	/// ���o�G�𐶐�
	/// </summary>
	void SpawnReleaser();

	/// <summary>
	/// �Ǐ]�G�𐶐�
	/// </summary>
	void SpawnChaser();

	/// <summary>
	/// �G����G�𐶐�
	/// </summary>
	/// <param name="enemy">�G</param>
	void SpawnEnemyToEnemy(BaseEnemy* enemy);

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
	Model* pBodyModel = nullptr;//�v���C���[�̑̂̃��f��

	Model* straighterModel1 = nullptr;//���i�G�̃��f��1
	Model* straighterModel2 = nullptr;//���i�G�̃��f��2
	Model* straighterModel3 = nullptr;//���i�G�̃��f��3
	Model* straighterModel4 = nullptr;//���i�G�̃��f��4

	Model* divisionModel1 = nullptr;//�����G�̃��f��1
	Model* divisionModel2 = nullptr;//�����G�̃��f��2
	Model* divisionModel3 = nullptr;//�����G�̃��f��3
	Model* divisionModel4 = nullptr;//�����G�̃��f��4

	Model* releaserModel1 = nullptr;//���o�G�̃��f��1
	Model* releaserModel2 = nullptr;//���o�G�̃��f��2
	Model* releaserModel3 = nullptr;//���o�G�̃��f��3
	Model* releaserModel4 = nullptr;//���o�G�̃��f��4

	Model* eBullModel = nullptr;//�G�̒e�̃��f��
	Model* hexagonModel = nullptr;//�Z�p�`�̃��f��
	Model* happyModel = nullptr;//�^�o�R���f��
	Model* portaModel = nullptr;//�|���^�̃��f��
	Model* charoModel = nullptr;//�`�����̃��f��
	Model* frameModel = nullptr;//�t���[���̃��f��
	Model* waveModel = nullptr;//�Ռ��g�̃��f��

	//�v���C���[
	Player* player = nullptr;
	//�Ռ��g(0�ԁF�v���C���[�A1,2,3�ԁF�|�C�̂āA4�ԁF����)
	static const int shockWaveNum = 5;
	ShockWave* shockWave[shockWaveNum] = { nullptr };
	//�v���C���[�e
	static const int playerBulletNum = 3;
	PlayerBullet* playerBullet[playerBulletNum] = { nullptr };
	//���e�n�_
	LandingPoint* landingPoint = nullptr;

	//�G
	std::list <BaseEnemy*>enemys;

	//�J��������
	XMFLOAT3 cameraPos = { 0, 0, -200 };
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

	//��
	Wall* wall = nullptr;

	//�G�t�F�N�g
	StageEffect* effects = nullptr;

	//�w�i
	BuckGround* buckGround = nullptr;

	//�R���{
	Combo* combo = nullptr;
	//��������
	TimeLimit* timeLimit = nullptr;
	//�󂵂��X�R�A
	BreakScore* breakScore = nullptr;
	//����Ռ��g�p�Q�[�W
	BigShockWaveGauge* shockWaveGauge = nullptr;
	//�������ԉ񕜗p�Q�[�W
	TimeLimitGauge* timeLimitGauge = nullptr;
};
