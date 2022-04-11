#pragma once
#include "Object3d.h"

class LaserSite
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
	/// レーザーサイト生成
	/// </summary>
	/// <returns>レーザーサイト</returns>
	static LaserSite* Create(Model* model);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LaserSite();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 座標更新
	/// </summary>
	/// <param name="weaponPosition">ウエポンの座標</param>
	/// <param name="weaponRotation">ウエポンの角度</param>
	void SetPosition(XMFLOAT3 weaponPosition, XMFLOAT3 weaponRotation);

private:
	//レーザーオブジェクト
	Object3d* razorObject = nullptr;
};