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

	/// <summary>
	/// 制限時間回復
	/// </summary>
	/// <param name="second"></param>
	void Recovery(int second);

	/// <summary>
	/// ゲームシーンの座標に移動状態にセット
	/// </summary>
	void SetMoveGamePos();

	/// <summary>
	/// リザルトシーンの座標に移動状態にセット
	/// </summary>
	void SetMoveResultPos();

	//setter
	void SetIsCountDown(bool isCountDown) { this->isCountDown = isCountDown; }

	//getter
	int GetTime() { return timer; }
	bool GetIsMoveGamePosEnd() { return isMoveGamePosEnd; }

private:
	/// <summary>
	/// カウントダウン
	/// </summary>
	void CountDown();

	/// <summary>
	/// 表示用制限時間スプライト変更
	/// </summary>
	void ChangeTimeSprite();

	/// <summary>
	/// ゲームシーンの座標に移動
	/// </summary>
	void MoveGamePos();

	/// <summary>
	/// リザルトシーンの座標に移動
	/// </summary>
	void MoveResultPos();

private:
	//制限時間スプライト
	static const int timeDigits = 2;
	Sprite* timeSprite[timeDigits] = { nullptr };
	//制限時間最大
	const int timeLimitMax = 3600;
	//時間計測タイマー
	int timer = timeLimitMax;
	//カウントダウンするか
	bool isCountDown = false;
	//ゲームシーンの座標に移動中か
	bool isMoveGamePos = false;
	//ゲームシーンの座標に移動終了したか
	bool isMoveGamePosEnd = false;
	//ゲームシーンの座標に移動する時間タイマー
	int moveGamePosTimer = 0;
	//リザルトシーンの座標に移動中か
	bool isMoveResultPos = false;
	//リザルトシーンの座標に移動終了したか
	bool isMoveResultPosEnd = false;
	//リザルトシーンの座標に移動する時間タイマー
	int moveResultPosTimer = 0;
};