#pragma once

#include <DirectXMath.h>

/// <summary>
/// カメラ基本機能
/// </summary>
class Camera
{
protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Camera() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 1人称カメラの更新
	/// </summary>
	/// <param name="position">移動量</param>
	/// <param name="move">移動量</param>
	/// <param name="speed">感度</param>
	void FpsCamera(XMFLOAT3& position, XMFLOAT3& move, float speed);

	/// <summary>
	/// 3人称カメラの更新
	/// </summary>
	/// <param name="distance">追従する物との距離</param>
	void TpsCamera(XMFLOAT3 distance);

	/// <summary>
	/// カメラのシェイク開始
	/// </summary>
	/// <param name="strength">シェイクの強さ</param>
	void StartCameraShake(int strength);

	/// <summary>
	/// カメラのシェイク終了
	/// </summary>
	void EndCameraShake();
	
	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	inline const XMMATRIX& GetView() { return matView; }

	/// <summary>
	/// 射影行列の取得
	/// </summary>
	/// <returns>射影行列</returns>
	inline const XMMATRIX& GetProjection() { return matProjection; }

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	inline const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	inline const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// 上方向ベクトルの取得
	/// </summary>
	/// <returns>上方向ベクトル</returns>
	inline const XMFLOAT3& GetUp() { return up; }

	/// <summary>
	/// 追従オブジェクトの座標セット
	/// </summary>
	/// <param name="position">追従オブジェクトの座標</param>
	void SetPosition(XMFLOAT3 position) { this->position=position; }

protected: // メンバ変数
	// ビュー行列
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// 射影行列
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	//追従オブジェクト座標
	XMFLOAT3 position = {};
	// 視点座標
	XMFLOAT3 eye = { 0, 0, -20 };
	// 注視点座標
	XMFLOAT3 target = { 0, 0, 0 };
	// 上方向ベクトル
	XMFLOAT3 up = { 0, 1, 0 };
	// アスペクト比
	float aspectRatio = 1.0f;
	//シェイク座標
	XMFLOAT3 ShakeDifference = {};
};