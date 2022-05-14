#pragma once
#include "BaseEnemy.h"

class Straighter : public BaseEnemy
{
public:
	/// <summary>
	/// 直進敵生成
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	/// <returns>直進敵</returns>
	static Straighter* Create(XMFLOAT3 spawnPosition, float moveDegree);

	/// <summary>
	/// 直進敵のモデルをセット
	/// </summary>
	/// <param name="straighterModel">モデル</param>
	static void SetModel(Model* straighterModel);

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

private:
	//モデル
	static Model* straighterModel;
};