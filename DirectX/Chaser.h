#pragma once
#include "BaseEnemy.h"

class Chaser : public BaseEnemy
{
public:
	/// <summary>
	/// 追跡敵生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="spawnPosition">初期座標</param>
	/// <param name="stayPosition">停止座標</param>
	/// <returns>追跡敵</returns>
	static Chaser* Create(Model* model, XMFLOAT3 spawnPosition);

	/// <summary>
	/// ターゲット座標をセット
	/// </summary>
	/// <param name="targetPos"></param>
	static void SetTargetPos(XMFLOAT3 targetPos) { Chaser::targetPos = targetPos; };

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model, XMFLOAT3 spawnPosition, float moveDegree) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private:
	/// <summary>
	/// 移動
	/// </summary>
	void Move() override;

	/// <summary>
	/// ターゲットの向きに進行角度を修正
	/// </summary>
	void SetAngleForTarget(XMFLOAT3 targetPosition);

	/// <summary>
	/// 移動速度変更
	/// </summary>
	void ChangeMoveSpeed();

private:
	//ターゲット座標
	static XMFLOAT3 targetPos;

private:
	//移動速度のタイマー
	int moveSpeedTimer = 0;
};