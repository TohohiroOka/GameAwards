#pragma once
#include "Object3d.h"

class FixedEnemy
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
	/// 固定敵生成
	/// </summary>
	/// <param name="coreModel">コアのモデル</param>
	/// <param name="squareModel">外枠のモデル</param>
	/// <param name="spawnPosition">スポーン時座標</param>
	/// <param name="stayPosition">停止座標</param>
	/// <returns>ガルタ</returns>
	static FixedEnemy* Create(Model* coreModel, Model* squareModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FixedEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="coreModel">コアのモデル</param>
	/// <param name="squareModel">外枠のモデル</param>
	/// <param name="spawnPosition">スポーン時座標</param>
	/// <param name="stayPosition">停止座標</param>
	/// <returns>成否</returns>
	bool Initialize(Model* coreModel, Model* squareModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

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
	/// <param name="angle">角度</param>
	/// <param name="power">ノックバックの強さ</param>
	void SetKnockBack(float angle, int power);

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { coreObject->SetModel(model); }

	/// <summary>
	/// ノックバックが終わった瞬間か判定
	/// </summary>
	/// <returns></returns>
	bool TriggerEndKnockBack();

	//getter
	XMFLOAT3 GetPosition() { return coreObject->GetPosition(); }
	XMFLOAT3 GetScale() { return coreObject->GetScale(); }
	bool GetIsAlive() { return isAlive; }
	bool GetIsDelete() { return isDelete; }
	bool GetIsExistence() { return isExistence; }
	bool GetIsDuringSpawn() { return isDuringSpawn; }
	int GetKillBulletPower() { return killBulletPower; }

protected:
	/// <summary>
	/// スポーン
	/// </summary>
	void Spawn();

	/// <summary>
	/// ノックバック
	/// </summary>
	void KnockBack();

	/// <summary>
	/// 外枠の大きさを変更
	/// </summary>
	void ChangeSquareSize();

protected:
	//コアオブジェクト
	Object3d* coreObject = nullptr;
	//外枠オブジェクト
	Object3d* squareObject = nullptr;
	//スポーン時座標
	XMFLOAT3 spawnPosition = { 0, 0, 0 };
	//移動後の座標
	XMFLOAT3 stayPosition = { 0, 0, 0 };
	//生きているか
	bool isAlive = true;
	//演出を行っているか
	bool isEffect = false;
	//ノックバック時間
	int effectCount = 0;
	//存在しているか(ノックバックも終了)
	bool isExistence = true;
	//スポーン中か
	bool isDuringSpawn = true;
	//スポーンタイマー
	int spawnTimer = 0;
	//ノックバックタイマー
	int knockBackTimer = 0;
	//ノックバックの角度
	float knockBackAngle = 0.0f;
	//ノックバックの強さ
	int killBulletPower = 0;
	//ノックバックが終わった瞬間か
	bool triggerEndKnockBack = false;
	//外枠の大きさ変更タイマー
	int changeSquareSizeTimer = 0;
	//外枠の大きさを変更するか
	bool isChangeSquareSize = false;
	//削除するか
	bool isDelete = false;
};