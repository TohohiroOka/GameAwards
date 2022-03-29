#pragma once
#include "Object3d.h"

class FixedObject
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
	/// 固定オブジェクト生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">座標</param>
	/// <returns>固定オブジェクト</returns>
	static FixedObject *Create(Model *model, XMFLOAT3 position);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FixedObject();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">座標</param>
	/// <returns>成否</returns>
	bool Initialize(Model *model, XMFLOAT3 position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	XMFLOAT3 GetPosition() { return fixedObject->GetPosition(); }
	XMFLOAT3 GetScale() { return fixedObject->GetScale(); }

private:
	//固定オブジェクト
	Object3d *fixedObject = nullptr;
};