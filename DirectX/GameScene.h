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
#include "HageEnemy.h"
#include "EnemyBullet.h"
#include "FixedObject.h"
#include "ConnectCircle.h"
#include "PowerUpLine.h"
#include "StageEffect.h"

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
	/// 敵(ガル族)を生成
	/// </summary>
	void SpawnGaruEnemy();

	/// <summary>
	/// 敵(ハゲ族)を生成
	/// </summary>
	void SpawnHageEnemy();

	/// <summary>
	/// 固定オブジェクトをセット
	/// </summary>
	void SetFixedObject();

	/// <summary>
	/// パワーアップ線を作成
	/// </summary>
	void CreatePowerUpLine(ConnectCircle *startPoint, ConnectCircle *endPoint);

private:// メンバ変数
	//音
	Audio *audio = nullptr;
	//ライト
	LightGroup *light = nullptr;
	//スプライト
	Sprite *sprite = nullptr;

	//モデル
	Model *circleModel = nullptr;//タバコのモデル
	Model *playerModel = nullptr;//プレイヤーのモデル
	Model *pBodyModel = nullptr;//プレイヤーの体のモデル
	Model *pHead01Model = nullptr;//プレイヤーの頭のモデル(HP1)
	Model *pHead02Model = nullptr;//プレイヤーの頭のモデル(HP2)
	Model *pHead03Model = nullptr;//プレイヤーの頭のモデル(HP3)
	Model *pBullModel = nullptr;//プレイヤーの弾のモデル
	Model *enemy01Model = nullptr;//敵01(ガルタ)のモデル
	Model *enemyPoint01Model = nullptr;//敵01(ガルタ)の出現位置のモデル
	Model *enemy02Model = nullptr;//敵02(ガルタタ)のモデル
	Model *enemyPoint02Model = nullptr;//敵02(ガルタタ)の出現位置のモデル
	Model *eBullModel = nullptr;//敵の弾のモデル
	Model *deadEnemyModel = nullptr;//死んだ敵のモデル
	Model *hexagonModel = nullptr;//六角形のモデル
	Model *happyModel = nullptr;//タバコモデル

	//プレイヤー
	Player *player = nullptr;
	//プレイヤー弾
	static const int playerBulletNum = 10;
	PlayerBullet *playerBullet[playerBulletNum] = { nullptr };
	//弾の発射間隔
	int bulletShotTimer = 10;
	//レーザーサイト
	LaserSite *laserSite = nullptr;

	//敵(ガル族)
	std::list <GaruEnemy *>garuEnemys;
	//敵(ハゲ族)
	std::list <HageEnemy *>hageEnemys;
	//敵の弾
	static const int enemyBulletNum = 100;
	EnemyBullet *enemyBullet[enemyBulletNum] = { nullptr };

	//固定オブジェクト
	std::list <FixedObject *> fixedObjects;

	//コネクトサークル
	std::list <ConnectCircle *> connectCircles;

	//パワーアップ線
	std::list <PowerUpLine *> powerUpLines;

	//画面シェイクスイッチ
	bool isShake = false;
	//画面シェイク時間
	int ShakeTime = 0;

	//エフェクト
	StageEffect* effects = nullptr;
};
