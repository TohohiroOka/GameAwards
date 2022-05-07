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

public:
	/// <summary>
	/// タイトルロゴ生成
	/// </summary>
	/// <returns>タイトルロゴ</returns>
	static ResultUI* Create(int plainTexNum, int resultTexNum, int breakTexNum, int numberTexNum, int maxComboTexNum, int retryTexNum, int pressATexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResultUI();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int plainTexNum, int resultTexNum, int breakTexNum, int numberTexNum, int maxComboTexNum, int retryTexNum, int pressATexNum);

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
	/// 壁破壊数を確定させる
	/// </summary>
	void SetBreakWallNum(int breakWallNum);

	/// <summary>
	/// 最大コンボを確定させる
	/// </summary>
	void SetMaxCombo(int maxCombo);

	/// <summary>
	/// 暗転状態にセット
	/// </summary>
	void SetBlackOut();

	/// <summary>
	/// リザルトスプライトを動かす状態にセット
	/// </summary>
	void SetMoveResultSprite();

	/// <summary>
	/// 壁破壊数スプライトを動かす状態にセット
	/// </summary>
	void SetMoveBreakSprite();

	/// <summary>
	/// 最大コンボ数スプライトを動かす状態にセット
	/// </summary>
	void SetMoveMaxComboSprite();

	/// <summary>
	/// リトライスプライトを動かす状態にセット
	/// </summary>
	void SetMoveRetrySprite();

	//getter
	bool GetIsDrawAll() { return isDrawAll; }

private:
	/// <summary>
	/// 壁破壊数スプライトの数字変更
	/// </summary>
	void ChangeBreakNumSprite();

	/// <summary>
	/// 最大コンボスプライトの数字変更
	/// </summary>
	void ChangeMaxComboSprite();

	/// <summary>
	/// 暗転
	/// </summary>
	void BlackOut();

	/// <summary>
	/// リザルトスプライトを動かす
	/// </summary>
	void MoveResultSprite();

	/// <summary>
	/// 壁破壊数スプライトを動かす
	/// </summary>
	void MoveBreakSprite();

	/// <summary>
	/// 最大コンボ数スプライトを動かす
	/// </summary>
	void MoveMaxComboSprite();

	/// <summary>
	/// リトライスプライトを動かす
	/// </summary>
	void MoveRetrySprite();

private:
	//暗転用スプライト
	Sprite* blackoutSprite = nullptr;
	//リザルトスプライト
	Sprite* resultSprite = nullptr;
	//BREAKスプライト
	Sprite* breakSprite = nullptr;
	//壊した数スプライト
	static const int breakDigits = 4;
	Sprite* breakNumSprite[breakDigits] = { nullptr };
	//MAXCOMBOスプライト
	Sprite* maxComboSprite = nullptr;
	//最大コンボ数スプライト
	static const int maxComboDigits = 4;
	Sprite* maxComboNumSprite[maxComboDigits] = { nullptr };
	//リトライスプライト
	Sprite* retrySprite = nullptr;
	//PRESS Aスプライト
	Sprite* pressASprite = nullptr;

	//壁破壊枚数
	int breakWallNum = 0;
	//最大コンボスコア
	int maxCombo = 0;

	//暗転中か
	bool isBlackout = false;
	//暗転する時間タイマー
	int blackoutTimer = 0;
	//リザルトスプライトを動かすか
	bool isMoveResultSprite = false;
	//リザルトスプライトを動かす時間タイマー
	int moveResultSpriteTimer = 0;
	//壁破壊数スプライトを動かすか
	bool isMoveBreakSprite = false;
	//壁破壊数スプライトを動かす時間タイマー
	int moveBreakSpriteTimer = 0;
	//最大コンボ数スプライトを動かすか
	bool isMoveMaxComboSprite = false;
	//最大コンボ数スプライトを動かす時間タイマー
	int moveMaxComboSpriteTimer = 0;
	//リトライスプライトを動かすか
	bool isMoveRetrySprite = false;
	//リトライスプライトを動かす時間タイマー
	int moveRetrySpriteTimer = 0;

	//全て描画したか
	bool isDrawAll = false;
};