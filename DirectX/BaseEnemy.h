#pragma once
#include "Object3d.h"

class BaseEnemy
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	enum EnemyGroup
	{
		None,		//未設定
		Straighter,	//直進
		Division,	//分裂
		Releaser,	//放出
		Chaser		//追跡
	};

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="enemyModel">モデル</param>
	/// <param name="spawnPosition">スポーン時の座標</param>
	/// <returns>成否</returns>
	virtual bool Initialize(Model* enemyModel, XMFLOAT3 spawnPosition, float moveDegree) = 0;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 死亡
	/// </summary>
	void Dead();

	/// <summary>
	/// 削除
	/// </summary>
	void SetDelete();

	/// <summary>
	/// ノックバックの情報をセット
	/// </summary>
	/// <param name="angle">吹っ飛ぶ角度</param>
	/// <param name="power">ノックバックの強さ</param>
	/// <param name="power">衝撃波の種類</param>
	virtual void SetKnockBack(float angle, int powerLevel, int shockWaveGroup);

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { enemyObject->SetModel(model); }

	/// <summary>
	///	壁にぶつかったか判定
	/// </summary>
	/// <returns>壁にぶつかったか</returns>
	bool IsCollisionWall(XMFLOAT2 wallLine);

	//getter
	int GetGroup() { return group; }
	XMFLOAT3 GetPosition() { return enemyObject->GetPosition(); }
	XMFLOAT3 GetScale() { return enemyObject->GetScale(); }
	int GetPower() { return power; }
	float GetMoveDegree() { return moveDegree; }
	int GetKnockBackPowerLevel() { return knockBackPowerLevel; }
	bool GetIsKnockBack() { return isKnockBack; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsCreateEnemy() { return isCreateEnemy; }
	bool GetIsDelete() { return isDelete; }

protected:
	/// <summary>
	/// 移動処理
	/// </summary>
	virtual void Move() = 0;

	/// <summary>
	/// 移動角度を設定
	/// </summary>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	void SetMoveAngle(float moveDegree);

	/// <summary>
	/// ノックバック
	/// </summary>
	virtual void KnockBack();

protected:
	//枠のライン
	static XMFLOAT2 wallLine;

protected:
	//所属グループ
	int group = None;
	//敵オブジェクト
	Object3d* enemyObject = nullptr;
	//攻撃力
	int power = 0;
	//生きているか
	bool isAlive = true;
	//移動角度
	float moveDegree = 0;
	//移動角度
	float moveAngle = 0.0f;
	//移動速度
	float moveSpeed = 0.5f;
	//移動量
	XMFLOAT2 vel = {};
	//画面内にいるか
	bool isInScreen = false;
	//ノックバックする時間
	int knockBackTime = 0;
	//ノックバックタイマー
	int knockBackTimer = 0;
	//ノックバックの角度
	float knockBackAngle = 0.0f;
	//ノックバックの強さ段階
	int knockBackPowerLevel = 0;
	//ノックバックの強さ
	int knockBackPower = 0;
	//ノックバックしているか
	bool isKnockBack = false;
	//最後に当たった衝撃波の種類
	int lastCollisionShockWave = 0;
	//最高レベルのノックバックを行うか
	bool isKnockBackMax = false;
	//新たに敵を生成するか
	bool isCreateEnemy = false;
	//エフェクト
	int effectCount = 0;
	//削除するか
	bool isDelete = false;
};