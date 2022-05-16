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
	/// プレイヤー移動時のエフェクトセット
	/// </summary>
	/// <param name="position">プレイヤーの座標</param>
	/// <param name="rotation">プレイヤーの角度</param>
	static void SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation);

	/// <summary>
	/// 壁に当たった時のエフェクト
	/// </summary>
	/// <param name="position">敵の座標</param>
	/// <param name="direction">角度</param>
	static void SetHitWall(const XMFLOAT3 position, const float angle);

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

	//プレイヤー移動時のエフェクト
	static Emitter* generalEffect;
	//出現間隔
	static int playerMoveContro;
	//壁オブジェクト系のパーティクルテクスチャ数
	static const int wallTexNum = 3;
	//敵が吹っ飛んだ時のエフェクト
	static Emitter* wallEffect[wallTexNum];
};