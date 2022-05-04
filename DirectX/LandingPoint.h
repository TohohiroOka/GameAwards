#pragma once
#include "Object3d.h"
#include "Player.h"

class LandingPoint
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
	/// 着弾地点生成
	/// </summary>
	/// <returns>着弾地点</returns>
	static LandingPoint* Create(Model* model);

	static void SetMoveRange(XMFLOAT2 moveRange) { LandingPoint::moveRange = moveRange; }

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LandingPoint();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(XMFLOAT3 playerPosition, XMFLOAT3 playerRotation);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 座標更新
	/// </summary>
	/// <param name="weaponPosition">プレイヤーの座標</param>
	/// <param name="weaponRotation">プレイヤーの角度</param>
	void SetPosition(XMFLOAT3 position, XMFLOAT3 rotation);

private:
	static XMFLOAT2 moveRange;

private:
	//着弾地点オブジェクト
	static const int pointNum = 3;
	Object3d* pointCircleObject[pointNum] = { nullptr };
};