#pragma once
#include "Object3d.h"

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
	/// 死亡
	/// </summary>
	void Dead();

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

	/// <summary>
	/// 死亡してサイズを変更状態をセット
	/// </summary>
	void SetDeadChangeScale();

	//getter
	XMFLOAT3 GetPosition() { return playerObject->GetPosition(); }
	XMFLOAT3 GetRotation() { return playerObject->GetRotation(); }
	XMFLOAT3 GetWeaponPosition() { return weaponObject->GetPosition(); }
	XMFLOAT3 GetWeaponRotation() { return weaponObject->GetRotation(); }
	XMFLOAT3 GetScale() { return playerObject->GetScale(); }
	int GetHP() { return HP; }
	bool GetIsSpawn() { return isDuringSpawn; }
	bool GetIsBulletShot() { return isBulletShot; }
	bool GetIsDamege() { return isDamage; }
	bool GetIsAlive() { return isAlive; }
	bool GetIsResetPos() { return isResetPos; }
	bool GetIsExistence() { return isExistence; }

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
	/// 弾発射
	/// </summary>
	void ShotBullet();

	/// <summary>
	/// ノックバック時の処理
	/// </summary>
	void Knockback();

	/// <summary>
	/// 枠のラインに当たってたら押し戻す
	/// </summary>
	void CollisionFrame();

	/// <summary>
	/// 初期位置に戻す処理
	/// </summary>
	void ResetPosition();

	/// <summary>
	/// 死亡してサイズを変更
	/// </summary>
	void DeadChangeScale();

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
	//体力
	int HP = 3;
	//移動速度
	float moveSpeed = 0.5f;
	//ダメージを喰らっているか
	bool isDamage = false;
	//ダメージを喰らってからの時間
	int damageTimer = 0;
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
	//弾を発射するか
	bool isBulletShot = false;
	//弾発射からの時間
	int bulletShotTimer = 0;
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
	bool isStop = true;
	//死亡してサイズを変更状態か
	bool isDeadChangeScale = false;
	//膨張前のサイズ
	float changeStartScale = 0;
	//膨張後のサイズ
	float changeEndScale = 0;
	//サイズ変更シーン
	int changeScaleScene = 0;
	//サイズ変更タイマー
	int changeScaleTimer = 0;
	//存在しているか(サイズが0になってたら完全消滅)
	bool isExistence = true;
	//バイブレーションタイマー
	int vibrationTimer = -1;
};