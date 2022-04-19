#pragma once
#include"Sprite.h"

class TimeLimit
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
	/// 制限時間生成
	/// </summary>
	/// <returns>制限時間</returns>
	static TimeLimit* Create(int textureNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TimeLimit();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int textureNum);

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

	//getter
	int GetTime() { return timer; }

private:
	/// <summary>
	/// カウントダウン
	/// </summary>
	void CountDown();

	/// <summary>
	/// 表示用制限時間スプライト変更
	/// </summary>
	void ChangeTimeSprite();

private:
	//制限時間スプライト
	static const int timeDigits = 2;
	Sprite* timeSprite[timeDigits] = { nullptr };
	//時間計測タイマー
	int timer = 6000;
};