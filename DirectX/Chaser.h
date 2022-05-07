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
	static Chaser* Create(XMFLOAT3 spawnPosition);

	/// <summary>
	/// 追跡敵のモデルをセット
	/// </summary>
	/// <param name="chaserModel1">初期モデル</param>
	/// <param name="chaserModel2">吹っ飛び威力1のモデル</param>
	/// <param name="chaserModel3">吹っ飛び威力2のモデル</param>
	/// <param name="chaserModel4">吹っ飛び威力3のモデル</param>
	static void SetModel(Model* chaserModel1, Model* chaserModel2, Model* chaserModel3, Model* chaserModel4);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	/// <returns>成否</returns>
	bool Initialize(XMFLOAT3 spawnPosition, float moveDegree) override;

	/// <summary>
	/// ノックバックの情報をセット
	/// </summary>
	/// <param name="angle">吹っ飛ぶ角度</param>
	/// <param name="power">ノックバックの強さ</param>
	void SetKnockBack(float angle, int powerLevel, int shockWaveGroup) override;

private:
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
	/// 移動速度変更
	/// </summary>
	void ChangeMoveSpeed();

private:
	//モデル
	static const int modelNum = 4;
	static Model* chaserModel[modelNum];

private:
	//移動速度のタイマー
	int moveSpeedTimer = 0;
};