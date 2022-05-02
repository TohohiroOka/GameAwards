#pragma once
#include "BaseEnemy.h"

class Straighter : public BaseEnemy
{
public:
	/// <summary>
	/// 直進敵生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	/// <returns>直進敵</returns>
	static Straighter* Create(Model* model, XMFLOAT3 spawnPosition, float moveDegree, int knockBackPowerLevel = 0);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model, XMFLOAT3 spawnPosition, float moveDegree) override;


private:
	/// <summary>
	/// 移動
	/// </summary>
	void Move() override;

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
	/// 親のノックバックの強さを引き継ぐ
	/// </summary>
	/// <param name="knockBackPowerLevel">ノックバックの強さ</param>
	void SetParentKnockBackPowerLevel(int knockBackPowerLevel);
};