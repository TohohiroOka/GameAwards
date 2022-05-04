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
	static TimeLimitGauge* Create(int frameTexNum, int barTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TimeLimitGauge();

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
	/// ポイントを増やす
	/// </summary>
	/// <param name="point">増加量</param>
	void AddPoint(int point);

	/// <summary>
	/// ポイントを使う
	/// </summary>
	void UsePoint();

	//getter
	bool GetIsGaugeMax() { return isGaugeMax; }

private:
	/// <summary>
	/// バーの長さを変更
	/// </summary>
	void ChangeLengthBar();

	/// <summary>
	/// バーの長さ変更をセット
	/// </summary>
	void SetChangeLength();

private:
	//ポイント表示(枠)スプライト
	Sprite* frameSprite = nullptr;
	//ポイント表示(バー)スプライト
	Sprite* barSprite = nullptr;
	//回復開始ポイント
	const int recoveryPointMax = 100;
	//回復ポイント
	int recoveryPoint = 0;
	//バースプライトの長さを変更するか
	bool isChangeLengthBar = false;
	//バースプライトの長さ最大値
	const float lengthMax = 556 / 5;
	//バースプライトの長さ変更タイマー
	int changeLengthTimer = 0;
	//バースプライトの長さ変更前の長さ
	float changeLengthBefore = 0;
	//バースプライトの長さ変更後の長さ
	float changeLengthAftar = 0;
	//ゲージが最大か
	bool isGaugeMax = false;
};