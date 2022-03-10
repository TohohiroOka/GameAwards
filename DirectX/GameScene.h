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
#include "Zakorin.h"
#include "DeadEnemyPoint.h"
#include "PowerUpLine.h"

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
	void Initialize(Input *input, Camera *camera);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="camera">カメラクラスのインスタンス</param>
	void Update(Camera *camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList *cmdList);

private:// メンバ変数
	//入力
	Input *input = nullptr;
	//音
	Audio *audio = nullptr;
	//ライト
	LightGroup *light = nullptr;
	//スプライト
	Sprite *sprite = nullptr;

	//モデル
	Model *circleModel = nullptr;

	//プレイヤー
	Player *player = nullptr;
	//プレイヤー弾
	static const int playerBulletNum = 10;
	PlayerBullet *playerBullet[playerBulletNum] = { nullptr };

	//敵
	static const int enemyNum = 4;
	BaseEnemy *enemy[enemyNum] = { nullptr };

	//死んだ敵の位置
	//BaseEnemy *deadEnemy[enemyNum] = { nullptr };
	std::list <DeadEnemyPoint *> deadEnemyPoints;
	//死んだ敵の位置の数
	int oldDeadPointNum = 0;

	//パワーアップ線
	//static const int powerUpLineNum = 3;
	std::list <PowerUpLine *> powerUpLines;
	float cameraZ = -100;
};
