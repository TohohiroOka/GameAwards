#pragma once
#include "Object3d.h"

class AbsorptionCircle 
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
	enum ActionNumber 
	{
		Spread,	//円を広げる
		Stay,	//一時停止
		Shrink,	//円を縮める
	};

public:
	/// <summary>
	/// 吸収円生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>吸収円</returns>
	static AbsorptionCircle* Create(Model* model, XMFLOAT3 position);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~AbsorptionCircle();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="waveModel">モデル</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model, XMFLOAT3 position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 削除
	/// </summary>
	void SetDelete();

	//getter
	XMFLOAT3 GetPosition() { return circleObject->GetPosition(); }
	XMFLOAT3 GetScale() { return circleObject->GetScale(); }
	bool GetIsDelete() { return isDelete; }

private:
	/// <summary>
	/// 吸収円を広げる
	/// </summary>
	void SpreadCircle();

	/// <summary>
	/// 一時停止
	/// </summary>
	void StayCircle();

	/// <summary>
	/// 吸収円を縮小させる
	/// </summary>
	void ShrinkCircle();

private:
	//吸収円オブジェクト
	Object3d* circleObject = nullptr;
	//円のサイズを広げるタイマー
	int spreadTimer = 0;
	//一時停止タイマー
	int stayTimer = 0;
	//円のサイズを縮めるタイマー
	int shrinkTimer = 0;
	//円の行動
	int action = ActionNumber::Spread;
	//削除するか
	bool isDelete = false;
};