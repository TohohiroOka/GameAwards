#pragma once
#include"Sprite.h"

class Blackout
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
	/// シーン遷移用暗転生成
	/// </summary>
	/// <returns>シーン遷移用暗転</returns>
	static Blackout* Create(int plainTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Blackout();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int plainTexNum);

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
	/// 暗転状態にセット
	/// </summary>
	void SetBlackout();

	/// <summary>
	/// 暗転解除状態にセット
	/// </summary>
	void SetBlackoutReturn();

	//getter
	bool GetIsAllBlack() { return isAllBlack; }
	bool GetIsBlackout() { return isBlackout; }
	bool GetIsBlackReturn() { return isReturn; }

private:
	/// <summary>
	/// 暗転
	/// </summary>
	void BlackoutUpdate();

	/// <summary>
	/// 暗転を戻す
	/// </summary>
	void BlackoutReturn();

private:
	//暗転用スプライト
	Sprite* blackoutSprite = nullptr;
	//真っ暗か
	bool isAllBlack = false;
	//暗転中か
	bool isBlackout = false;
	//暗転する時間タイマー
	int blackoutTimer = 0;
	//暗転を戻すか
	bool isReturn = false;
	//暗転解除する時間タイマー
	int returnTimer = 0;
};