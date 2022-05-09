#pragma once
#include"Sprite.h"

class BackGround
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
	/// 背景生成
	/// </summary>
	/// <returns>背景</returns>
	static BackGround* Create(int backGroundTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BackGround();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int backGroundTexNum);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	//背景スプライト
	Sprite* backGroundSprite = nullptr;
};