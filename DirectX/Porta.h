#pragma once
#include "Object3d.h"

class Porta
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
	/// ポルタ生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	/// <returns>ポルタ</returns>
	static Porta *Create(Model *model, XMFLOAT3 spawnPosition, float moveDegree);

	/// <summary>
	/// 反射ラインをセット
	/// </summary>
	/// <param name="reflectionLine">反射ライン</param>
	static void SetReflectionLine(XMFLOAT2 reflectionLine) { Porta::reflectionLine = reflectionLine; }

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Porta();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <returns>成否</returns>
	bool Initialize(Model *model, XMFLOAT3 spawnPosition, float moveDegree);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="targetPosition">進行方向になる標的の座標</param>
	void Update();

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
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 移動角度を設定
	/// </summary>
	/// <param name="moveDegree">移動角度(真上が0)</param>
	void SetMoveAngle(float moveDegree);

	/// <summary>
	/// 左右反射
	/// </summary>
	void ReflectionX();

	/// <summary>
	/// 上下反射
	/// </summary>
	void ReflectionY();

	/// <summary>
	/// スピードアップ
	/// </summary>
	void SpeedUp();

protected:
	//反射するライン
	static XMFLOAT2 reflectionLine;

protected:
	//敵オブジェクト
	Object3d *enemyObject = nullptr;
	//移動角度
	float moveDegree = 0;
	//移動角度
	float moveAngle = 0.0f;
	//移動速度
	float moveSpeed = 0.5f;
	//移動量
	XMFLOAT2 vel = {};
	//体力
	int HP = 40;
	//生きているか
	bool isAlive = true;
	//画面内にいるか
	bool isInScreen = false;
	//削除するか
	bool isDelete = false;
};

