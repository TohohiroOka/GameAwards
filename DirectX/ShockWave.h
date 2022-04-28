#pragma once
#include "Object3d.h"
#include "GaruEnemy.h"
#include "Charo.h"
#include "Porta.h"

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
	/// 衝撃波発射
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="power">威力</param>
	void WaveStart(XMFLOAT3 position, int power);

	/// <summary>
	/// 死亡
	/// </summary>
	void Dead();

	/// <summary>
	/// 一度きりの判定をする為に、引数のガル族を知っているかどうか判定する
	/// </summary>
	bool IsKnowGaruEnemy(GaruEnemy* garuEnemy);

	/// <summary>
	/// 一度きりの判定をする為に、引数のチャロを知っているかどうか判定する
	/// </summary>
	bool IsKnowCharo(Charo* charo);

	/// <summary>
	/// 一度きりの判定をする為に、引数のポルタを知っているかどうか判定する
	/// </summary>
	bool IsKnowPorta(Porta* porta);

	//getter
	XMFLOAT3 GetPosition() { return shockWaveObject->GetPosition(); }
	float GetRadius() { return shockWaveObject->GetScale().x; }
	int GetPower() { return power; }
	bool GetIsAlive() { return isAlive; }

private:
	/// <summary>
	/// 衝撃波を広げる
	/// </summary>
	void WaveSpread();

private:
	//衝撃波オブジェクト
	Object3d* shockWaveObject = nullptr;
	//衝撃が広がる速度
	float spreadSpeed = 0;
	//攻撃力
	int power = 0;
	//生成からの時間タイマー
	int aliveTimer = 0;
	//生存可能時間
	int aliveTime = 0;
	//生きてるか
	bool isAlive = false;
	//衝撃波が知っているガル族
	std::list <GaruEnemy*> alreadyGaruEnemys;
	//衝撃波が知っているチャロ
	std::list <Charo*> alreadyCharoEnemys;
	//衝撃波が知っているポルタ
	std::list <Porta*> alreadyPortaEnemys;
};