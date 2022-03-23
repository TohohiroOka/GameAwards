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

	/// <summary>
	/// プレイヤー移動時のエフェクトセット
	/// </summary>
	/// <param name="position">プレイヤーの座標</param>
	/// <param name="rotation">プレイヤーの角度</param>
	void SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation);

	/// <summary>
	/// 敵が倒されたときのエフェクト
	/// </summary>
	/// <param name="position">敵の座標</param>
	/// <returns>生成個数</returns>
	int SetEnemeyDead1(const XMFLOAT3 position);

private:

	//プレイヤー移動時のエフェクト
	Emitter* playerMove = nullptr;
	//出現間隔
	int playerMoveContro = 0;
	//敵が倒されたときのエフェクト
	Emitter* enemeyDead1 = nullptr;

};

