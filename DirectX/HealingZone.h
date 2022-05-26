#pragma once
#include "Object3d.h"

class HealingZone
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
	/// ゲージ回復地点生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ゲージ回復地点</returns>
	static HealingZone* Create(Model* model);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~HealingZone();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
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

	//getter
	XMFLOAT3 GetPosition() { return healingZoneObject->GetPosition(); }
	float GetRadius() { return healingZoneObject->GetScale().x; }

private:
	//回復地点オブジェクト
	Object3d* healingZoneObject = nullptr;
};