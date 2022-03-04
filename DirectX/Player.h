#pragma once
#include "Sprite.h"

class Player
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
	/// プレイヤー生成
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <param name="position">初期座標</param>
	/// <param name="size">大きさ</param>
	/// <returns>プレイヤー</returns>
	static Player *Create(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size);
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <param name="position">座標</param>
	/// <param name="size">大きさ</param>
	/// <returns>成否</returns>
	bool Initialize(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	XMFLOAT2 GetPosition() { return playerSprite->GetPosition(); }
	float GetRotation() { return playerSprite->GetRotation(); }

private:
	//プレイヤースプライト
	Sprite *playerSprite = nullptr;
};

