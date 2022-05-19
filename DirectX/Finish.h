#pragma once
#include"Sprite.h"

class Finish
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
	/// Finish表示生成
	/// </summary>
	/// <returns>Finish表示</returns>
	static Finish* Create(int finishTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Finish();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int finishTexNum);

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
	/// Finishスプライトを動かす状態にセット
	/// </summary>
	void SetFinishSpriteMove();

	//getter
	bool GetIsFinishSpriteMoveEnd() { return isFinishSpriteMoveEnd; }

private:

	/// <summary>
	/// Finishスプライトを動かす
	/// </summary>
	void FinishSpriteMove();

private:
	//Finish用スプライト
	Sprite* finishSprite = nullptr;
	//Finishスプライトを動かすか
	bool isFinishSpriteMove = false;
	//Finishが終わったか
	bool isFinishSpriteMoveEnd = false;
	//Finishスプライトを動かす時間タイマー
	int finishSpriteMoveTimer = 0;
	//サウンドの再生用
	int sound[1];
	bool isSound = false;//再生のフラグ
	//0:FINISH
};