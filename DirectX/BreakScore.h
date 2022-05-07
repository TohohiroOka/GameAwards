#pragma once
#include"Sprite.h"

class BreakScore
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
	/// スコア表示生成
	/// </summary>
	/// <returns>スコア表示</returns>
	static BreakScore* Create(int numberTexNum, int breakTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BreakScore();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int numberTexNum, int breakTexNum);

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
	/// スコアを加算する
	/// </summary>
	void AddScore();

	/// <summary>
	/// ゲームシーンの座標に移動状態にセット
	/// </summary>
	void SetMoveGamePos();

	/// <summary>
	/// リザルトシーンの座標に移動状態にセット
	/// </summary>
	void SetMoveResultPos();

	//getter
	int GetScore() { return score; }
	bool GetIsMoveGamePosEnd() { return isMoveGamePosEnd; }

private:
	/// <summary>
	/// 表示用スコアスプライト変更
	/// </summary>
	void ChangeScoreSprite();

	/// <summary>
	/// ゲームシーンの座標に移動
	/// </summary>
	void MoveGamePos();

	/// <summary>
	/// リザルトシーンの座標に移動
	/// </summary>
	void MoveResultPos();

private:
	//スコア表示用スプライト
	static const int scoreDigits = 4;
	Sprite* scoreSprite[scoreDigits] = { nullptr };
	//BREAKスプライト
	Sprite* breakSprite = nullptr;
	//スコア
	int score = 0;
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