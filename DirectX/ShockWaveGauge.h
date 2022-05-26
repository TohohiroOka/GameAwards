#pragma once
#include"Sprite.h"

class ShockWaveGauge
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
	/// 巨大衝撃波用ゲージ生成
	/// </summary>
	/// <returns>巨大衝撃波用ゲージ</returns>
	static ShockWaveGauge* Create(int frameTexNum, int barTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ShockWaveGauge();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int frameTexNum, int barTexNum);

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
	/// ゲージポイント増加
	/// </summary>
	void IncreasePoint();

	/// <summary>
	/// ゲージポイント減少
	/// </summary>
	void DecreasePoint();

	/// <summary>
	/// ゲームシーンの座標に移動状態にセット
	/// </summary>
	void SetMoveGamePos();

	/// <summary>
	/// リザルトシーンの座標に移動状態にセット
	/// </summary>
	void SetMoveResultPos();

	//getter
	int GetGaugeLevel() { return gaugeLevel; }
	bool GetIsMoveGamePosEnd() { return isMoveGamePosEnd; }

private:
	/// <summary>
	/// バーの長さを変更
	/// </summary>
	void ChangeLengthBar();

	/// <summary>
	/// ゲージレベルを変更
	/// </summary>
	void ChangeGaugeLevel();

	/// <summary>
	/// ゲームシーンの座標に移動
	/// </summary>
	void MoveGamePos();

	/// <summary>
	/// リザルトシーンの座標に移動
	/// </summary>
	void MoveResultPos();

private:
	//ゲージ(枠)スプライト
	Sprite* frameSprite = nullptr;
	//ゲージ(バー)スプライト
	Sprite* barSprite = nullptr;
	//最大ゲージポイント
	const int gaugePointMax = 1000;
	//ゲージポイント
	int gaugePoint = 0;
	//ゲージレベル
	int gaugeLevel = 0;
	//バースプライトの長さ最大値
	const float lengthMax = 134;
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