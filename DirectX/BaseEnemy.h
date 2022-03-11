#pragma once
#include "Object3d.h"

class BaseEnemy
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
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>成否</returns>
	virtual bool Initialize(Model *model, XMFLOAT3 position) = 0;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update() = 0;

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
	/// ノックバックの情報をセット
	/// </summary>
	/// <param name="angle">角度</param>
	/// <param name="power">ノックバックの強さ</param>
	void SetKnockBack(float angle, int power);

	//getter
	XMFLOAT3 GetPosition() { return enemyObject->GetPosition(); }
	XMFLOAT3 GetScale() { return enemyObject->GetScale(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsExistence() { return isExistence; }
	int GetKillBulletPower() { return killBulletPower; }

protected:
	//敵スプライト
	Object3d *enemyObject = nullptr;
	//発射角度
	float angle = 0.0f;
	//体力
	int HP = 20;
	//生きているか
	bool isAlive = true;
	//存在しているか(ノックバックも終了)
	bool isExistence = true;
	//ノックバックタイマー
	int knockBackTimer = 0;
	//ノックバックの角度
	float knockBackAngle = 0.0f;
	//ノックバックの強さ
	int killBulletPower = 0;
};