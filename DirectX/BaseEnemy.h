#pragma once
#include "Sprite.h"

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
	virtual bool Initialize(XMFLOAT2 position) = 0;

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


	//getter
	XMFLOAT2 GetPosition() { return enemySprite->GetPosition(); }
	XMFLOAT2 GetSize() { return enemySprite->GetSize(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	static int GetDeadCount() { return deadCount; }
	int GetDeadNum() { return deadNum; }

private:
	static int deadCount;

protected:
	//敵スプライト
	Sprite *enemySprite = nullptr;
	//発射角度
	float angle = 0.0f;
	//体力
	int HP = 100;
	//弾が生きているか
	bool isAlive = true;
	//死亡順
	int deadNum;
};