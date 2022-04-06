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
	/// 敵が倒されたときのエフェクト
	/// </summary>
	/// <param name="position">敵の座標</param>
	/// <returns>生成個数</returns>
	static int SetEnemeyDead(const XMFLOAT3 position);

	/// <summary>
	/// 弾消滅時のエフェクト
	/// </summary>
	/// <param name="position">弾の座標</param>
	static void SetPlayerBulletDelete(const XMFLOAT3 position);

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
	static Emitter* playerMove;
	//出現間隔
	static int playerMoveContro;
	//敵が倒されたときのエフェクト
	static Emitter* enemeyDead;
	//弾消滅時のエフェクト
	static Emitter* playerBulletDelete;

};

