#pragma once
#include"Sprite.h"

class TitleLogo
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
	static TitleLogo* Create(int textureNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleLogo();

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
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 落下開始
	/// </summary>
	/// <param name="fallStartPosition">落下開始座標</param>
	/// <param name="fallEndPosition">落下地点</param>
	void FallStart(XMFLOAT2 fallStartPosition, XMFLOAT2 fallEndPosition);

	//getter
	bool GetIsFall() { return isFall; }

private:
	/// <summary>
	/// 落下
	/// </summary>
	void Fall();

private:
	//タイトルロゴスプライト
	Sprite* titleSprite = nullptr;
	//落下開始座標
	XMFLOAT2 fallStartPosition = {};
	//落下地点
	XMFLOAT2 fallEndPosition = {};
	//落下中か
	bool isFall = false;
	//落下タイマー
	int fallTimer = 0;
};