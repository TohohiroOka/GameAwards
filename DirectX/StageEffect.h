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

private://構造体

	struct SMASH {
		XMFLOAT3 position = {};//出現位置
		XMFLOAT3 velocity = {};//速度
		unsigned int power = 0;
		int maxTime = 0;
		int time = 0;//エフェクトの出る時間
	};

public://静的メンバ関数

	/// <summary>
	/// プレイヤー移動時のエフェクトセット
	/// </summary>
	/// <param name="position">プレイヤーの座標</param>
	/// <param name="rotation">プレイヤーの角度</param>
	static void SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation);

	/// <summary>
	/// 敵がはじかれたときのエフェクト
	/// </summary>
	/// <param name="position">敵座標</param>
	/// <param name="power">敵座標</param>
	static void SetPushEnemy(const XMFLOAT3 position, const unsigned char power);

	/// <summary>
	/// 壁が破壊されたときのエフェクト
	/// </summary>
	/// <param name="position">出現座標</param>
	static void SetWallBreak(const XMFLOAT3 position);

	/// <summary>
	/// 壁に当たった時のエフェクト
	/// </summary>
	/// <param name="position">出現座標</param>
	/// <param name="power">威力</param>
	static void SetSmash(const XMFLOAT3 position, const  unsigned int power);

public://メンバ関数

	StageEffect() {};
	~StageEffect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 壁に当たった時の処理更新
	/// </summary>
	void smashUpdate();

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
	static std::array<Emitter*, wallTexNum> wallEffect;
	//スマッシュ時のエフェクト
	static Emitter* smash;
	//スマッシュエフェクトの情報
	static std::forward_list<SMASH> smashInfo;
};