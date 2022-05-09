#pragma once
#include "Object3d.h"

class WallObject : public Object3d
{
private:

	struct EasingNum {
		XMFLOAT3 start = {};
		XMFLOAT3 end = {};
	};

public:

	enum class STATE {
		NONE,//非表示
		WAIT,//待機状態
		DIRECTING_LEFT_UP,//左上の定位置移動
		DIRECTING_RIGHT_UP,//右上の定位置移動
		DIRECTING_RIGHT_DOWN,//右下の定位置移動
		DIRECTING_LEFT_DOWN,//左下の定位置移動
		MOVE_UP,//上壁時の移動
		MOVE_RIGHT,//右壁時の移動
		MOVE_DOWN,//下壁時の移動
		MOVE_LEFT,//左壁時の移動
		DISPERSE,//現在地から少しばらけさせる
		HIT,//吹き飛ばされたガラクタと当たった時
	};

public:

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<WallObject> Create(Model* model);

private:

	/// <summary>
	/// 定位置までの移動
	/// </summary>
	void Directing();

	/// <summary>
	/// 壁移動
	/// </summary>
	void WallMove();

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

private://固定値

	//演出開始時位置
	static const XMFLOAT3 startPosition;
	//最大時間
	static const float directingMaxTime;
	//画面端の最小
	static const XMFLOAT2 minPosition;
	//画面端の最大
	static const XMFLOAT2 maxPosition;
	//定位置への移動時の回転幅
	static const float initDistance;
	//ちりばめ時間の最大
	static const float disperseMaxTime;

private:

	//オブジェクトの現在の状態
	STATE state = STATE::NONE;
	//オブジェクトの前の状態
	STATE oldState = STATE::NONE;
	//スタート時の演出時間
	float time = 0;
	//回転の演出時に使用する角度
	int angle = 0;
	//回転時の幅
	float distance = 0;
	//イージングで使う座標
	EasingNum easingPos;
	//イージングで使う回転
	EasingNum easingRota;
	//ちりばめ時の移動距離
	XMFLOAT3 disperseMovePos = {};
	//ちりばめ時の回転角
	XMFLOAT3 disperseMoveRota = {};

public:

	/// <summary>
	/// 演出開始
	/// </summary>
	void SetState(STATE state) { this->state = state; }

	/// <summary>
	/// lerpの終点セット
	/// </summary>
	void SetLerpEndPosition(XMFLOAT3 EndPos) { this->easingPos.end = EndPos; }

	/// <summary>
	/// ちりばめ時の移動距離セット
	/// </summary>
	void SetDisperseMovePos(XMFLOAT3 disperseMovePos) { this->disperseMovePos = disperseMovePos; }

	/// <summary>
	/// ちりばめ時の回転角セット
	/// </summary>
	void SetDisperseMoveRota(XMFLOAT3 disperseMoveRota) { this->disperseMoveRota = disperseMoveRota; }

	/// <summary>
	/// 画面端取得
	/// </summary>
	float GetTime() { return time; }

	/// <summary>
	/// 画面端の最小取得
	/// </summary>
	static XMFLOAT2 GetWallMinPosition() { return minPosition; }

	/// <summary>
	/// 画面端の最小取得
	/// </summary>
	static XMFLOAT2 GetWallMaxPosition() { return maxPosition; }
};