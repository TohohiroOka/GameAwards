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
private:// エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

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
	/// ゲームを初期化
	/// </summary>
	void ResetGame();

	/// <summary>
	/// プレイヤーから一定間隔で出る衝撃波発射
	/// </summary>
	void PlayerShockWaveStart(XMFLOAT3 pos);

	/// <summary>
	/// ポイ捨て衝撃波発射
	/// </summary>
	void LitteringShockWaveStart(XMFLOAT3 pos);

	/// <summary>
	/// 巨大衝撃波発射
	/// </summary>
	void BigShockWaveStart(XMFLOAT3 pos);

	/// <summary>
	/// プレイヤーポイ捨て弾発射
	/// </summary>
	void ShotPlayerBullet();

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
	/// カメラ距離変更をセット
	/// </summary>
	/// <param name="distance">距離</param>
	void SetChangeCameraDistance(float distance);

private:// メンバ変数
	//音
	Audio* audio = nullptr;
	//ライト
	LightGroup* light = nullptr;

	//モデル
	Model* circleModel = nullptr;//タバコのモデル
	Model* pBodyModel = nullptr;//プレイヤーの体のモデル

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

	Model* eBullModel = nullptr;//敵の弾のモデル
	Model* hexagonModel = nullptr;//六角形のモデル
	Model* happyModel = nullptr;//タバコモデル
	Model* portaModel = nullptr;//ポルタのモデル
	Model* charoModel = nullptr;//チャロのモデル
	Model* frameModel = nullptr;//フレームのモデル
	Model* waveModel = nullptr;//衝撃波のモデル

	//プレイヤー
	Player* player = nullptr;
	//衝撃波(0番：プレイヤー、1,2,3番：ポイ捨て、4番：巨大)
	static const int shockWaveNum = 5;
	ShockWave* shockWave[shockWaveNum] = { nullptr };
	//プレイヤー弾
	static const int playerBulletNum = 3;
	PlayerBullet* playerBullet[playerBulletNum] = { nullptr };
	//着弾地点
	LandingPoint* landingPoint = nullptr;

	//敵
	std::list <BaseEnemy*>enemys;

	//カメラ距離
	XMFLOAT3 cameraPos = { 0, 0, -200 };
	//カメラ距離イージング開始
	float cameraDisEaseStart = 0;
	//カメラ距離イージング終了
	float cameraDisEaseEnd = 0;
	//カメラ距離を変更中か
	bool isChangecameraDis = false;
	//カメラ距離イージングタイマー
	int cameraDisEaseTimer = 0;
	//画面シェイクスイッチ
	bool isShake = false;
	//画面シェイク時間
	int ShakeTime = 0;

	//壁
	Wall* wall = nullptr;

	//エフェクト
	StageEffect* effects = nullptr;

	//背景
	BuckGround* buckGround = nullptr;

	//コンボ
	Combo* combo = nullptr;
	//制限時間
	TimeLimit* timeLimit = nullptr;
	//壊したスコア
	BreakScore* breakScore = nullptr;
	//巨大衝撃波用ゲージ
	BigShockWaveGauge* shockWaveGauge = nullptr;
	//制限時間回復用ゲージ
	TimeLimitGauge* timeLimitGauge = nullptr;
};
