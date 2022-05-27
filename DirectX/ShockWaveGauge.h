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
	/// ゲージをリセット
	/// </summary>
	void GaugeReset();

	/// <summary>
	/// ゲージポイント増加
	/// </summary>
	void IncreasePoint();

	/// <summary>
	/// ゲージポイント減少
	/// </summary>
	void DecreasePoint();

	//setter
	void SetIsUpdate(bool isUpdate) { this->isUpdate = isUpdate; }

	//getter
	int GetGaugeLevel() { return gaugeLevel; }

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
	/// 描画を開始(リスタート)する
	/// </summary>
	void DrawStart();

	/// <summary>
	/// 描画時間をカウント
	/// </summary>
	void CountDrawTimer();

	/// <summary>
	/// 透過させる
	/// </summary>
	void Transparent();

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
	//前のフレームのゲージレベル
	int oldGaugeLevel = 0;
	//前のフレームとゲージレベルが違うか
	bool isChangeGaugeLevel = false;
	//バースプライトの長さ最大値
	const float lengthMax = 229;
	//更新するか
	bool isUpdate = false;
	//描画するか
	bool isDraw = false;
	//描画時間カウントするか
	bool isDrawTimeCount = false;
	//描画時間カウントタイマー
	int drawTimer = 0;
	//透過させるか
	bool isTransparent = false;
	//透過させる時間
	int transparentTimer = 0;
};