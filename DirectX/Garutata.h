#pragma once
#include "BaseEnemy.h"

class Garutata : public BaseEnemy
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
	/// ガルタタ生成
	/// </summary>
	/// <param name="garutataModel">モデル</param>
	/// <param name="stayPointModel">停止座標モデル</param>
	/// <param name="spawnPosition">スポーン時座標</param>
	/// <param name="stayPosition">停止座標</param>
	/// <returns>ガルタタ</returns>
	static Garutata *Create(Model *garutataModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="enemyModel">モデル</param>
	/// <param name="stayPointModel">停止座標モデル</param>
	/// <param name="spawnPosition">スポーン時座標</param>
	/// <param name="stayPosition">停止座標</param>
	/// <returns>成否</returns>
	bool Initialize(Model *enemyModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition) override;
};