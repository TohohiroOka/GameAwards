#pragma once
#include "Emitter.h"

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
	/// コア爆破エフェクトセット
	/// </summary>
	/// <param name="position">出現座標</param>
	/// <returns>進み割合</returns>
	static float SetTitleCoreExplosion(const XMFLOAT3 position);

	/// <summary>
	/// プレイヤー移動時のエフェクトセット
	/// </summary>
	/// <param name="position">プレイヤーの座標</param>
	/// <param name="rotation">プレイヤーの角度</param>
	static void SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation);

	/// <summary>
	/// 敵が倒されたときのエフェクト
	/// </summary>
	/// <param name="position">敵の座標</param>
	/// <returns>生成個数</returns>
	static int SetEnemeyDead(const XMFLOAT3 position);

	/// <summary>
	/// 弾消滅時のエフェクト
	/// </summary>
	/// <param name="position">弾の座標</param>
	/// <param name="color">色</param>
	static void SetPlayerBulletDelete(const XMFLOAT3 position, const XMFLOAT4 color);

	/// <summary>
	/// 線が繋がった時のエフェクト
	/// </summary>
	/// <param name="position_one">始点</param>
	/// <param name="position_two">終点</param>
	static void SetConnectLine(const XMFLOAT3 position_one, const XMFLOAT3 position_two);

	/// <summary>
	/// 敵がはじかれたときのエフェクト
	/// </summary>
	/// <param name="position">敵座標</param>
	/// <param name="radius">吹っ飛ばす角度（ラジアン）</param>
	/// <param name="color">色</param>
	static void SetPushEnemy(const XMFLOAT3 position, const float radius, const XMFLOAT4 color);

	/// <summary>
	/// 壁が破壊されたときのエフェクト
	/// </summary>
	/// <param name="position">出現座標</param>
	static void SetWallBreak(const XMFLOAT3 position);

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

	//コア爆破エフェクト
	static Emitter* titleCoreExplosion;
	//爆破エフェクト時間
	static int explosionTime;
	//爆破エフェクト最大時間
	static const int explosionTimeMax = 100;
	//プレイヤー移動時のエフェクト
	static Emitter* playerMove;
	//出現間隔
	static int playerMoveContro;
	//敵が倒されたときのエフェクト
	static Emitter* enemeyDead;
	//弾消滅時のエフェクト
	static Emitter* playerBulletDelete;
	//線が繋がった時のエフェクト
	static Emitter* connectLine;
	//敵が吹っ飛んだ時のエフェクト
	static Emitter* pushEnemy;
	//壁オブジェクト系のパーティクルテクスチャ数
	static const int wallTexNum = 10;
	//敵が吹っ飛んだ時のエフェクト
	static Emitter* wallBreak[wallTexNum];
};