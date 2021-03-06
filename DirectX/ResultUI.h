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
	static ResultUI* Create(int plainTexNum, int resultTexNum, int breakTexNum, int numberTexNum, int retryTexNum, int backTitleTexNum, int AButtonTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResultUI();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int plainTexNum, int resultTexNum, int breakTexNum, int numberTexNum, int retryTexNum, int backTitleTexNum, int AButtonTexNum);

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
	/// リトライスプライトを動かす状態にセット
	/// </summary>
	void SetMoveRetrySprite();

	/// <summary>
	/// 次のシーン選択を確定させる
	/// </summary>
	void SetSelect() { isSelect = true; };

	//getter
	bool GetIsDrawAll() { return isDrawAll; }
	bool GetIsRetry() { return isRetry; }
	bool GetIsSelect() { return isSelect; }

private:
	/// <summary>
	/// 壁破壊数スプライトの数字変更
	/// </summary>
	void ChangeBreakNumSprite();

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
	/// リトライスプライトを動かす
	/// </summary>
	void MoveRetrySprite();

	/// <summary>
	/// リトライするかタイトルシーンに戻るか
	/// </summary>
	void SelectRetry();

private:
	//暗転背景用スプライト
	Sprite* blackoutSprite = nullptr;
	//リザルトスプライト
	Sprite* resultSprite = nullptr;
	//BREAKスプライト
	Sprite* breakSprite = nullptr;
	//壊した数スプライト
	static const int breakDigits = 3;
	Sprite* breakNumSprite[breakDigits] = { nullptr };
	//リトライスプライト
	Sprite* retrySprite = nullptr;
	//タイトルに戻るスプライト
	Sprite* backTitleSprite = nullptr;
	//Aボタンスプライト
	Sprite* AButtonSprite = nullptr;

	//壁破壊枚数
	int breakWallNum = 0;

	//背景暗転中か
	bool isBlackout = false;
	//背景暗転する時間タイマー
	int blackoutTimer = 0;
	//リザルトスプライトを動かすか
	bool isMoveResultSprite = false;
	//リザルトスプライトを動かす時間タイマー
	int moveResultSpriteTimer = 0;
	//壁破壊数スプライトを動かすか
	bool isMoveBreakSprite = false;
	//壁破壊数スプライトを動かす時間タイマー
	int moveBreakSpriteTimer = 0;
	//リトライスプライトを動かすか
	bool isMoveRetrySprite = false;
	//リトライスプライトを動かす時間タイマー
	int moveRetrySpriteTimer = 0;

	//全て描画したか
	bool isDrawAll = false;

	//リトライ状態か
	bool isRetry = true;
	//確定したか
	bool isSelect = false;
	//サウンドの再生用
	int sound[2];
	//0:カーソル移動 1:ロゴ動く音
};