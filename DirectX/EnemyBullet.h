#pragma once
#include "Object3d.h"

class EnemyBullet
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
	/// 敵弾生成
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>敵弾</returns>
	static EnemyBullet *Create(Model *model = nullptr);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>成否</returns>
	bool Initialize(Model *model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾発射
	/// </summary>
	/// <param name="position">開始座標</param>
	/// <param name="targetPosition">標的の座標</param>
	void BulletStart(XMFLOAT3 position, XMFLOAT3 targetPosition);

	/// <summary>
	/// 弾死亡
	/// </summary>
	void Dead();

	//getter
	XMFLOAT3 GetPosition() { return bulletObject->GetPosition(); }
	XMFLOAT3 GetScale() { return bulletObject->GetScale(); }
	float GetAngle() { return angle; }
	bool GetIsAlive() { return isAlive; };

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

private:
	//弾オブジェクト
	Object3d *bulletObject = nullptr;
	//発射角度
	float angle = 0.0f;
	//弾が生きているか
	bool isAlive = false;
};
