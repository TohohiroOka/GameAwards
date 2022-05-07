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
	static Straighter* Create(XMFLOAT3 spawnPosition, float moveDegree, int knockBackPowerLevel = 0);

	/// <summary>
	/// 直進敵のモデルをセット
	/// </summary>
	/// <param name="straighterModel1">初期モデル</param>
	/// <param name="straighterModel2">吹っ飛び威力1のモデル</param>
	/// <param name="straighterModel3">吹っ飛び威力2のモデル</param>
	/// <param name="straighterModel4">吹っ飛び威力3のモデル</param>
	static void SetModel(Model* straighterModel1, Model* straighterModel2, Model* straighterModel3, Model* straighterModel4);

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

private:
	//モデル
	static const int modelNum = 4;
	static Model* straighterModel[modelNum];
};