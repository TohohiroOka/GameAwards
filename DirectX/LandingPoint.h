#pragma once
#include "Object3d.h"
#include "Player.h"

class LandingPoint
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// 着弾地点生成
	/// </summary>
	/// <returns>着弾地点</returns>
	static LandingPoint* Create(Model* model);

	static void SetMoveRange(XMFLOAT2 moveRangeMin, XMFLOAT2 moveRangeMax) {
		LandingPoint::moveRangeMin = moveRangeMin;
		LandingPoint::moveRangeMax = moveRangeMax;
	}

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LandingPoint();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(XMFLOAT3 playerPosition, XMFLOAT3 playerRotation);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 死亡
	/// </summary>
	/// <param name="pointNum">着弾地点の番号(左が0)</param>
	void Dead(int pointNum);

	/// <summary>
	/// 再生成
	/// </summary>
	void Revive();

	/// <summary>
	/// 着弾点が全て死んでいるか
	/// </summary>
	/// <returns>全て死んでいるか</returns>
	bool CheckAllDead();

	//プレイヤーに追従させるか
	void SetIsChase(bool isChase) { this->isChase = isChase; }

	//getter
	bool GetIsChase() { return isChase; }

private:
	/// <summary>
	/// 生成
	/// </summary>
	void Create();

	/// <summary>
	/// 座標更新
	/// </summary>
	/// <param name="weaponPosition">プレイヤーの座標</param>
	/// <param name="weaponRotation">プレイヤーの角度</param>
	void Chase(XMFLOAT3 position, XMFLOAT3 rotation);


private:
	//動ける範囲
	static XMFLOAT2 moveRangeMin;
	static XMFLOAT2 moveRangeMax;

private:
	//着弾地点オブジェクト
	static const int pointNum = 3;
	Object3d* pointCircleObject[pointNum] = { nullptr };
	//生きているか
	bool isAlive[pointNum] = { true, true, true };
	//生成中か
	bool isCreate = true;
	//生成する時間タイマー
	int createTimer = 0;
	//追従するか
	bool isChase = true;
};