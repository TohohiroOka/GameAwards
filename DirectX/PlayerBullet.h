#pragma once
#include "Object3d.h"

class PlayerBullet
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
	/// プレイヤー弾生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>プレイヤー弾</returns>
	static PlayerBullet* Create(Model* model = nullptr);

	/// <summary>
	/// 削除する座標をセット
	/// </summary>
	/// <param name="deadPos">削除する座標</param>
	static void SetDeadPos(XMFLOAT2 deadPos) { PlayerBullet::deadPos = deadPos; }

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerBullet();

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
	/// 弾発射
	/// </summary>
	/// <param name="position">開始座標</param>
	/// <param name="rotation">弾の角度</param>
	void BulletStart(XMFLOAT3 position, float angle);

	/// <summary>
	/// 弾死亡
	/// </summary>
	void Dead();

	//getter
	XMFLOAT3 GetPosition() { return bulletObject->GetPosition(); }
	float GetAngle() { return angle; }
	bool GetIsAlive() { return isAlive; };
	bool GetIsShockWaveStart() { return isShockWaveStart; }

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 生存時間更新
	/// </summary>
	void AliveTimeUpdate();

private:
	//削除する座標
	static XMFLOAT2 deadPos;

private:
	//弾オブジェクト
	Object3d* bulletObject = nullptr;
	//発射角度
	float angle = 0.0f;
	//弾の生存時間タイマー
	int aliveTimer = 0;
	//弾が生きているか
	bool isAlive = false;
	//衝撃波を発射するか
	bool isShockWaveStart = false;
};
