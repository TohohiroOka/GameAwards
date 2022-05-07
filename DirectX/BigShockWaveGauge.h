#pragma once
#include"Sprite.h"

class BigShockWaveGauge
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
	static BigShockWaveGauge* Create(int frameTexNum, int barTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BigShockWaveGauge();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int frameTexNum, int barTexNum);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(int combo);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// ゲームシーンの座標に移動状態にセット
	/// </summary>
	void SetMoveGamePos();

	/// <summary>
	/// リザルトシーンの座標に移動状態にセット
	/// </summary>
	void SetMoveResultPos();

	//getter
	bool GetIsMoveGamePosEnd() { return isMoveGamePosEnd; }

private:
	/// <summary>
	/// バーの長さを変更
	/// </summary>
	void ChangeLengthBar();

	/// <summary>
	/// バーの長さ変更をセット
	/// </summary>
	void SetChangeLength();

	/// <summary>
	/// ゲームシーンの座標に移動
	/// </summary>
	void MoveGamePos();

	/// <summary>
	/// リザルトシーンの座標に移動
	/// </summary>
	void MoveResultPos();

private:
	//ポイント表示(枠)スプライト
	Sprite* frameSprite = nullptr;
	//ポイント表示(バー)スプライト
	Sprite* barSprite = nullptr;
	//コンボ数
	int combo = 0;
	//バースプライトの長さを変更するか
	bool isChangeLengthBar = false;
	//バースプライトの長さ最大値
	const float lengthMax = 556 / 4;
	//バースプライトの長さ変更タイマー
	int changeLengthTimer = 0;
	//バースプライトの長さ変更前の長さ
	float changeLengthBefore = 0;
	//バースプライトの長さ変更後の長さ
	float changeLengthAftar = 0;
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