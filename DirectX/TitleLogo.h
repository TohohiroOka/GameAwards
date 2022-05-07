#pragma once
#include "BaseEnemy.h"

class TitleLogo : public BaseEnemy
{
public:
	/// <summary>
	/// タイトルロゴ生成
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <returns>タイトルロゴ</returns>
	static TitleLogo* Create(XMFLOAT3 spawnPosition);

	/// <summary>
	/// タイトルロゴのモデルをセット
	/// </summary>
	/// <param name="titleLogoModel">モデル</param>
	static void SetModel(Model* titleLogoModel);

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

private:
	//モデル
	static Model* titleLogoModel;
};