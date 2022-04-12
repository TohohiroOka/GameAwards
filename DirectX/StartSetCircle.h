#pragma once
#include "ConnectCircle.h"

class StartSetCircle : public ConnectCircle
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
	/// 初期化時セットコネクトサークル生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="FixedEnemy">固定敵</param>
	/// <param name="radius">固定敵</param>
	/// <returns>コネクトサークル</returns>
	static StartSetCircle* Create(Model* model, FixedEnemy* enemy, float radius);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="FixedEnemy">固定敵</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model, FixedEnemy* enemy, float radius);
};