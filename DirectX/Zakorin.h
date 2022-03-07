#pragma once
#include "BaseEnemy.h"

class Zakorin : public BaseEnemy
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
	/// ザコリン生成
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>ザコリン</returns>
	static Zakorin *Create(Model *model, XMFLOAT3 position);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>成否</returns>
	bool Initialize(Model *model, XMFLOAT3 position) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

private:
};