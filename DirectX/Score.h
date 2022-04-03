#pragma once
#include"Sprite.h"

class Score
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
	static Score *Create(int textureNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Score();

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
	/// スコアに加算する値をセット
	/// </summary>
	void SetAddScore(int addScore);

protected:
	/// <summary>
	/// 表示用スコアを変更
	/// </summary>
	void ChangeScore();

	/// <summary>
	/// 表示用スコアスプライト変更
	/// </summary>
	void ChangeScoreSprite();

private:
	//スコア表示用スプライト
	static const int scoreDigits = 8;
	Sprite *scoreSprite[scoreDigits] = { nullptr };
	//スコア
	int score = 0;
	//表示用スコア
	int displayScore = 0;
	//変更前の表示用スコア
	int changeDisplayScoreStart = 0;
	//変更後の表示用スコア
	int changeDisplayScoreEnd = 0;
	//表示用スコア変更用のタイマー
	int changeDisplayScoreTimer = 0;
	//表示用スコア変更中か
	bool isChangeDisplayScore = false;
};