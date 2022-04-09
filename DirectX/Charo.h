#pragma once
#include "Object3d.h"

class Charo
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
	/// チャロ生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <returns>チャロ</returns>
	static Charo *Create(Model *model, XMFLOAT3 spawnPosition);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Charo();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <returns>成否</returns>
	bool Initialize(Model *model, XMFLOAT3 spawnPosition);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="targetPosition">進行方向になる標的の座標</param>
	void Update(XMFLOAT3 targetPosition);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	void Damage(int damagePower);

	/// <summary>
	/// 死亡
	/// </summary>
	void Dead();

	/// <summary>
	/// 削除
	/// </summary>
	void SetDelete();

	//getter
	XMFLOAT3 GetPosition() { return enemyObject->GetPosition(); }
	XMFLOAT3 GetScale() { return enemyObject->GetScale(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsDelete() { return isDelete; }

protected:
	/// <summary>
	/// ターゲットの向きに進行角度を修正
	/// </summary>
	void SetMoveAngle(XMFLOAT3 targetPosition);

	/// <summary>
	/// 移動速度変更
	/// </summary>
	void ChangeMoveSpeed();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

protected:
	//敵オブジェクト
	Object3d *enemyObject = nullptr;
	//移動速度
	float moveSpeed = 1.0f;
	//移動速度のタイマー
	int moveSpeedTimer = 0;
	//移動角度
	float moveAngle = 0.0f;
	//体力
	int HP = 20;
	//生きているか
	bool isAlive = true;
	//削除するか
	bool isDelete = false;
};

