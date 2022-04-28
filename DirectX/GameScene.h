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
#include "GaruEnemy.h"
#include "Charo.h"
#include "Porta.h"
#include "BossEnemy.h"
#include "EnemyBullet.h"
#include "FixedEnemy.h"
#include "StageEffect.h"
#include "Frame.h"
#include "ShockWave.h"
#include "BuckGround.h"

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
	/// 衝撃波発射
	/// </summary>
	/// <param name="shockWavePower">衝撃波の威力</param>
	void ShockWaveStart(const int shockWavePower);

	/// <summary>
	/// 敵(ガル族)を生成
	/// </summary>
	void SpawnGaruEnemy(int spawnPattern, int wave);

	/// <summary>
	/// ガル族の弾発射を管理
	/// </summary>
	/// <param name="garuEnemy">ガル族</param>
	void GaruEnemyShotBullet(GaruEnemy* garuEnemy);

	/// <summary>
	/// 敵(チャロ・ポルタ)を生成
	/// </summary>
	void SpawnCharoPorta(int spawnPattern, int wave);

	/// <summary>
	/// ボス戦開始
	/// </summary>
	void BossStageStart();

	/// <summary>
	///	ボスの弾発射を管理
	/// </summary>
	/// <param name="moveBossNumber">動いているボス</param>
	void BossEnemyShotBullet(int moveBossNumber);

	/// <summary>
	/// ボスの攻撃の衝撃により敵を降らせる
	/// </summary>
	void BossImpactFallEnemy();

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
	Model* playerModel = nullptr;//プレイヤーのモデル
	Model* pBodyModel = nullptr;//プレイヤーの体のモデル
	Model* pHead01Model = nullptr;//プレイヤーの頭のモデル(HP1)
	Model* pHead02Model = nullptr;//プレイヤーの頭のモデル(HP2)
	Model* pHead03Model = nullptr;//プレイヤーの頭のモデル(HP3)
	Model* enemy01Model = nullptr;//敵01(ガルタ)のモデル
	Model* enemyPoint01Model = nullptr;//敵01(ガルタ)の出現位置のモデル
	Model* enemy02Model = nullptr;//敵02(ガルタタ)のモデル
	Model* enemyPoint02Model = nullptr;//敵02(ガルタタ)の出現位置のモデル
	Model* eBullModel = nullptr;//敵の弾のモデル
	Model* deadEnemyModel = nullptr;//死んだ敵のモデル
	Model* initialCircleCoreModel = nullptr;//固定敵のコアのモデル
	Model* initialCircleSquareModel = nullptr;//固定敵の外枠のモデル
	Model* hexagonModel = nullptr;//六角形のモデル
	Model* happyModel = nullptr;//タバコモデル
	Model* portaModel = nullptr;//ポルタのモデル
	Model* charoModel = nullptr;//チャロのモデル
	Model* tuffModel = nullptr;//タッフのモデル
	Model* frameModel = nullptr;//フレームのモデル
	Model* waveModel = nullptr;//衝撃波のモデル
	Model* RBbuttonModel = nullptr;//RBボタンのモデル

	//プレイヤー
	Player* player = nullptr;
	//衝撃波
	static const int shockWaveNum = 10;
	ShockWave* shockWave[shockWaveNum] = { nullptr };


	//敵(ガル族)
	std::list <GaruEnemy*>garuEnemys;
	//敵(チャロ)
	std::list <Charo*>charoEnemys;
	//敵(ポルタ)
	std::list <Porta*>portaEnemys;
	//ボス
	static const int bossNum = 1;
	BossEnemy* bossEnemy[bossNum] = { nullptr };
	//ボス戦中
	bool isBossStage = false;
	//動かすボス
	int moveBossNumber;
	//敵の弾
	static const int enemyBulletNum = 100;
	EnemyBullet* enemyBullet[enemyBulletNum] = { nullptr };


	//固定敵
	std::list <FixedEnemy*> fixedEnemys;

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

	//画面枠
	Frame* frame = nullptr;


	//スポーンパターン
	bool isSpawnTimer = false;//スポーンタイマーのカウントをするかしないか
	int spawnTimer = 0;//スポーンタイマー(敵の出現用のカウント)
	int spawnInterval = 300;//スポーン間隔のフレーム数
	int spawnPattern = 0;//スポーンパターン(敵パターン計算用)
	int spawnSet = 0;//スポーンの乱数用

	int wave = 1;

	//エフェクト
	StageEffect* effects = nullptr;

	//背景
	BuckGround* buckGround = nullptr;
};
