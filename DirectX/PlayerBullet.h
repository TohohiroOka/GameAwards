#pragma once
#include "Sprite.h"

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
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>プレイヤー弾</returns>
	static PlayerBullet *Create(UINT texNumber);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>成否</returns>
	bool Initialize(UINT texNumber);

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
	void BulletStart(XMFLOAT2 position, float rotation);

	/// <summary>
	/// 弾死亡
	/// </summary>
	void Dead();

	//getter
	XMFLOAT2 GetPosition() { return bulletSprite->GetPosition(); }
	XMFLOAT2 GetSize() { return bulletSprite->GetSize(); }
	int GetPower() { return power; }
	bool GetIsAlive() { return isAlive; };

private:
	//弾スプライト
	Sprite *bulletSprite = nullptr;
	//発射角度
	float angle = 0.0f;
	//弾の強さ
	int power = 10;
	//弾が生きているか
	bool isAlive = false;
};
