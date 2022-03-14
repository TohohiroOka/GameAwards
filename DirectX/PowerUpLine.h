#pragma once
#include "DrawLine3D.h"
#include "DeadEnemyPoint.h"
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
	static PowerUpLine *Create(DeadEnemyPoint* startPoint, DeadEnemyPoint* endPoint);

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
	bool Initialize(DeadEnemyPoint* startPoint, DeadEnemyPoint* endPoint);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Camera *camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 既に線が作られているかをチェック
	/// </summary>
	bool IsAlreadyCreateLine(DeadEnemyPoint* startPoint, DeadEnemyPoint* endPoint);

	void SetColor(XMFLOAT4 color);
	//getter
	XMFLOAT3 GetStartPoint() { return startPoint->GetPosition(); }
	XMFLOAT3 GetEndPoint() { return endPoint->GetPosition(); }

private:
	//線
	DrawLine3D* line = nullptr;
	//線の始点
	DeadEnemyPoint* startPoint = nullptr;
	//線の終点
	DeadEnemyPoint* endPoint = nullptr;
	//線の太さ
	float weight = 0.5f;
};