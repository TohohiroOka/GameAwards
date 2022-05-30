#pragma once
#include"Sprite.h"

class Rush
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
	///	ラッシュ用UI生成
	/// </summary>
	/// <returns>タイトル画面用UI</returns>
	static Rush* Create(int rushTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Rush();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int rushTexNum);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 画面内に移動状態にセット
	/// </summary>
	void SetMoveInScreen();

	/// <summary>
	/// 画面外に移動状態にセット
	/// </summary>
	void SetMoveOutScreen();

	//getter
	bool GetIsInScreen() { return isInScreen; }


private:
	/// <summary>
	/// 画面内に移動
	/// </summary>
	void MoveInScreen();

	/// <summary>
	/// 画面外に移動
	/// </summary>
	void MoveOutScreen();

	/// <summary>
	/// 色変更
	/// </summary>
	void ChangeColor();

private:
	//ラッシュスプライト
	Sprite* rushSprite = nullptr;
	//画面内に移動中か
	bool isMoveInScreen = false;
	//画面内に移動する時間タイマー
	int moveInScreenTimer = 0;
	//画面外に移動中か
	bool isMoveOutScreen = false;
	//画面外に移動する時間タイマー
	int moveOutScreenTimer = 0;
	//画面内にいるか
	bool isInScreen = false;
	//色を変えるか
	bool isChangeColor = false;
	//色を変える時間
	int changeColorTimer = 0;
	//薄くするか
	bool isAlphaDown = true;
};