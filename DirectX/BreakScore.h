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
	/// スプライト行動
	/// </summary>
	enum MoveSpriteScene
	{
		None,	//何もしない
		Fall,	//降下
		Stay,	//停止
		Rize,	//上昇
	};

public:
	/// <summary>
	/// 壁破壊スコア生成
	/// </summary>
	/// <returns>壁破壊スコア</returns>
	static BreakScore* Create(int breakTexNum, int numberTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BreakScore();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int breakTexNum, int numberTexNum);

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
	/// 壁破壊スコアを増やす
	/// </summary>
	void AddScore();

	//getter
	int GetBreakScore() { return breakScore; }

private:
	/// <summary>
	/// 壁破壊数スプライトの数字変更
	/// </summary>
	void ChangeBreakNumSprite();

	/// <summary>
	/// スプライトを動かす状態にする
	/// </summary>
	void SetMoveSprite();

	/// <summary>
	/// スプライトを降下させる
	/// </summary>
	void SpriteFall();

	/// <summary>
	/// スプライトを停止させる
	/// </summary>
	void SpriteStay();

	/// <summary>
	/// スプライトを上昇させる
	/// </summary>
	void SpriteRize();

private:
	//BREAKスプライト
	Sprite* breakSprite = nullptr;
	//壁破壊数スプライト
	static const int breakDigits = 3;
	Sprite* breakNumSprite[breakDigits] = { nullptr };

	//壁破壊枚数
	int breakScore = 0;

	//スプライトを動かすか
	bool isMoveSprite = false;
	//スプライトの行動
	int moveSpriteScene = MoveSpriteScene::None;
	//スプライト降下タイマー
	int spriteFallTimer = 0;
	//スプライト停止タイマー
	int spriteStayTimer = 0;
	//スプライト上昇タイマー
	int spriteRizeTimer = 0;
};