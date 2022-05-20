#pragma once
#include "Object3d.h"

class WallObject : public Object3d
{
public:

	enum class STATE {
		NONE,//非表示
		WAIT,//待機状態
		MOVE_UP_LEFT,//上壁時の左移動
		MOVE_UP_RIGHT,//上壁時の右移動
		MOVE_RIGHT_DOWN,//右壁時の下移動
		MOVE_RIGHT_UP,//右壁時の上移動
		MOVE_DOWN_LEFT,//下壁時の左移動
		MOVE_DOWN_RIGHT,//下壁時の右移動
		MOVE_LEFT_DOWN,//左壁時の下移動
		MOVE_LEFT_UP,//左壁時の上移動
		TRANSPARENCY,//透明化
		FALL,//落ちる
		OUT_SCREEN,//画面外に行く
	};

public:

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static WallObject* Create(Model* model);

	/// <summary>
	/// 静的変数の初期化
	/// </summary>
	static void staticReset();

private:

	/// <summary>
	/// 壁移動
	/// </summary>
	void WallMove();

	/// <summary>
	/// 透明化
	/// </summary>
	void Transparency();

	/// <summary>
	/// 落下
	/// </summary>
	void Fall();

	/// <summary>
	/// 画面外に行く処理
	/// </summary>
	void OutScreen();

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private://固定値

	//画面端の最小
	static const XMFLOAT2 minPosition;
	//画面端の最大
	static const XMFLOAT2 maxPosition;
	//透明化時間
	static const float transparentMaxTime;
	//減速フラグ
	static bool isSlow;

private:

	//オブジェクトの現在の状態
	STATE state = STATE::NONE;
	//オブジェクトの前の状態
	STATE oldState = STATE::NONE;
	//スタート時の演出時間
	float time = 0;
	//回転の演出時に使用する角度
	int angle = 0;
	//減速率
	float slow = 0.0f;
	//移動速度
	XMFLOAT3 moveSpeed = {};

public:

	/// <summary>
	/// 演出セット
	/// </summary>
	void SetState(STATE state) { this->state = state; }

	/// <summary>
	/// 演出状況取得
	/// </summary>
	STATE GetState() { return state; }

	/// <summary>
	/// 減速度セット
	/// </summary>
	static void SetSlow(bool isSlow) { WallObject::isSlow = isSlow; }

	/// <summary>
	/// 画面端の最小取得
	/// </summary>
	static XMFLOAT2 GetWallMinPosition() { return minPosition; }

	/// <summary>
	/// 画面端の最小取得
	/// </summary>
	static XMFLOAT2 GetWallMaxPosition() { return maxPosition; }
};