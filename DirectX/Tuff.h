#pragma once
#include "BossEnemy.h"

class Tuff :public BossEnemy
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
	//行動パターン
	enum MovementPattern
	{
		ThreeWayBullet,	//3方向弾
		MoveRandomAngle,//ランダムで角度を設定して移動させる
		FiveWayBullet,	//5方向弾
		MoveTopScreen,	//画面上部に移動
		FallLockOn,		//落下地点ロックオン
		Shake,			//震える
		Fall,			//落下
		MoveRandomPos,	//ランダムで指定した座標に移動させる
	};

public:
	/// <summary>
	/// タッフ生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <returns>タッフ</returns>
	static Tuff* Create(Model* model);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Tuff();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="targetPosition">標的の座標</param>
	void Update(XMFLOAT3 targetPosition) override;

protected:
	/// <summary>
	/// スポーン
	/// </summary>
	void Spawn() override;

	/// <summary>
	/// 次の行動に移す
	/// </summary>
	void ChangeAction();

	/// <summary>
	/// 3方向弾の発射命令管理
	/// </summary>
	void ShotBulletThreeWay();

	/// <summary>
	/// 5方向弾の発射命令管理
	/// </summary>
	void ShotBulletFiveWay();

	/// <summary>
	/// ランダムで移動角度をセット
	/// </summary>
	void SetRandomAngle();

	/// <summary>
	/// ランダムでセットした角度に移動
	/// </summary>
	void MoveAngle();

	/// <summary>
	/// 画面上部に移動情報をセット
	/// </summary>
	void SetMoveTop();

	/// <summary>
	/// 画面上部に移動
	/// </summary>
	void MoveTop();

	/// <summary>
	/// ターゲットをロックオンして頭上で動かす
	/// </summary>
	/// <param name="targetPosition">標的の座標</param>
	void LockOnTargetHead(XMFLOAT3 targetPosition);

	/// <summary>
	/// ランダムで移動座標をセット
	/// </summary>
	void SetRandomPos();

	/// <summary>
	/// シェイク情報をセット
	/// </summary>
	void SetShake();

	/// <summary>
	/// オブジェクトをシェイクさせる
	/// </summary>
	void ShakeObject();

	/// <summary>
	/// 落下情報をセット
	/// </summary>
	void SetFall();

	/// <summary>
	/// オブジェクトを落下させる
	/// </summary>
	void FallObject();

	/// <summary>
	/// ランダムでセットした座標に移動
	/// </summary>
	void MovePos();

	/// <summary>
	/// 枠のラインに当たっている
	/// </summary>
	void CollisitonFrame();

protected:
	//移動速度
	float moveSpeed = 0;
	//移動角度
	float moveAngle = 0;
	//イージングで座標移動時のスタート座標
	XMFLOAT3 easeStartPos = {};
	//イージングで座標移動時のエンド座標
	XMFLOAT3 easeEndPos = {};
	//シェイク時の基準座標
	XMFLOAT3 shakeBasePos = {};
};