#pragma once
#include "Object3d.h"

class Wall
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
	/// 壁生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>プレイヤー</returns>
	static Wall* Create(Model* model = nullptr);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Wall();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ダメージを与える
	/// </summary>
	/// <param name="damagePower">ダメージを与える威力</param>
	void Damage(int damagePower);

	/// <summary>
	/// 破壊
	/// </summary>
	void Break();

	/// <summary>
	/// 休憩状態にする
	/// </summary>
	void SetBreakTime();

	/// <summary>
	/// 新たな壁生成状態にする
	/// </summary>
	void SetCreateWall();

	/// <summary>
	/// 破壊された瞬間か
	/// </summary>
	/// <returns></returns>
	bool GetTriggerBreak();

	//getter
	XMFLOAT2 GetWallLine() { return wallLine; }
	int GetHP() { return HP; }
	bool GetIsCreate() { return isCreate; }
	bool GetIsAlive() { return isAlive; }

private:
	/// <summary>
	/// 壁破壊後から生成までの休憩時間
	/// </summary>
	void BreakTime();

	/// <summary>
	/// 新たな壁生成
	/// </summary>
	void CreateWall();

	/// <summary>
	/// HP割合に応じて色を変化させる
	/// </summary>
	void ChangeColor();

private:
	//壁オブジェクト
	Object3d* wallObject = nullptr;
	//画面上で見たときの壁のラインの位置
	XMFLOAT2 wallLine = { 196, 108 };
	//基準の最大HP
	const int baseMaxHP = 10;
	//壁の最大HP
	int maxHP = 0;
	//壁のHP
	int HP = 0;
	//休憩時間タイマー
	int breakTimer = 0;
	//休憩中か
	bool isBreakTime = false;
	//壁生成初期サイズ
	XMFLOAT3 createStartScale = {};
	//壁生成最終サイズ
	XMFLOAT3 createEndScale = {};
	//壁生成タイマー
	int createTimer = 0;
	//壁生成回数
	int createCount = 0;
	//壁生成中か
	bool isCreate = false;
	//壊されたか
	bool isBreak = false;
	//生きているか
	bool isAlive = true;
};