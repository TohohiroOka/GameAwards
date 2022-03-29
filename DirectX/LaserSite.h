#pragma once
#include "DrawLine3D.h"

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
	static LaserSite *Create();

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LaserSite();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Camera *camera);

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
	//線
	DrawLine3D *line = nullptr;
	//線の始点
	XMFLOAT3 startPoint = {};
	//線の終点
	XMFLOAT3 endPoint = {};
	//線の色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//線の太さ
	float weight = 1;
};