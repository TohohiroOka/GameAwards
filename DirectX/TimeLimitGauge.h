#pragma once
#include"Sprite.h"

class TimeLimitGauge
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
	/// 制限時間回復用ゲージ生成
	/// </summary>
	/// <returns>巨大衝撃波用ゲージ</returns>
	static TimeLimitGauge* Create(int timeTexNum, int frameTexNum, int barTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TimeLimitGauge();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int timeTexNum, int frameTexNum, int barTexNum);

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
	bool GetIsCountDownEnd() { return isCountDownEnd; }
	bool GetIsMoveGamePosEnd() { return isMoveGamePosEnd; }
	int GetTimer() { return timer; }

private:
	/// <summary>
	/// カウントダウン
	/// </summary>
	void CountDown();

	/// <summary>
	/// カウントダウンによるバーの長さ変更
	/// </summary>
	void CountDownLengthBar();

	/// <summary>
	/// 回復中バーの長さを変更
	/// </summary>
	void RecoveryLengthBar();

	/// <summary>
	/// 回復中バーの長さ変更をセット
	/// </summary>
	void SetRecoveryLengthBar();

	/// <summary>
	/// ゲームシーンの座標に移動
	/// </summary>
	void MoveGamePos();

	/// <summary>
	/// リザルトシーンの座標に移動
	/// </summary>
	void MoveResultPos();

private:
	//TIME文字スプライト
	Sprite* timeSprite = nullptr;
	//ポイント表示(枠)スプライト
	Sprite* frameSprite = nullptr;
	//ポイント表示(バー)スプライト
	Sprite* barSprite = nullptr;
	//制限時間最大
	const int timeLimitMax = 5400;
	//時間計測タイマー
	int timer = timeLimitMax;
	//カウントダウンするか
	bool isCountDown = false;
	//回復中か
	bool isRecovery = false;
	//カウントダウンが最後まで行ったか
	bool isCountDownEnd = false;


	//バーの長さ最大値
	const float lengthMax = 896;
	//回復中バーの長さ変更タイマー
	int recoveryLengthTimer = 0;
	//回復中バーの長さ変更前の長さ
	float recoveryLengthBefore = 0;
	//回復中バーの長さ変更後の長さ
	float recoveryLengthAfter = 0;

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