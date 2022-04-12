#pragma once
#include "ConnectCircle.h"

class EnemyCircle : public ConnectCircle
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
	/// 敵から出るコネクトサークル生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="garuEnemy">ガル族の敵</param>
	/// <returns>コネクトサークル</returns>
	static EnemyCircle* Create(Model* model, GaruEnemy* garuEnemy);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">ガル族の敵</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model, GaruEnemy* garuEnemy);
};