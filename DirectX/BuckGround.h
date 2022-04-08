#pragma once
#include "Object3d.h"

class BuckGround
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private://静的メンバ変数

	static const int widthX = 13;
	static const int widthY = 8;
	static const int arreyNum = widthX * widthY;

private://メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize(Model* model);

public://メンバ関数

	BuckGround() {};
	~BuckGround();

	/// <summary>
	/// 解放処理
	/// </summary>
	void AllDelete();

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	BuckGround* Create(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private://メンバ変数

	static std::vector<Object3d*> buckGround;
};
