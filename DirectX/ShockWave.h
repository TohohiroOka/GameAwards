#pragma once
#include "Object3d.h"

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
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 衝撃波開始をセット
	/// </summary>
	void SetWaveStart(XMFLOAT3 startPos);

	//getter
	XMFLOAT3 GetPosition() { return position; }
	float GetRadius() { return radius; }

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
	float spreadSpeed = 0;
	//衝撃が広がる速度の速度
	float spreadSpeedAccle = 0;
	//衝撃波を広げるか
	bool isSpreadWave = false;
};