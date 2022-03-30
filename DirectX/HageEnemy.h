#pragma once
#include "Object3d.h"

class HageEnemy
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
	virtual ~HageEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">座標</param>
	/// <returns>成否</returns>
	virtual bool Initialize(Model *model, XMFLOAT3 position) = 0;

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
	/// モデルをセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model *model) { enemyObject->SetModel(model); }

	//getter
	XMFLOAT3 GetPosition() { return enemyObject->GetPosition(); }
	XMFLOAT3 GetScale() { return enemyObject->GetScale(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsInScreen() { return isInScreen; }
	bool GetIsDelete() { return isDelete; }
	bool GetIsBulletShot() { return isBulletShot; }

protected:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 弾を発射
	/// </summary>
	void ShotBullet();

protected:
	//敵オブジェクト
	Object3d *enemyObject = nullptr;
	//移動角度
	float moveAngle = 0.0f;
	//体力
	int HP = 20;
	//生きているか
	bool isAlive = true;
	//弾を発射するか
	bool isBulletShot = false;
	//画面内にいるか
	bool isInScreen = true;
	//弾発射からの時間
	int bulletShotTimer = 0;
	//削除するか
	bool isDelete = false;
};

