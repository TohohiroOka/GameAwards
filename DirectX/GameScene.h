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
#include "BaseEnemy.h"
#include "StageEffect.h"
#include "WallManager.h"
#include "ShockWave.h"
#include "HealingZone.h"
#include "BackGround.h"

#include "Blackout.h"
#include "Explanation.h"
#include "UIFrame.h"
#include "TimeLimitGauge.h"
#include "ShockWaveGauge.h"
#include "BreakScore.h"
#include "ReadyGo.h"
#include "PauseUI.h"
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
		TitleStayScene,
		TitleScene,
		ReadyGoScene,
		GamePlayScene,
		PauseScene,
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
	/// �^�C�g���V�[���ڍs�p�̏�����
	/// </summary>
	void ResetTitleScene();

	/// <summary>
	/// �Q�[����������
	/// </summary>
	void ResetGame();

	/// <summary>
	/// �Ռ��g����
	/// </summary>
	void ShockWaveStart(XMFLOAT3 pos, int powerLevel);

	/// <summary>
	/// �Ǐ]�G�𐶐�
	/// </summary>
	void SpawnChaser();

	/// <summary>
	/// �����G�𐶐�
	/// </summary>
	void SpawnDivision();

	/// <summary>
	/// ���o�G�𐶐�
	/// </summary>
	void SpawnReleaser();


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
	/// �G�𐶐�
	/// </summary>
	/// <param name="score">�j�󂵂��ǂ̐�</param>
	/// /// <param name="score">���݂̃^�C��</param>
	void SpawnEnemyManager(int score, int time);

	/// <summary>
	/// �ǂ̃��C���Z�b�g
	/// </summary>
	/// <param name="isTitle"></param>
	void WallLineSet(const bool isTitle);

	/// <summary>
	/// �T�E���h�̍Đ��A��~
	/// </summary>
	/// <param name="soundNum">�Đ�����T�E���h</param>
	/// <param name="isBGM">true�Ȃ�BGM�Afalse�Ȃ�SE</param>
	/// <param name="stop">true�Ȃ��~�Afalse�Ȃ��~�����Ȃ�</param>
	void SoundManager(int soundNum, bool isBGM, bool stop);

private:// �����o�ϐ�
	//��
	//Audio* audio = nullptr;
	//���C�g
	LightGroup* light = nullptr;

	//���f��
	Model* titleLogoModel = nullptr;//�^�C�g�����S�̃��f��
	Model* playerModel = nullptr;//�v���C���[�̃��f��
	Model* waveModel = nullptr;//�Ռ��g�̃��f��
	Model* healingZoneModel = nullptr;//�Q�[�W�񕜒n�_�̃��f��
	Model* XButtonModel = nullptr;//X�{�^���̃��f��
	Model* chaserModel = nullptr;//�ǐՓG�̃��f��
	Model* divisionModel = nullptr;//�����G�̃��f��
	Model* releaserModel = nullptr;//���o�G�̃��f��

	//�ǂƂ̓����蔻��͈̔�
	XMFLOAT2 minWallLinePosition = {};
	XMFLOAT2 maxWallLinePosition = {};

	//�͈͊O�͈�
	XMFLOAT2 outsideMinPosition = {};
	XMFLOAT2 outsideMaxPosition = {};

	//�v���C���[
	Player* player = nullptr;
	//�Ռ��g
	ShockWave* shockWave = nullptr;
	//�Q�[�W�񕜒n�_
	HealingZone* healingZone = nullptr;

	//�G
	std::list <BaseEnemy*>enemys;

	//��
	WallManager* wall = nullptr;

	//�G�t�F�N�g
	StageEffect* effects = nullptr;

	//�w�i
	BackGround* backGround = nullptr;

	//�V�[���J�ڗp�Ó]
	Blackout* blackout = nullptr;

	//�Q�[������
	Explanation* explanation = nullptr;

	//UI���͂��g
	UIFrame* UIFrame = nullptr;

	//�ǔj��X�R�A
	BreakScore* breakScore = nullptr;
	//��������
	TimeLimitGauge* timeLimitGauge = nullptr;
	//�Ռ��g�p�Q�[�W
	ShockWaveGauge* shockWaveGauge = nullptr;

	//ReadyGo
	ReadyGo* readyGo = nullptr;
	//�|�[�Y�V�[��UI
	PauseUI* pauseUI = nullptr;
	//Finish
	Finish* finish = nullptr;
	//���U���g�V�[��UI
	ResultUI* resultUI = nullptr;

	//�V�[��
	int scene = SceneName::TitleStayScene;

	//�J��������
	XMFLOAT3 cameraPos = { 0, 0, -200 };
	//��ʃV�F�C�N�X�C�b�`
	bool isShake = false;
	//��ʃV�F�C�N����
	int ShakeTime = 0;

	//�G�̃X�|�[���p
	int spawnTimer = 0;//�X�|�[������܂ł̃J�E���g
	int spawnInterval = 0;//�X�|�[���Ԋu
	int spawnRate = 0;//�X�|�[�����[�g(��x�ɃX�|�[������G�̐�)
	int enemyType = 0;//�G�̎�ޔ��ʗp
	int enemyDirection = 0;//�G�̏o���������ʗp

	//�T�E���h�̍Đ��p
	int sound[17];
	bool isBGM = false;//BGM�̃t���O
	//0:�J�[�\���ړ� 1:�I�������� 2:�v���C���[��e 3:�v���C���[�Ռ��g 4:�G������� 5:�G�ǏՓ� 6:�Ǒϋv�l0 7:����Ռ��g 8:GO 9:FINISH 10:�ǐ��� 11:�g�U
	//12:�K�E�Z���܂����Ƃ� 13:���U���g���S�\�� 14:�ǔ�����G�̈ړ��� 15:�G��������� 16:BGM
};