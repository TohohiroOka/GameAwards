#pragma once
#include "Object3d.h"

class BossEnemy
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
	enum BossName
	{
		Tuff,	//タッフ
	};

public:
	/// <summary>
	/// 枠のラインをセット
	/// </summary>
	/// <param name="frameLine">枠のライン</param>
	static void SetFrameLine(XMFLOAT2 frameLine) { BossEnemy::frameLine = frameLine; }

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BossEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>成否</returns>
	virtual bool Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="targetPosition">標的の座標</param>
	virtual void Update(XMFLOAT3 targetPosition) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// リセット
	/// </summary>
	virtual void Reset();

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	virtual void Damage(int damagePower);

	/// <summary>
	/// 死亡
	/// </summary>
	virtual void Dead();

	/// <summary>
	/// 死亡してサイズを変更状態をセット
	/// </summary>
	void SetDeadChangeScale();

	/// <summary>
	/// 強い衝撃を与える瞬間か
	/// </summary>
	bool TriggerImpact();

	//getter
	XMFLOAT3 GetPosition() { return bossObject->GetPosition(); }
	XMFLOAT3 GetScale() { return bossObject->GetScale(); }
	int GetHP() { return HP; }
	int GetAction() { return action; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsDuringSpawn() { return isDuringSpawn; }
	bool GetIsBulletShot() { return isBulletShot; }
	bool GetIsExistence() { return isExistence; }

protected:
	/// <summary>
	/// スポーン
	/// </summary>
	virtual void Spawn() = 0;

	/// <summary>
	/// 死亡してサイズを変更
	/// </summary>
	void DeadChangeScale();

protected:
	//枠のライン
	static XMFLOAT2 frameLine;

protected:
	//名前
	int name;
	//ボスオブジェクト
	Object3d* bossObject = nullptr;
	//体力
	int HP = 2000;
	//行動
	int action = 0;
	//行動シーン
	int actionScene = 0;
	//次の行動シーンにするか
	bool isChangeActionScene = false;
	//行動した時間を計測するタイマー
	int actionTimer = 0;
	//生きているか
	bool isAlive = true;
	//スポーン中か
	bool isDuringSpawn = true;
	//スポーンタイマー
	int spawnTimer = 0;
	//弾を発射するか
	bool isBulletShot = false;
	//何回弾を発射したか
	int shotCount = 0;
	//弾発射からの時間
	int bulletShotTimer = 0;
	//強い衝撃を与えるか
	bool isImpact = false;
	//死亡してサイズを変更状態か
	bool isDeadChangeScale = false;
	//膨張前のサイズ
	float changeStartScale = 0;
	//膨張後のサイズ
	float changeEndScale = 0;
	//サイズ変更シーン
	int changeScaleScene = 0;
	//サイズ変更タイマー
	int changeScaleTimer = 0;
	//存在しているか(サイズが0になってたら完全消滅)
	bool isExistence = true;
};