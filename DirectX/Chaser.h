#pragma once
#include "BaseEnemy.h"

class Chaser : public BaseEnemy
{
public:
	/// <summary>
	/// 追跡敵生成
	/// </summary>
	/// <param name="spawnPosition">初期座標</param>
	/// <param name="stayPosition">停止座標</param>
	/// <returns>追跡敵</returns>
	static Chaser* Create(XMFLOAT3 spawnPosition, float moveDegree = 0, bool isParent = false);

	/// <summary>
	/// 追跡敵のモデルをセット
	/// </summary>
	/// <param name="chaserModel">モデル</param>
	static void SetModel(Model* chaserModel);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	/// <returns>成否</returns>
	bool Initialize(XMFLOAT3 spawnPosition, float moveDegree) override;

private:
	/// <summary>
	/// 親から生まれた場合の設定
	/// </summary>
	/// <param name="moveDegree"></param>
	void SetSpawnParent(float moveDegree);

	/// <summary>
	/// 移動
	/// </summary>
	void Move() override;

	/// <summary>
	/// リザルトシーン用の移動
	/// </summary>
	void ResultMove() override;

	/// <summary>
	/// ターゲットの向きに進行角度を修正
	/// </summary>
	void SetAngleForTarget(XMFLOAT3 targetPosition);

	/// <summary>
	/// 画面内にいるかチェック
	/// </summary>
	bool CheckInScreen();

	/// <summary>
	/// 直進の動き
	/// </summary>
	void MoveStraight();

	/// <summary>
	/// 直進の動きをする時間を計測
	/// </summary>
	void CountMoveStraightTime();

	/// <summary>
	/// 追従の動き
	/// </summary>
	void MoveChase();

	/// <summary>
	/// 移動速度変更
	/// </summary>
	void ChangeMoveSpeed();

private:
	//モデル
	static Model* chaserModel;

private:
	//移動速度のタイマー
	int moveSpeedTimer = 0;
	//親から生まれた後の動きをするか
	bool isParentMove = false;
	//直進時間タイマー
	int moveStraightTimer = 0;
};