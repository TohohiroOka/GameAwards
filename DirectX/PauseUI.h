#pragma once
#include"Sprite.h"

class PauseUI
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
	/// タイトルロゴ生成
	/// </summary>
	/// <returns>タイトルロゴ</returns>
	static PauseUI* Create(int plainTexNum, int pauseTexNum, int selectBackTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PauseUI();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int plainTexNum, int pauseTexNum, int selectBackTexNum);

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
	/// 確定させる
	/// </summary>
	void SetSelect() { isSelect = true; };

	//getter
	bool GetIsBackGame() { return isBackGame; }
	bool GetIsSelect() { return isSelect; }

private:
	/// <summary>
	/// ゲームに戻るかタイトルシーンに戻るか
	/// </summary>
	void SelectBack();

private:
	//背景用スプライト
	Sprite* backSprite = nullptr;
	//ポーズスプライト
	Sprite* pauseSprite = nullptr;
	//戻るシーン選択スプライト
	Sprite* selectBackSprite = nullptr;
	//ゲームに戻るか
	bool isBackGame = true;
	//選択したか
	bool isSelect = false;
};