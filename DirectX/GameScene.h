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
#include "WallManager.h"
#include "ShockWave.h"
#include "BuckGround.h"
#include "BackGround.h"
#include "UIFrame.h"
#include "Combo.h"
#include "TimeLimit.h"
#include "BreakScore.h"
#include "BigShockWaveGauge.h"
#include "TimeLimitGauge.h"
#include "ReadyGo.h"
#include "Finish.h"
#include "ResultUI.h"

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
	enum SceneName
	{
		TitleScene,
		ReadyGoScene,
		GamePlayScene,
		FinishScene,
		ResultScene,
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

private:// �����o�ϐ�
	//��
	Audio* audio = nullptr;
	//���C�g
	LightGroup* light = nullptr;

	//���f��
	Model* titleLogoModel = nullptr;//�^�C�g�����S�̃��f��
	Model* circleModel = nullptr;//�^�o�R�̃��f��
	Model* playerModel = nullptr;//�v���C���[�̃��f��

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

	Model* chaserModel1 = nullptr;//�ǐՓG�̃��f��1
	Model* chaserModel2 = nullptr;//�ǐՓG�̃��f��2
	Model* chaserModel3 = nullptr;//�ǐՓG�̃��f��3
	Model* chaserModel4 = nullptr;//�ǐՓG�̃��f��4

	Model* eBullModel = nullptr;//�G�̒e�̃��f��
	Model* hexagonModel = nullptr;//�Z�p�`�̃��f��
	Model* portaModel = nullptr;//�|���^�̃��f��
	Model* charoModel = nullptr;//�`�����̃��f��
	Model* frameModel = nullptr;//�t���[���̃��f��
	Model* waveModel = nullptr;//�Ռ��g�̃��f��

	//�ǂ͈̔�
	const XMFLOAT2 minPosition = WallObject::GetWallMinPosition();
	const XMFLOAT2 maxPosition = WallObject::GetWallMaxPosition();

	//�͈͊O�͈�
	const float outsideRange = 20.0f;
	const XMFLOAT2 outsideMinPosition = { minPosition.x - outsideRange,minPosition.y - outsideRange };
	const XMFLOAT2 outsideMaxPosition = { maxPosition.x + outsideRange,maxPosition.y + outsideRange };

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

	//��
	WallManager* wall = nullptr;

	//�G�t�F�N�g
	StageEffect* effects = nullptr;

	//�w�i
	//BuckGround* buckGround = nullptr;
	BackGround* backGround = nullptr;

	//UI���͂��g
	UIFrame* UIFrame = nullptr;

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

	//ReadyGo
	ReadyGo* readyGo = nullptr;
	//Finish
	Finish* finish = nullptr;
	//���U���g�V�[��UI
	ResultUI* resultUI = nullptr;

	//�V�[��
	int scene = SceneName::TitleScene;

	//�J��������
	XMFLOAT3 cameraPos = { 0, 0, -200 };
	//��ʃV�F�C�N�X�C�b�`
	bool isShake = false;
	//��ʃV�F�C�N����
	int ShakeTime = 0;
};
