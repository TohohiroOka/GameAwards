#pragma once
#include"Sprite.h"

class Energy
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
	/// エネルギー生成
	/// </summary>
	/// <returns>エネルギー</returns>
	static Energy* Create(int frameTexNum, int barTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Energy();

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
	/// エネルギーポイントを増やす
	/// </summary>
	void AddEnergyPoint(const int addPoint);

	/// <summary>
	/// エネルギーポイントを失う
	/// </summary>
	void LoseEnergyPoint();

	/// <summary>
	/// 最大値の20%のポイントを使用する
	/// </summary>
	void UseTwentyPercent();

	/// <summary>
	/// 最大値の○○%のポイントを持っているか確認
	/// </summary>
	/// <returns>最大値の○○%のポイントを持っているか</returns>
	bool CheckPercent(int percent);

	//getter
	const int GetEnergyPoint() {}

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
	//エネルギーポイント上限値
	const int energyPointMax = 100;
	//エネルギーポイント
	int energyPoint = 0;
	//バーの長さを変更するか
	bool isChangeLengthBar = false;
	//バーの長さ最大値
	const float lengthMax = 556;
	//バーの長さ変更タイマー
	int changeLengthTimer = 0;
	//バーの長さ変更前の長さ
	float changeLengthBefore = 0;
	//バーの長さ変更後の長さ
	float changeLengthAftar = 0;
};