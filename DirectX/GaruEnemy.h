#pragma once
#include "Object3d.h"

class GaruEnemy
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	enum GaruGroup
	{
		Garuta,		//ガルタ
		Garutata,	//ガルタタ
	};

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GaruEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="enemyModel">モデル</param>
	/// <param name="stayPointModel">停止座標モデル</param>
	/// <param name="spawnPosition">スポーン時の座標</param>
	/// <param name="stayPosition">停止座標</param>
	/// <returns>成否</returns>
	virtual bool Initialize(Model* enemyModel, Model* stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition) = 0;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	void Damage(int damagePower);

	/// <summary>
	/// 死亡
	/// </summary>
	void Dead();

	/// <summary>
	/// 削除
	/// </summary>
	void SetDelete();

	/// <summary>
	/// ノックバックの情報をセット
	/// </summary>
	/// <param name="angle">吹っ飛ぶ角度</param>
	/// <param name="power">ノックバックの強さ</param>
	void SetKnockBack(float angle, int powerLevel);

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { enemyObject->SetModel(model); }

	/// <summary>
	/// 枠にぶつかったか判定
	/// </summary>
	/// <returns>枠にぶつかったか</returns>
	bool IsCollisionFrame(XMFLOAT2 frameLine);

	//getter
	int GetGroup() { return group; }
	XMFLOAT3 GetPosition() { return enemyObject->GetPosition(); }
	XMFLOAT3 GetScale() { return enemyObject->GetScale(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsEscape() { return isEscape; }
	bool GetIsDelete() { return isDelete; }
	bool GetIsDuringSpawn() { return isDuringSpawn; }
	bool GetIsBulletShot() { return isBulletShot; }

protected:
	/// <summary>
	/// スポーン
	/// </summary>
	void Spawn();

	/// <summary>
	/// 弾を発射
	/// </summary>
	void ShotBullet();

	/// <summary>
	/// ノックバック
	/// </summary>
	void KnockBack();

	/// <summary>
	/// 逃走
	/// </summary>
	void Escape();

protected:
	//枠のライン
	static XMFLOAT2 frameLine;

protected:
	//どちらに属しているか
	int group;
	//敵オブジェクト
	Object3d* enemyObject = nullptr;
	//停止座標オブジェクト
	Object3d* stayPointObject = nullptr;
	//スポーン時座標
	XMFLOAT3 spawnPosition = { 0, 0, 0 };
	//移動後の座標
	XMFLOAT3 stayPosition = { 0, 0, 0 };
	//体力
	int HP = 40;
	//生きているか
	bool isAlive = true;
	//生きた時間タイマー
	int aliveTimer = 0;
	//一定時間放置され逃走するか
	bool isEscape = false;
	//逃走時間計測用タイマー
	int escapeTimer = 0;
	//ノックバック時間
	int effectCount = 0;
	//スポーン中か
	bool isDuringSpawn = true;
	//スポーンタイマー
	int spawnTimer = 0;
	//ノックバックタイマー
	int knockBackTimer = 0;
	//ノックバックの角度
	float knockBackAngle = 0.0f;
	//ノックバックの強さ
	int knockBackPowerLevel = 0;
	//ノックバックしているか
	bool isKnockBack = false;
	//弾を発射するか
	bool isBulletShot = false;
	//弾発射からの時間
	int bulletShotTimer = 0;
	//削除するか
	bool isDelete = false;
};