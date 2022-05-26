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
	/// 衝撃波発射
	/// </summary>
	void ShockWaveStart(XMFLOAT3 pos, int powerLevel);

	/// <summary>
	/// 追従敵を生成
	/// </summary>
	void SpawnChaser();

	/// <summary>
	/// 分裂敵を生成
	/// </summary>
	void SpawnDivision();

	/// <summary>
	/// 放出敵を生成
	/// </summary>
	void SpawnReleaser();


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
	/// /// <param name="score">現在のタイム</param>
	void SpawnEnemyManager(int score, int time);

	/// <summary>
	/// 壁のラインセット
	/// </summary>
	/// <param name="isTitle"></param>
	void WallLineSet(const bool isTitle);

	/// <summary>
	/// サウンドの再生、停止
	/// </summary>
	/// <param name="soundNum">再生するサウンド</param>
	/// <param name="isBGM">trueならBGM、falseならSE</param>
	/// <param name="stop">trueなら停止、falseなら停止させない</param>
	void SoundManager(int soundNum, bool isBGM, bool stop);

private:// メンバ変数
	//音
	//Audio* audio = nullptr;
	//ライト
	LightGroup* light = nullptr;

	//モデル
	Model* titleLogoModel = nullptr;//タイトルロゴのモデル
	Model* playerModel = nullptr;//プレイヤーのモデル
	Model* waveModel = nullptr;//衝撃波のモデル
	Model* healingZoneModel = nullptr;//ゲージ回復地点のモデル
	Model* XButtonModel = nullptr;//Xボタンのモデル
	Model* chaserModel = nullptr;//追跡敵のモデル
	Model* divisionModel = nullptr;//分裂敵のモデル
	Model* releaserModel = nullptr;//放出敵のモデル

	//壁との当たり判定の範囲
	XMFLOAT2 minWallLinePosition = {};
	XMFLOAT2 maxWallLinePosition = {};

	//範囲外範囲
	XMFLOAT2 outsideMinPosition = {};
	XMFLOAT2 outsideMaxPosition = {};

	//プレイヤー
	Player* player = nullptr;
	//衝撃波
	ShockWave* shockWave = nullptr;
	//ゲージ回復地点
	HealingZone* healingZone = nullptr;

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

	//ゲーム説明
	Explanation* explanation = nullptr;

	//UIを囲う枠
	UIFrame* UIFrame = nullptr;

	//壁破壊スコア
	BreakScore* breakScore = nullptr;
	//制限時間
	TimeLimitGauge* timeLimitGauge = nullptr;
	//衝撃波用ゲージ
	ShockWaveGauge* shockWaveGauge = nullptr;

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

	//サウンドの再生用
	int sound[17];
	bool isBGM = false;//BGMのフラグ
	//0:カーソル移動 1:選択肢決定 2:プレイヤー被弾 3:プレイヤー衝撃波 4:敵吹っ飛び 5:敵壁衝突 6:壁耐久値0 7:巨大衝撃波 8:GO 9:FINISH 10:壁生成 11:拡散
	//12:必殺技溜まったとき 13:リザルトロゴ表示 14:追尾する敵の移動音 15:敵強吹っ飛び 16:BGM
};