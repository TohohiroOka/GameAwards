#pragma once
#include"Sprite.h"

class ReadyGo
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
	/// ReadyGo表示生成
	/// </summary>
	/// <returns>ReadyGo表示</returns>
	static ReadyGo* Create(int readyTexNum, int goTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ReadyGo();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int readyTexNum, int goTexNum);

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
	/// Readyスプライトを動かす状態にセット
	/// </summary>
	void SetReadySpriteMove();

	/// <summary>
	/// Goスプライトを動かす状態にセット
	/// </summary>
	void SetGoSpriteMove();

	//getter
	bool GetIsReadyGoEnd() { return isReadyGoEnd; }

private:

	/// <summary>
	/// Readyスプライトを動かす
	/// </summary>
	void ReadySpriteMove();

	/// <summary>
	/// Goスプライトを動かす
	/// </summary>
	void GoSpriteMove();

private:
	//Ready用スプライト
	Sprite* readySprite = nullptr;
	//Go用スプライト
	Sprite* goSprite = nullptr;
	//Readyスプライトを動かすか
	bool isReadySpriteMove = false;
	//Goスプライトを動かすか
	bool isGoSpriteMove = false;
	//Readyスプライトを動かす時間タイマー
	int readySpriteMoveTimer = 0;
	//Goスプライトを動かす時間タイマー
	int goSpriteMoveTimer = 0;
	//ReadyGoが終わったか
	bool isReadyGoEnd = false;
};