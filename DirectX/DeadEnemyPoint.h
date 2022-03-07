#pragma once
#include "Object3d.h"

class DeadEnemyPoint
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
	/// 敵の死亡位置生成
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>敵の死亡位置</returns>
	static DeadEnemyPoint *Create(Model *model, XMFLOAT3 position, float radius);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~DeadEnemyPoint();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>成否</returns>
	bool Initialize(Model *model, XMFLOAT3 position, float radius);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	XMFLOAT3 GetPosition() { return deadPointObject->GetPosition(); }
	float GetRadius() { return radius; }

private:
	//死んだ位置オブジェクト
	Object3d *deadPointObject = nullptr;
	//円の半径
	float radius;
};