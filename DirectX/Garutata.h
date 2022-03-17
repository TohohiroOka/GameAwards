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
	/// <param name="model">モデル</param>
	/// <param name="position">座標</param>
	/// <param name="targetPosition">標的の座標</param>
	/// <returns>ガルタタ</returns>
	static Garutata *Create(Model *model, XMFLOAT3 position, XMFLOAT3 targetPosition);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">座標</param>
	/// <returns>成否</returns>
	bool Initialize(Model *model, XMFLOAT3 position) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 追従角度を設定
	/// </summary>
	/// <param name="targetPosition">標的の座標</param>
	void SetMoveAngle(XMFLOAT3 targetPosition);
};