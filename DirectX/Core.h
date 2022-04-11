#pragma once
#include "Object3d.h"

class Core
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
	/// コア生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>コア</returns>
	static Core* Create(Model* model);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Core();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="damagePower">攻撃の強さ</param>
	void Damage(int damagePower);

	/// <summary>
	/// 死亡
	/// </summary>
	void Dead();

	/// <summary>
	/// スポーン開始をセット
	/// </summary>
	/// <param name="spawnPosition">スポーン座標</param>
	/// <param name="stayPosition">停止座標</param>
	void SetSpawn(XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

	//getter
	XMFLOAT3 GetPosition() { return coreObject->GetPosition(); }
	XMFLOAT3 GetScale() { return coreObject->GetScale(); }
	bool GetIsAlive() { return isAlive; }
	bool GetIsSpawn() { return isDuringSpawn; }

private:
	/// <summary>
	/// 初期位置に戻す処理
	/// </summary>
	void Spawn();

private:
	//コアオブジェクト
	Object3d* coreObject = nullptr;
	//生きているか
	bool isAlive = true;
	//スポーン中か
	bool isDuringSpawn = false;
	//スポーンタイマー
	int spawnTimer = 0;
	//スポーン座標
	XMFLOAT3 spawnPosition = {};
	//停止座標
	XMFLOAT3 stayPosition = {};
};

