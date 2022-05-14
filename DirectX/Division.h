#pragma once
#include "BaseEnemy.h"

class Division : public BaseEnemy
{
public:
	/// <summary>
	/// 分裂敵生成
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	/// <returns>分裂敵</returns>
	static Division* Create(XMFLOAT3 spawnPosition, float moveDegree);

	/// <summary>
	/// 分裂敵のモデルをセット
	/// </summary>
	/// <param name="divisionModel">モデル</param>
	static void SetModel(Model* divisionModel);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	/// <returns>成否</returns>
	bool Initialize(XMFLOAT3 spawnPosition, float moveDegree) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ノックバックの情報をセット
	/// </summary>
	/// <param name="angle">吹っ飛ぶ角度</param>
	/// <param name="power">ノックバックの強さ</param>
	void SetKnockBack(float angle, int powerLevel, float powerMagnification, int shockWaveGroup) override;

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
	/// 画面内にいるかチェック
	/// </summary>
	bool CheckInScreen();

	/// <summary>
	/// 左右反射
	/// </summary>
	void ReflectionX();

	/// <summary>
	/// 上下反射
	/// </summary>
	void ReflectionY();

	/// <summary>
	/// 生存時間更新
	/// </summary>
	void AliveTimeUpdate();

private:
	//モデル
	static Model* divisionModel;

private:
	//生存した時間タイマー
	int aliveTimer = 0;
};