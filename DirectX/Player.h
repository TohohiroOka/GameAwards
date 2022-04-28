#pragma once
#include "Object3d.h"
#include "Energy.h"
#include "PowerUpLine.h"

class Player
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
	/// プレイヤー生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>プレイヤー</returns>
	static Player* Create(Model* model = nullptr);

	/// <summary>
	/// ウエポンのモデルをセット
	/// </summary>
	/// <param name="weaponHP1Model">プレイヤーのHP1状態のモデル</param>
	/// <param name="weaponHP2Model">プレイヤーのHP2状態のモデル</param>
	/// <param name="weaponHP3Model">プレイヤーのHP3状態のモデル</param>
	static void SetWeaponModel(Model* weaponHP1Model, Model* weaponHP2Model, Model* weaponHP3Model);

	/// <summary>
	/// 移動可能範囲をセット
	/// </summary>
	/// <param name="moveRange">移動可能範囲</param>
	static void SetMoveRange(XMFLOAT2 moveRange) { Player::moveRange = moveRange; }

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
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
	void Damage();

	/// <summary>
	/// ノックバックの情報をセット
	/// </summary>
	void SetKnockback();

	/// <summary>
	/// タックル状態をセット
	/// </summary>
	void SetTackle();

	/// <summary>
	/// スポーン開始をセット
	/// </summary>
	/// <param name="spawnPosition">スポーン座標</param>
	/// <param name="stayPosition">停止座標</param>
	void SetSpawn(XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

	/// <summary>
	/// 初期位置に戻す状態にセット
	/// </summary>
	void SetResetPosition();

	/// <summary>
	/// 停止状態をセット
	/// </summary>
	/// <param name="isStop">停止状態</param>
	void SetIsStop(bool isStop) { this->isStop = isStop; }


	//getter
	XMFLOAT3 GetPosition() { return playerObject->GetPosition(); }
	XMFLOAT3 GetRotation() { return playerObject->GetRotation(); }
	XMFLOAT3 GetWeaponPosition() { return weaponObject->GetPosition(); }
	XMFLOAT3 GetWeaponRotation() { return weaponObject->GetRotation(); }
	XMFLOAT3 GetScale() { return playerObject->GetScale(); }
	bool GetIsSpawn() { return isDuringSpawn; }
	bool GetIsShockWaveStart() { return isShockWaveStart; }
	bool GetIsDamege() { return isDamage; }
	bool GetIsResetPos() { return isResetPos; }
	bool GetIsTackle() { return isTackle; }

private:
	/// <summary>
	/// 初期位置に戻す処理
	/// </summary>
	void Spawn();

	/// <summary>
	/// プレイヤー移動
	/// </summary>
	/// <returns>移動処理を行ったか</returns>
	bool Move();

	/// <summary>
	/// パッドのスティックの角度による回転
	/// </summary>
	void PadStickRotation();

	/// <summary>
	/// 衝撃波発射
	/// </summary>
	void ShockWaveStart();

	/// <summary>
	/// ノックバック時の処理
	/// </summary>
	void Knockback();

	/// <summary>
	/// タックルの処理
	/// </summary>
	void Tackle();

	/// <summary>
	/// 枠のラインに当たってたら押し戻す
	/// </summary>
	void CollisionFrame();

	/// <summary>
	/// 初期位置に戻す処理
	/// </summary>
	void ResetPosition();

private:
	//プレイヤーのHP1のときのウエポンのモデル
	static Model* weaponHP1Model;
	//プレイヤーのHP2のときのウエポンのモデル
	static Model* weaponHP2Model;
	//プレイヤーのHP3のときのウエポンのモデル
	static Model* weaponHP3Model;
	//移動範囲
	static XMFLOAT2 moveRange;

private:
	//プレイヤーオブジェクト
	Object3d* playerObject = nullptr;
	//ウエポンオブジェクト
	Object3d* weaponObject = nullptr;
	//移動速度
	float moveSpeed = 0.5f;
	//ダメージを喰らっているか
	bool isDamage = false;
	//ダメージを喰らってからの時間
	int damageTimer = 0;
	//スポーン中か
	bool isDuringSpawn = false;
	//スポーンタイマー
	int spawnTimer = 0;
	//スポーン座標
	XMFLOAT3 spawnPosition = {};
	//停止座標
	XMFLOAT3 stayPosition = {};
	//衝撃波を発射するか
	bool isShockWaveStart = false;
	//ノックバックするか
	bool isKnockback = false;
	//ノックバック時間
	int knockBackTimer = 0;
	//ノックバックラジアン
	float knockRadian = 0;
	//ウエポン移動のlarpタイム
	int weaponLarpTime = 4;
	//ウエポン移動のスタート角度
	XMFLOAT3 rotaMin = {};
	//ウエポン移動のスタート角度
	float rotaMax = {};
	//タックル中か
	bool isTackle = false;
	//タックル経過時間タイマー
	int tackleTimer = 0;
	//タックルの向き
	float tackleAngle = 0;
	//初期位置に戻るか
	bool isResetPos = false;
	//初期位置に戻す前の位置
	XMFLOAT3 beforeResetPos = {};
	//初期位置に戻す前の角度
	XMFLOAT3 beforeResetRota = {};
	//初期位置に戻す前のウエポンの角度
	XMFLOAT3 beforeResetWeaponRota = {};
	//初期位置に戻る時間タイマー
	int resetPosTimer = 0;
	//停止状態か
	bool isStop = false;
	//バイブレーションタイマー
	int vibrationTimer = -1;
};