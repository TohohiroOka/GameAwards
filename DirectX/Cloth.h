#pragma once
#include "DrawLine3D.h"

class Camera;

struct Grid
{
public:
	float mass;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 vel;
	DirectX::XMFLOAT3 force;

	Grid() {
		mass = 0.0f;
		pos = { 0.0f,0.0f,0.0f };
		vel = { 0.0f,0.0f,0.0f };
		force = { 0.0f,0.0f,0.0f };
	}
};

typedef std::vector<std::vector<Grid>> Grids;

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
	static const int LineNum = NumGrid - 1;//線の本数
	static const int sub = 4;
	const float gridmass = 0.1f;// 制御点の質量
	XMFLOAT3 gravity;// 重力
	XMFLOAT3 windforce;// 風の強さ
	XMFLOAT3 damping;// 弾性力
	XMFLOAT3 k;// よくわからん
	XMFLOAT3 kd;// よくわからん
	XMFLOAT3 spring_length;// よくわからん
	XMFLOAT3 d_spring_length;// よくわからん
	XMFLOAT3 dt;// よくわからん
	//ここまでの変数の中、適当にいじってもらえたら

	//線の太さ
	float weight;
	//線の色
	XMFLOAT4 color;

	//線
	DrawLine3D* line = nullptr;
};