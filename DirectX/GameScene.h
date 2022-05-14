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
#include "BackGround.h"

#include "Blackout.h"
#include "TitleUI.h"
#include "UIFrame.h"
#include "TimeLimitGauge.h"
#include "BigShockWaveGauge.h"
#include "ReadyGo.h"
#include "PauseUI.h"
#include "Finish.h"
#include "ResultUI.h"

class Input;

class GameScene
{
private:// エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
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

public:// メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="camera">カメラクラスのインスタンス</param>
	void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// タイトルシーン移行用の初期化
	/// </summary>
	void ResetTitleScene();

	/// <summary>
	/// ゲームを初期化
	/// </summary>
	void ResetGame();

	/// <summary>
	/// プレイヤーから一定間隔で出る衝撃波発射
	/// </summary>
	void PlayerShockWaveStart(XMFLOAT3 pos);

	/// <summary>
	/// 巨大衝撃波発射
	/// </summary>
	void BigShockWaveStart(XMFLOAT3 pos);

	/// <summary>
	/// 直進敵を生成
	/// </summary>
	void SpawnStraighter();

	/// <summary>
	/// 分裂敵を生成
	/// </summary>
	void SpawnDivision();

	/// <summary>
	/// 放出敵を生成
	/// </summary>
	void SpawnReleaser();

	/// <summary>
	/// 追従敵を生成
	/// </summary>
	void SpawnChaser();

	/// <summary>
	/// 敵から敵を生成
	/// </summary>
	/// <param name="enemy">敵</param>
	void SpawnEnemyToEnemy(BaseEnemy* enemy);

	/// <summary>
	/// カメラ更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	void CameraUpdate(Camera* camera);

	/// <summary>
	/// 敵を生成
	/// </summary>
	/// <param name="score">破壊した壁の数</param>
	void SpawnEnemyManager(int score);

private:// メンバ変数
	//音
	Audio* audio = nullptr;
	//ライト
	LightGroup* light = nullptr;

	//モデル
	Model* titleLogoModel = nullptr;//タイトルロゴのモデル
	Model* circleModel = nullptr;//タバコのモデル
	Model* playerModel = nullptr;//プレイヤーのモデル

	Model* straighterModel1 = nullptr;//直進敵のモデル1
	Model* straighterModel2 = nullptr;//直進敵のモデル2
	Model* straighterModel3 = nullptr;//直進敵のモデル3
	Model* straighterModel4 = nullptr;//直進敵のモデル4

	Model* divisionModel1 = nullptr;//分裂敵のモデル1
	Model* divisionModel2 = nullptr;//分裂敵のモデル2
	Model* divisionModel3 = nullptr;//分裂敵のモデル3
	Model* divisionModel4 = nullptr;//分裂敵のモデル4

	Model* releaserModel1 = nullptr;//放出敵のモデル1
	Model* releaserModel2 = nullptr;//放出敵のモデル2
	Model* releaserModel3 = nullptr;//放出敵のモデル3
	Model* releaserModel4 = nullptr;//放出敵のモデル4

	Model* chaserModel1 = nullptr;//追跡敵のモデル1
	Model* chaserModel2 = nullptr;//追跡敵のモデル2
	Model* chaserModel3 = nullptr;//追跡敵のモデル3
	Model* chaserModel4 = nullptr;//追跡敵のモデル4

	Model* eBullModel = nullptr;//敵の弾のモデル
	Model* hexagonModel = nullptr;//六角形のモデル
	Model* portaModel = nullptr;//ポルタのモデル
	Model* charoModel = nullptr;//チャロのモデル
	Model* frameModel = nullptr;//フレームのモデル
	Model* waveModel = nullptr;//衝撃波のモデル

	//壁の生成範囲
	const XMFLOAT2 minWallPosition = WallObject::GetWallMinPosition();
	const XMFLOAT2 maxWallPosition = WallObject::GetWallMaxPosition();

	//壁との当たり判定の範囲
	//壁生成範囲との差分
	const float wallPosDis = 18.0f;
	const XMFLOAT2 minWallLinePosition = { minWallPosition.x + wallPosDis, minWallPosition.y + wallPosDis };
	const XMFLOAT2 maxWallLinePosition = { maxWallPosition.x - wallPosDis, maxWallPosition.y - wallPosDis };

	//範囲外範囲
	const float outsideRange = 20.0f;
	const XMFLOAT2 outsideMinPosition = { minWallLinePosition.x - outsideRange,minWallLinePosition.y - outsideRange };
	const XMFLOAT2 outsideMaxPosition = { maxWallLinePosition.x + outsideRange,maxWallLinePosition.y + outsideRange };

	//プレイヤー
	Player* player = nullptr;
	//衝撃波(0番：プレイヤー、1番：巨大)
	static const int shockWaveNum = 2;
	ShockWave* shockWave[shockWaveNum] = { nullptr };

	//敵
	std::list <BaseEnemy*>enemys;

	//壁
	WallManager* wall = nullptr;

	//エフェクト
	StageEffect* effects = nullptr;

	//背景
	BackGround* backGround = nullptr;

	//シーン遷移用暗転
	Blackout* blackout = nullptr;

	//タイトルシーンUI
	TitleUI* titleUI = nullptr;

	//UIを囲う枠
	UIFrame* UIFrame = nullptr;

	//制限時間
	TimeLimitGauge* timeLimitGauge = nullptr;
	//壊したスコア
	int breakScore = 0;
	//巨大衝撃波用ゲージ
	BigShockWaveGauge* shockWaveGauge = nullptr;

	//ReadyGo
	ReadyGo* readyGo = nullptr;
	//ポーズシーンUI
	PauseUI* pauseUI = nullptr;
	//Finish
	Finish* finish = nullptr;
	//リザルトシーンUI
	ResultUI* resultUI = nullptr;

	//シーン
	int scene = SceneName::TitleStayScene;

	//カメラ距離
	XMFLOAT3 cameraPos = { 0, 0, -200 };
	//画面シェイクスイッチ
	bool isShake = false;
	//画面シェイク時間
	int ShakeTime = 0;

	//敵のスポーン用
	int spawnTimer = 0;//スポーンするまでのカウント
	int spawnInterval = 0;//スポーン間隔
	int spawnRate = 0;//スポーンレート(一度にスポーンする敵の数)
	int enemyType = 0;//敵の種類判別用
	int enemyDirection = 0;//敵の出現方向判別用
};
