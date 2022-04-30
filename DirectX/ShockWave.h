#pragma once
#include "Object3d.h"
#include "GaruEnemy.h"

class ShockWave
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
	/// 衝撃波生成
	/// </summary>
	/// <param name="waveModel">モデル</param>
	/// <returns>衝撃波</returns>
	static ShockWave* Create(Model* waveModel);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ShockWave();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="waveModel">モデル</param>
	/// <returns>成否</returns>
	bool Initialize(Model* waveModel);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーから一定間隔で出る衝撃波発射
	/// </summary>
	void PlayerWaveStart(XMFLOAT3 position);

	/// <summary>
	/// ポイ捨て衝撃波発射
	/// </summary>
	void LitteringWaveStart(XMFLOAT3 position);

	/// <summary>
	/// 巨大衝撃波発射
	/// </summary>
	/// <param name="position"></param>
	void BigWaveStart(XMFLOAT3 position, int powerLevel);

	/// <summary>
	/// 死亡
	/// </summary>
	void Dead();

	/// <summary>
	/// 一度きりの判定をする為に、引数のガル族を知っているかどうか判定する
	/// </summary>
	bool IsKnowGaruEnemy(GaruEnemy* garuEnemy);


	//getter
	XMFLOAT3 GetPosition() { return shockWaveObject->GetPosition(); }
	float GetRadius() { return shockWaveObject->GetScale().x; }
	int GetPowerLevel() { return powerLevel; }
	bool GetIsAlive() { return isAlive; }

private:
	/// <summary>
	/// 衝撃波を広げる
	/// </summary>
	void WaveSpread();

	/// <summary>
	/// 衝撃波発射共通処理
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="power">威力段階</param>
	void WaveStartCommon(XMFLOAT3 position, int powerLevel);

private:
	//衝撃波オブジェクト
	Object3d* shockWaveObject = nullptr;
	//衝撃が広がる速度
	float spreadSpeed = 0;
	//威力段階
	int powerLevel = 0;
	//生成からの時間タイマー
	int aliveTimer = 0;
	//生存可能時間
	int aliveTime = 0;
	//生きてるか
	bool isAlive = false;
	//衝撃波が知っているガル族
	std::list <GaruEnemy*> alreadyGaruEnemys;
};