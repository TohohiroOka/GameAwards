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
	static Player* Create(Model* playerModel, Model* circleModel);

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
	bool Initialize(Model* playerModel, Model* circleModel);

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
	/// ゲーム開始時の座標に移動させる状態にセット
	/// </summary>
	void SetGameStartPos();

	/// <summary>
	/// ノックバックの情報をセット
	/// </summary>
	void SetKnockback();

	/// <summary>
	/// 一定間隔で出る衝撃波の発射
	/// </summary>
	/// <param name="combo">コンボ数</param>
	/// <returns>発射するか</returns>
	bool AutoShockWaveStart(int combo);

	/// <summary>
	/// プレイヤーがゲームシーンの座標に移動終了した瞬間か
	/// </summary>
	/// <returns></returns>
	bool GetTriggerMoveStartPosEnd();


	//setter
	void SetIsFreeMove(bool isFreeMove);

	//getter
	XMFLOAT3 GetPosition() { return playerObject->GetPosition(); }
	XMFLOAT3 GetRotation() { return playerObject->GetRotation(); }
	XMFLOAT3 GetScale() { return playerObject->GetScale(); }
	bool GetIsMoveStartPos() { return isMoveStartPos; }
	bool GetIsLitteringStart() { return isLitteringStart; }
	bool GetIsDamege() { return isDamage; }

private:
	/// <summary>
	/// ゲーム開始時の座標に移動
	/// </summary>
	void MoveGameStartPos();

	/// <summary>
	/// プレイヤー移動
	/// </summary>
	/// <returns>移動処理を行ったか</returns>
	bool Move();

	/// <summary>
	/// ダメージを暗い硬直
	/// </summary>
	void DamageWaitingTime();

	/// <summary>
	/// ポイ捨て
	/// </summary>
	void LitteringStart();

	/// <summary>
	/// ノックバック時の処理
	/// </summary>
	void Knockback();

	/// <summary>
	/// 枠のラインに当たってたら押し戻す
	/// </summary>
	void CollisionFrame();

private:
	//移動範囲
	static XMFLOAT2 moveRange;

private:
	//プレイヤーオブジェクト
	Object3d* playerObject = nullptr;
	//自動衝撃波が出るタイミングオブジェクト
	Object3d* shockWaveTimingObject = nullptr;
	//ゲーム開始時の座標に移動中か
	bool isMoveStartPos = false;
	//ゲーム開始時の座標に移動終了したか
	bool isMoveStartPosEnd = false;
	//ゲーム開始時の座標に移動前の座標
	XMFLOAT3 beforeMoveStartPos = {};
	//ゲーム開始時の座標に移動前の角度
	XMFLOAT3 beforeMoveStartRota = {};
	//ゲーム開始時の座標に移動する時間タイマー
	int moveStartPosTimer = 0;
	//移動速度
	float moveSpeed = 0.5f;
	//移動角度
	float moveDegree = 0;
	//自由に動けるか
	bool isFreeMove = true;
	//ダメージを喰らっているか
	bool isDamage = false;
	//ダメージを喰らってからの時間
	int damageTimer = 0;
	//衝撃波発射タイマー
	int autoShockWaveStartTimer = 0;
	//衝撃波発射時間
	int autoShockWaveStartTime = 0;
	//ポイ捨てをするか
	bool isLitteringStart = false;
	//ノックバックするか
	bool isKnockback = false;
	//ノックバック時間
	int knockBackTimer = 0;
	//ノックバックラジアン
	float knockRadian = 0;
	//バイブレーションタイマー
	int vibrationTimer = -1;
};