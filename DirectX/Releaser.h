#pragma once
#include "BaseEnemy.h"

class Releaser : public BaseEnemy
{
public:
	/// <summary>
	/// 放出敵生成
	/// </summary>
	/// <param name="spawnPosition">初期座標</param>
	/// <param name="stayPosition">停止座標</param>
	/// <returns>放出敵</returns>
	static Releaser* Create(XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

	/// <summary>
	/// 放出敵のモデルをセット
	/// </summary>
	/// <param name="releaserModel">モデル</param>
	static void SetModel(Model* releaserModel);

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
	/// <param name="powerLevel">ノックバックの強さ</param>
	/// <param name="powerMagnification">ノックバックの強さの倍率</param>
	void SetKnockBack(float angle, int powerLevel, float powerMagnification) override;

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
	/// 移動方向を停止座標に向けセット
	/// </summary>
	void SetStayPosAngle();

	/// <summary>
	/// 停止座標をセット
	/// </summary>
	void SetStayPos(XMFLOAT3 stayPosition);

	/// <summary>
	/// ノックバック
	/// </summary>
	void KnockBack() override;

	/// <summary>
	/// 放出モードの処理
	/// </summary>
	void ReleaseMode();

	/// <summary>
	/// 敵放出
	/// </summary>
	void Release();

private:
	//モデル
	static Model* releaserModel;

private:
	//停止座標
	XMFLOAT3 stayPos = {};
	//放出タイマー
	int releaseTimer = 0;
	//放出した回数
	int releaseCount = 0;
	//放出モードか
	bool isReleaseMode = false;
};