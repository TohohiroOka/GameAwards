#pragma once
#include "Emitter.h"
#include <array>

class Camera;

class StageEffect
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public://静的メンバ関数

	/// <summary>
	/// プレイヤー移動時のエフェクトセット
	/// </summary>
	/// <param name="position">プレイヤーの座標</param>
	/// <param name="rotation">プレイヤーの角度</param>
	static void SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation);

	/// <summary>
	/// 敵をはじいた時のエフェクト
	/// </summary>
	/// <param name="position">敵座標</param>
	/// <param name="power">敵座標</param>
	static void SetPushEnemyPower(const XMFLOAT3 position, const unsigned char power);

	/// <summary>
	/// 敵がはじかれたときのエフェクト
	/// </summary>
	/// <param name="position">敵座標</param>
	static void SetPushEnemy(const XMFLOAT3 position);

	/// <summary>
	/// 壁が破壊されたときのエフェクト
	/// </summary>
	/// <param name="position">出現座標</param>
	static void SetWallBreak(const XMFLOAT3 position);

	/// <summary>
	/// が壁に当たった時のエフェクト
	/// </summary>
	/// <param name="position">敵の座標</param>
	/// <param name="direction">壁の方向 上->1/左->2/下->3/右->4</param>
	static void SetWallHitEnemey(const XMFLOAT3 position, const unsigned char direction);

	/// <summary>
	/// 敵を消すときの演出
	/// </summary>
	/// <param name="position">敵の座標</param>
	static void SetDeleteEnemey(const XMFLOAT3 position);

	/// <summary>
	/// 回復フィールドのエフェクト
	/// </summary>
	/// <param name="position">座標</param>
	static void SetHealField(const XMFLOAT3 position);

	/// <summary>
	/// 回復エフェクト
	/// </summary>
	/// <param name="position">プレイヤー座標</param>
	static void SetHeal(const XMFLOAT3 position);

public://メンバ関数

	StageEffect() {};
	~StageEffect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラクラスのインスタンス</param>
	void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

private:

	//プレイヤー移動時のエフェクト
	static Emitter* generalEffect;
	//出現間隔
	static int playerMoveControl;
	//壁オブジェクト系のパーティクルテクスチャ数
	static const int wallTexNum = 3;
	//敵が吹っ飛んだ時のエフェクト
	static std::array<Emitter*, wallTexNum> wallEffect;
	//敵を弾いた時のエフェクト
	static Emitter* pushEnemy;
	//回復エフェクト
	static Emitter* heal;
	//回復エフェクト出現時間
	static int healControl;
	//弾けるエフェクト
	static Emitter* pop;
	//回復フィールドのエフェクト出現間隔
	static int healFieldControl;
};