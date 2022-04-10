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
	/// <param name="model">モデル</param>
	/// <returns>敵弾</returns>
	static EnemyBullet* Create(Model* model);

	/// <summary>
	/// 削除する座標をセット
	/// </summary>
	/// <param name="deadPos">削除する座標</param>
	static void SetDeadPos(XMFLOAT2 deadPos) { EnemyBullet::deadPos = deadPos; }

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ターゲット狙い弾発射
	/// </summary>
	/// <param name="position">開始座標</param>
	/// <param name="targetPosition">ターゲットの座標</param>
	/// <param name="moveSpeed">弾の移動速度</param>
	void AimBulletStart(XMFLOAT3 position, XMFLOAT3 targetPosition, float moveSpeed = 0.5f);

	/// <summary>
	/// 直進弾発射
	/// </summary>
	/// <param name="position">開始座標</param>
	/// <param name="angle">弾の角度</param>
	/// <param name="moveSpeed">弾の移動速度</param>
	void StraightBulletStart(XMFLOAT3 position, float angle, float moveSpeed = 0.5f);

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
	//削除する座標
	static XMFLOAT2 deadPos;

private:
	//弾オブジェクト
	Object3d* bulletObject = nullptr;
	//移動速度
	float moveSpeed = 0;
	//発射角度
	float angle = 0.0f;
	//弾が生きているか
	bool isAlive = false;
};
