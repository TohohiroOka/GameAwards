#pragma once
#include "HageEnemy.h"

class Hageta : public HageEnemy
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
	/// ハゲタ生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">座標</param>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	/// <returns>ハゲタ</returns>
	static Hageta *Create(Model *model, XMFLOAT3 position, float moveDegree);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">座標</param>
	/// <returns>成否</returns>
	bool Initialize(Model *model, XMFLOAT3 position) override;

	/// <summary>
	/// 移動角度を設定
	/// </summary>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	void SetMoveAngle(float moveDegree);
};