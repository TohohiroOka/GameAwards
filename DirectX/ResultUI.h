#pragma once
#include"Sprite.h"

class ResultUI
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	enum DrawScene
	{
		BlackOut,		//暗転
		ResultDraw,		//リザルト描画
		FinalScoreDraw,	//最終スコア描画
		Stay,			//一時停止
		PressButtonDraw,//pressButton描画
	};

public:
	/// <summary>
	/// タイトルロゴ生成
	/// </summary>
	/// <returns>タイトルロゴ</returns>
	static ResultUI* Create(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int pressButtonTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResultUI();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int pressButtonTexNum);

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
	/// 最終スコアを確定させる
	/// </summary>
	void SetFinalScore(int finalScore);

	//getter
	bool GetIsDrawAll() { return isDrawAll; }

private:
	/// <summary>
	/// 暗転
	/// </summary>
	void BlackOutUpdate();

	/// <summary>
	/// 時間計測
	/// </summary>
	void TimeCount();

	/// <summary>
	/// 表示用スコアスプライトのカウントを増やす演出
	/// </summary>
	void IncreaseDisplayScore();

	/// <summary>
	///  表示用スコアスプライト変更
	/// </summary>
	void IncreaseScoreSprite();

private:
	//暗転用スプライト
	Sprite* blackoutSprite = nullptr;
	//リザルトスプライト
	Sprite* resultSprite = nullptr;
	//SCORE:スプライト
	Sprite* SCORESprite = nullptr;
	//最終スコアスプライト
	static const int scoreDigits = 8;
	Sprite* finalScoreSprite[scoreDigits] = { nullptr };
	//pressButtonスプライト
	Sprite* pressButtonSprite = nullptr;
	//最終スコア
	int finalScore = 0;
	//表示用スコア
	int displayScore = 0;
	//時間計測タイマー
	int timer = 0;
	//だんだん描画していく
	int drawScene = DrawScene::BlackOut;
	//全て描画したか
	bool isDrawAll = false;
};