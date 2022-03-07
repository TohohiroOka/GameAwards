#pragma once
#include "DrawLine3D.h"
#include "Camera.h"

class PowerUpLine
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
	/// パワーアップ線生成
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>パワーアップ線</returns>
	static PowerUpLine *Create(XMFLOAT3 startPoint, XMFLOAT3 endPoint);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PowerUpLine();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>成否</returns>
	bool Initialize(XMFLOAT3 startPoint, XMFLOAT3 endPoint);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Camera *camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetColor(XMFLOAT4 color);
	//getter
	XMFLOAT3 GetStartPoint() { return startPoint; }
	XMFLOAT3 GetEndPoint() { return endPoint; }

private:
	//線
	//DrawFunction *line = nullptr;
	//DrawLine* line = nullptr;
	DrawLine3D* line = nullptr;
	//線の始点
	XMFLOAT3 startPoint = { 0, 0, 0 };
	//線の終点
	XMFLOAT3 endPoint = { 0, 0, 0 };
};