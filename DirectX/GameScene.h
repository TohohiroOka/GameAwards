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
#include "FixedObject.h"
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
	//シーン番号
	enum SceneName
	{
		Title,		//タイトルシーン
		Game,		//ゲームプレイシーン
		ChangeWave,	//ウェーブ変更シーン
		GameOver,	//ゲームオーバーシーン
		Result,		//リザルトシーン
	};
	//タイトルシーン内のシーン番号
	enum TitleSceneName
	{
		SpawnEnemySet,	//タイトルシーン用の敵スポーンをセット
		SpawnEnemy,		//タイトルシーン用の敵スポーン
		CreateConnectCircle,//コネクトサークルを作る
		SpawnPlayerCore,//コアとプレイヤースポーン
		PlayerMove,		//プレイヤー移動可能
		CoreExplosion,	//コア爆発
	};
	//ウェーブ変更シーン内のシーン番号
	enum ChangeWaveSceneName
	{
		WaveUpdate,		//ウェーブ変更情報更新
		PlayerReset,	//プレイヤー初期位置移動シーン
		FrameMove,		//枠オブジェクト移動シーン(画面サイズ3→1)
		CameraMove,		//カメラ移動シーン(画面サイズ3→1)
		FrameCameraMove,//枠オブジェクトとカメラ移動シーン(画面サイズ1→2→3)
	};
	//ゲームオーバーシーン内のシーン番号
	enum GameOverSceneName
	{
		DeletePlayerAndBullets,	//プレイヤーと弾を削除
		ShockWaveMove,			//衝撃波を動かす
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
	void Initialize(Camera *camera);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="camera">カメラクラスのインスタンス</param>
	void Update(Camera *camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList *cmdList);

	/// <summary>
	/// ゲームを初期化
	/// </summary>
	void ResetGame();

	/// <summary>
	/// タイトルシーン用の敵生成
	/// </summary>
	void TitleSceneEnemySpawn();

	/// <summary>
	/// プレイヤー弾発射
	/// </summary>
	void ShotPlayerBullet();

	/// <summary>
	/// 敵生成管理
	/// </summary>
	void SpawnEnemyManager();

	/// <summary>
	/// 敵(ガル族)を生成
	/// </summary>
	void SpawnGaruEnemy(int spawnPattern);

	/// <summary>
	/// ガル族の弾発射を管理
	/// </summary>
	/// <param name="garuEnemy">ガル族</param>
	void GaruEnemyShotBullet(GaruEnemy* garuEnemy);

	/// <summary>
	/// 敵(チャロ・ポルタ)を生成
	/// </summary>
	void SpawnCharoPorta(int spawnPattern);

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
	/// 固定オブジェクトをセット
	/// </summary>
	void SetFixedObject();

	/// <summary>
	/// パワーアップ線を作成
	/// </summary>
	void CreatePowerUpLine(ConnectCircle *startPoint, ConnectCircle *endPoint);

	/// <summary>
	/// カメラ更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	void CameraUpdate(Camera *camera);

	/// <summary>
	/// カメラ距離変更をセット
	/// </summary>
	/// <param name="distance">距離</param>
	void SetChangeCameraDistance(float distance);

private:// メンバ変数
	//音
	Audio *audio = nullptr;
	//ライト
	LightGroup *light = nullptr;
	//スプライト
	Sprite *sprite = nullptr;

	//モデル
	Model* circleModel = nullptr;//タバコのモデル
	Model* playerModel = nullptr;//プレイヤーのモデル
	Model* pBodyModel = nullptr;//プレイヤーの体のモデル
	Model* pHead01Model = nullptr;//プレイヤーの頭のモデル(HP1)
	Model* pHead02Model = nullptr;//プレイヤーの頭のモデル(HP2)
	Model* pHead03Model = nullptr;//プレイヤーの頭のモデル(HP3)
	Model* razorModel = nullptr;//レーザーのモデル
	Model* pBullModel = nullptr;//プレイヤーの弾のモデル
	Model* enemy01Model = nullptr;//敵01(ガルタ)のモデル
	Model* enemyPoint01Model = nullptr;//敵01(ガルタ)の出現位置のモデル
	Model* enemy02Model = nullptr;//敵02(ガルタタ)のモデル
	Model* enemyPoint02Model = nullptr;//敵02(ガルタタ)の出現位置のモデル
	Model* eBullModel = nullptr;//敵の弾のモデル
	Model* deadEnemyModel = nullptr;//死んだ敵のモデル
	Model* hexagonModel = nullptr;//六角形のモデル
	Model* happyModel = nullptr;//タバコモデル
	Model* portaModel = nullptr;//ポルタのモデル
	Model* charoModel = nullptr;//チャロのモデル
	Model* tuffModel = nullptr;//タッフのモデル
	Model* frameModel = nullptr;//フレームのモデル
	Model* waveModel = nullptr;//衝撃波のモデル
	Model* coreModel = nullptr;//コアのモデル

	//プレイヤー
	Player *player = nullptr;
	//プレイヤー弾
	static const int playerBulletNum = 10;
	PlayerBullet *playerBullet[playerBulletNum] = { nullptr };
	//レーザーサイト
	LaserSite *laserSite = nullptr;

	//コア
	Core* core = nullptr;
	//titleの爆破演出出現位置
	XMFLOAT3 explosionPosition = {};

	//敵(ガル族)
	std::list <GaruEnemy *>garuEnemys;
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
	EnemyBullet *enemyBullet[enemyBulletNum] = { nullptr };

	//固定オブジェクト
	std::list <FixedObject *> fixedObjects;

	//コネクトサークル
	std::list <ConnectCircle *> connectCircles;

	//パワーアップ線
	std::list <PowerUpLine *> powerUpLines;

	//カメラ距離
	XMFLOAT3 cameraPos = { 0, 0, -100 };
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

	//衝撃波
	ShockWave* shockWave = nullptr;

	//スポーンパターン
	int spawnTimer = 0;//スポーンタイマー

	//タイトルロゴ
	TitleLogo* titleLogo = nullptr;
	//タイトルシーン用タイマー
	int titleSceneTimer = 0;
	//タイトルシーン用UI
	TitleUI* titleUI = nullptr;

	//リザルトシーン用UI
	ResultUI* resultUI = nullptr;

	//シーン
	int scene = SceneName::Title;
	//タイトルシーン
	int titleScene = TitleSceneName::SpawnEnemySet;
	//ウェーブ変更シーン
	int changeWaveScene = ChangeWaveSceneName::WaveUpdate;
	//ゲームオーバーシーン
	int gameOverScene = GameOverSceneName::DeletePlayerAndBullets;
	//ウェーブ
	int wave = 1;

	//スコア
	Score *score = nullptr;

	//エフェクト
	StageEffect *effects = nullptr;

	//背景
	BuckGround* buckGround = nullptr;
};
