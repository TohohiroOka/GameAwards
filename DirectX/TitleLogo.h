#pragma once
#include "BaseEnemy.h"

class TitleLogo : public BaseEnemy
{
public:
	/// <summary>
	/// タイトルロゴ生成
	/// </summary>
	/// <returns>タイトルロゴ</returns>
	static TitleLogo* Create();

	/// <summary>
	/// タイトルロゴのモデルをセット
	/// </summary>
	/// <param name="titleLogoModel">モデル</param>
	static void SetModel(Model* titleLogoModel);

	/// <summary>
	/// スポーン終了した瞬間か
	/// </summary>
	/// <returns></returns>
	static bool GetTriggerSpawnEnd();

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
	/// <param name="powerLevel">ノックバックの強さ</param>
	/// <param name="powerMagnification">ノックバックの強さの倍率</param>
	void SetKnockBack(float angle, int powerLevel, float powerMagnification) override;

	/// <summary>
	///	壁にぶつかったか判定
	/// </summary>
	/// <returns>壁にぶつかったか</returns>
	bool IsCollisionWall() override;

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
	/// X軸反射
	/// </summary>
	void ReflectionX();

	/// <summary>
	/// Y軸反射
	/// </summary>
	void ReflectionY();


private:
	//モデル
	static Model* titleLogoModel;
	//スポーン終了したか
	static bool isSpawnEnd;
	//スポーン座標
	static const XMFLOAT3 spawnPos;
	//停止座標
	static const XMFLOAT3 stayPos;

private:
	//スポーン中か
	bool isSpawn = true;
	//スポーンする時間タイマー
	int spawnTimer = 0;
	//一度壁に当たったか
	bool isCollisionWallFirst = false;
};