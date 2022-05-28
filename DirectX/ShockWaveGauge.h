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
	static ShockWaveGauge* Create(int gaugeTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ShockWaveGauge();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int gaugeTexNum);

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
	/// ゲージリセットをセット
	/// </summary>
	void SetGaugeReset();

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
	/// ゲージリセット
	/// </summary>
	void GaugeReset();

private:
	//ゲージスプライト
	Sprite* gaugeSprite = nullptr;
	//最大ゲージポイント
	const int gaugePointMax = 1000;
	//ゲージポイント
	int gaugePoint = 0;
	//ゲージレベル
	int gaugeLevel = 0;
	//ゲージスプライトの長さ最大値
	const float lengthMax = 192;
	//更新するか
	bool isUpdate = false;
	//ゲージリセットするか
	bool isGaugeReset = false;
	//ゲージリセットする時間タイマー
	int gaugeResetTimer = 0;
	//ゲージリセットする前のゲージの長さ
	XMFLOAT2 resetGaugeBeforeLength = {};
};