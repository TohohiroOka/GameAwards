#pragma once
#include"Sprite.h"

class UIFrame
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
	/// UIを囲う枠生成
	/// </summary>
	/// <returns>UIを囲う枠</returns>
	static UIFrame* Create(int frameTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~UIFrame();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int frameTexNum);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	//枠スプライト
	Sprite* frameSprite = nullptr;
};