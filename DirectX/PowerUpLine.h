#pragma once
#include "DrawLine3D.h"
#include "ConnectCircle.h"
#include "Camera.h"

class PowerUpLine
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
	/// パワーアップ線生成
	/// </summary>
	/// <param name="startPoint">始点に使用するコネクトサークル</param>
	/// <param name="endPoint">終点に使用するコネクトサークル</param>
	/// <returns>パワーアップ線</returns>
	static PowerUpLine *Create(ConnectCircle* startPoint, ConnectCircle* endPoint);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PowerUpLine();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="startPoint">始点に使用するコネクトサークル</param>
	/// <param name="endPoint">終点に使用するコネクトサークル</param>
	/// <returns>成否</returns>
	bool Initialize(ConnectCircle* startPoint, ConnectCircle* endPoint);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Camera *camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 既に線が作られているかをチェック
	/// </summary>
	bool IsAlreadyCreateLine(ConnectCircle* startPoint, ConnectCircle* endPoint);

	/// <summary>
	/// 始点か終点に引数のコネクトサークルを使用しているか確認
	/// </summary>
	/// <param name="point">コネクトサークル</param>
	/// <returns>始点か終点に引数のコネクトサークルを使用しているか</returns>
	bool CheckUsePoints(ConnectCircle* point);

	/// <summary>
	/// 削除
	/// </summary>
	void SetDelete();


	void SetColor(XMFLOAT4 color);
	//getter
	XMFLOAT3 GetStartPoint() { return startPoint->GetPosition(); }
	XMFLOAT3 GetEndPoint() { return endPoint->GetPosition(); }
	bool GetIsDelete() { return isDelete; }

private:
	//線
	DrawLine3D* line = nullptr;
	//線の始点で使用するコネクトサークル
	ConnectCircle* startPoint = nullptr;
	//線の終点で使用するコネクトサークル
	ConnectCircle* endPoint = nullptr;
	//線の太さ
	float weight = 0.5f;
	//削除するか
	bool isDelete = false;
};