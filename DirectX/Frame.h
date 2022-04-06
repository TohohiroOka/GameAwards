#pragma once
#include "Object3d.h"

class Frame
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
	/// プレイヤー生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>プレイヤー</returns>
	static Frame *Create(Model *model = nullptr);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Frame();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>成否</returns>
	bool Initialize(Model *model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 枠のラインの位置と枠オブジェクトの大きさ変更をセット
	/// </summary>
	/// <param name="nextFrameNum">次の画面サイズ番号</param>
	void SetChangeFrameLine(char nextFrameNum);

	/// <summary>
	/// 枠のラインの位置変更
	/// </summary>
	void ChangeFrameLine();

	//getter
	XMFLOAT2 GetFrameLine() { return frameLine; }
	bool GetIsChangeFrameLine() { return isChangeFrameLine; }

private:
	//枠オブジェクト
	Object3d *frameObject = nullptr;
	//画面上で見たときの枠のラインの位置
	XMFLOAT2 frameLine = { 100, 55 };
	//枠オブジェクトの大きさイージング開始
	XMFLOAT3 frameScaleEaseStart = {};
	//枠オブジェクトの大きさイージング終了
	XMFLOAT3 frameScaleEaseEnd = {};
	//枠のラインの位置イージング開始
	XMFLOAT2 frameLineEaseStart = {};
	//枠のラインの位置イージング終了
	XMFLOAT2 frameLineEaseEnd = {};
	//枠のラインの位置を変更中か
	bool isChangeFrameLine = false;
	//枠のラインの位置イージングタイマー
	int frameLineEaseTimer = 0;
};