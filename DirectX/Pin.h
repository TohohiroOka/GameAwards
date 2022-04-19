#pragma once
#include "Object3d.h"

class Pin
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
	/// ピン生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ピン</returns>
	static Pin* Create(Model* model, XMFLOAT3 position);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Pin();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model, XMFLOAT3 position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
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

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { pinObject->SetModel(model); }

	//getter
	XMFLOAT3 GetPosition() { return pinObject->GetPosition(); }
	XMFLOAT3 GetScale() { return pinObject->GetScale(); }
	int GetHP() { return HP; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsDelete() { return isDelete; }
	bool GetIsDamege() { return isDamage; }

protected:
	//ピンオブジェクト
	Object3d* pinObject = nullptr;
	//体力
	int HP = 40;
	//生きているか
	bool isAlive = true;
	//削除するか
	bool isDelete = false;
	//ダメージを喰らっているか
	bool isDamage = false;
	//ダメージを喰らってからの時間
	int damageTimer = 0;
};