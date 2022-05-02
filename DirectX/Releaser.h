#pragma once
#include "BaseEnemy.h"

class Releaser : public BaseEnemy
{
public:
	/// <summary>
	/// 放出敵生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="spawnPosition">初期座標</param>
	/// <param name="stayPosition">停止座標</param>
	/// <returns>放出敵</returns>
	static Releaser* Create(Model* model, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

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

	/// <summary>
	/// ノックバックの情報をセット
	/// </summary>
	/// <param name="angle">吹っ飛ぶ角度</param>
	/// <param name="power">ノックバックの強さ</param>
	void SetKnockBack(float angle, int powerLevel) override;

private:
	/// <summary>
	/// 移動
	/// </summary>
	void Move() override;

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
	//停止座標
	XMFLOAT3 stayPos = {};
	//放出タイマー
	int releaseTimer = 0;
	//放出した回数
	int releaseCount = 0;
	//放出モードか
	bool isReleaseMode = false;
};