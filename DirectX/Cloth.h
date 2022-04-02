#pragma once
#include "DrawLine3D.h"
#include "Camera.h"
#include <vector>

using namespace std;

struct Grid
{
private:// エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	float mass;
	XMFLOAT3 pos;
	XMFLOAT3 vel;
	XMFLOAT3 force;

	Grid() {
		pos = { 0.0f,0.0f,0.0f };
		vel = { 0.0f,0.0f,0.0f };
		force = { 0.0f,0.0f,0.0f };
	}
};

typedef vector<vector<Grid>> Grids;

class Cloth
{
private:// エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// ぐにょぐにょ線生成
	/// </summary>
	/// <param name="startPoint"> 始点
	/// <param name="endPoint"> 終点
	/// <param name="color"> 色
	/// <param name="width"> 幅
	/// <returns></returns>
	static Cloth* Create(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Cloth();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Cloth();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="startPoint"> 始点
	/// <param name="endPoint"> 終点
	/// <param name="color"> 色
	/// <param name="width"> 幅
	/// <returns></returns>
	bool Init(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//setter
	void SetColor(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width);

private:
	/// <summary>
	/// 力の更新
	/// </summary>
	void UpdateForce();

	/// <summary>
	/// オイラー法
	/// </summary>
	void EularMethod();

	Grids grids;
	int counter;

	//ここから
	static const int NumGrid = 5;// 制御点
	static const int sub = 4;
	const float gridmass = 0.1f;// 制御点の質量
	XMFLOAT3 gravity = { 4.9f ,4.9f ,0.0f };// 重力
	XMFLOAT3 windforce = { 3.0f ,3.0f ,0.0f };// 風の強さ
	XMFLOAT3 damping = { 0.05f ,0.05f ,0.0f };// 弾性力
	XMFLOAT3 k = { 5.0f ,5.0f ,0.0f };// よくわからん
	XMFLOAT3 kd = { 2.0f ,2.0f ,0.0f };// よくわからん
	XMFLOAT3 spring_length = { 2.0f ,2.0f ,0.0f };// よくわからん
	XMFLOAT3 d_spring_length = { spring_length.x * sqrtf(2.0f),spring_length.y * sqrtf(2.0f),0.0f };// よくわからん
	XMFLOAT3 dt = { 0.01f ,0.01f ,0.0f };// よくわからん
	//ここまでの変数の中、適当にいじってもらえたら

	//線の太さ
	float weight = 0.5f;

	XMFLOAT4 color = { 0.4f, 1, 0.2f, 1 };

	//線
	DrawLine3D* line[NumGrid][NumGrid] = { nullptr };
};