#pragma once
#include "Object3d.h"

class Player
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
	/// プレイヤー生成
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="scale">大きさ</param>
	/// <returns>プレイヤー</returns>
	static Player *Create(Model *model = nullptr);
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="scale">大きさ</param>
	/// <returns>成否</returns>
	bool Initialize(Model *model, XMFLOAT3 position, XMFLOAT3 scale);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	XMFLOAT3 GetPosition() { return playerObject->GetPosition(); }
	XMFLOAT3 GetRotation() { return playerObject->GetRotation(); }

private:
	/// <summary>
	/// プレイヤー移動
	/// </summary>
	void Move();

	/// <summary>
	/// パッドのスティックの角度による回転
	/// </summary>
	void PadStickRotation();

private:
	//プレイヤーオブジェクト
	Object3d *playerObject = nullptr;
};

