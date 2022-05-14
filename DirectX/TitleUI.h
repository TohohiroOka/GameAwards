#pragma once
#include "Object3d.h"

class TitleUI
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
	/// タイトルシーンのUI生成
	/// </summary>
	/// <param name="RBModel">RBボタンオブジェクトに使用するモデル</param>
	/// <returns>タイトルシーンのUI</returns>
	static TitleUI* Create(Model* RBModel);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleUI();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model* RBModel);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="playerPos">追従するプレイヤー座標</param>
	void Update(XMFLOAT3 playerPos);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//setter
	void SetIsDraw(bool isDraw) { this->isDraw = isDraw; };

private:
	//RBボタンスオブジェクト
	Object3d* RBObject = nullptr;
	//描画するか
	bool isDraw = false;
};