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
	static ShockWave* Create(Model* waveModel, XMFLOAT3 position, int power);

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
	bool Initialize(Model* waveModel, XMFLOAT3 position, int power);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 削除
	/// </summary>
	void SetDelete();

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
	XMFLOAT3 GetPosition() { return position; }
	float GetRadius() { return radius; }
	int GetPower() { return power; }
	bool GetIsDelete() { return isDelete; }

private:
	/// <summary>
	/// 衝撃波を広げる
	/// </summary>
	void WaveSpread();

private:
	//衝撃波オブジェクト
	Object3d* shockWaveObject = nullptr;
	//座標
	XMFLOAT3 position = {};
	//衝撃波円の半径
	float radius = 0;
	//衝撃が広がる速度
	float spreadSpeed = 1;
	//衝撃が広がる速度の速度
	float spreadSpeedAccle = 0;
	//攻撃力
	int power = 0;
	//生成からの時間タイマー
	int aliveTimer = 0;
	//削除するか
	bool isDelete = false;
	//衝撃波が知っているガル族
	std::list <GaruEnemy*> alreadyGaruEnemys;
	//衝撃波が知っているチャロ
	std::list <Charo*> alreadyCharoEnemys;
	//衝撃波が知っているポルタ
	std::list <Porta*> alreadyPortaEnemys;
};