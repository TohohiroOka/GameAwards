#pragma once
#include "Object3d.h"
#include "BaseEnemy.h"

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
	enum ShockWaveGroup
	{
		None,			//未設定
		PlayerWave,		//自動衝撃波
		LitteringWave,	//ポイ捨て衝撃波
		BigWave			//巨大衝撃波
	};

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
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// プレイヤーから一定間隔で出る衝撃波発射
	/// </summary>
	void PlayerWaveStart(XMFLOAT3 position);

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
	/// 一度きりの判定をする為に、引数の敵を知っているかどうか判定する
	/// </summary>
	bool IsKnowEnemy(BaseEnemy* enemy);


	//getter
	int GetGroup() { return group; }
	XMFLOAT3 GetPosition() { return shockWaveObject->GetPosition(); }
	float GetRadius() { return shockWaveObject->GetScale().x; }
	int GetPowerLevel() { return powerLevel; }
	float GetPowerMagnification() { return powerMagnification; }
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
	void WaveStartCommon(XMFLOAT3 position);

private:
	//所属グループ
	int group = None;
	//衝撃波オブジェクト
	Object3d* shockWaveObject = nullptr;
	//衝撃が広がる速度
	float spreadSpeed = 0;
	//威力段階
	int powerLevel = 0;
	//威力倍率
	float powerMagnification = 1.0f;
	//生成からの時間タイマー
	int aliveTimer = 0;
	//生存可能時間
	int aliveTime = 0;
	//生きてるか
	bool isAlive = false;
	//衝撃波が知っている敵
	std::list <BaseEnemy*> alreadyEnemys;
};