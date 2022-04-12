#pragma once
#include "Object3d.h"
#include "GaruEnemy.h"
#include "FixedEnemy.h"

class ConnectCircle
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
	/// デストラクタ
	/// </summary>
	~ConnectCircle();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// サイズを大きくする
	/// </summary>
	void BigRadius();

	/// <summary>
	/// サイズを小さくする
	/// </summary>
	void SmallRadius();

	/// <summary>
	/// 保持しているガル族の敵と引数のガル族の敵を使用しているか確認
	/// </summary>
	/// <param name="garuEnemy">ガル族の敵</param>
	/// <returns>保持しているガル族の敵と引数のガル族の敵を使用しているか</returns>
	bool CheckUseGaruEnemy(GaruEnemy* garuEnemy);

	/// <summary>
	/// 保持している固定敵と引数の固定敵を使用しているか確認
	/// </summary>
	/// <param name="fixedObject">固定敵</param>
	/// <returns>保持している固定敵と引数の固定敵を使用しているか</returns>
	bool CheckUseFixedEnemy(FixedEnemy* fixedEnemy);

	/// <summary>
	/// 削除
	/// </summary>
	void SetDelete();

	//getter
	XMFLOAT3 GetPosition() { return circleObject->GetPosition(); }
	float GetRadius() { return radius; }
	bool GetIsChangeRadius() { return isChangeRadius; }
	bool GetIsDelete() { return isDelete; }

protected:
	/// <summary>
	/// 半径の大きさ変更をセット
	/// </summary>
	void SetChangeRadius();

	/// <summary>
	/// 半径の大きさを変更
	/// </summary>
	void ChangeRadius();

protected:
	//円オブジェクト
	Object3d* circleObject = nullptr;
	//円の中心となるガル族の敵(死亡)
	GaruEnemy* garuEnemy = nullptr;
	//円の中心となる固定敵
	FixedEnemy* fixedEnemy = nullptr;
	//基準の半径
	float baseRadius;
	//円の半径
	float radius = 0;
	//変更前の円のサイズ
	float changeRadiusStart = 0;
	//変更後の円のサイズ
	float changeRadiusEnd = 0;
	//円のサイズ変更用のタイマー
	int changeRadiusTimer = 0;
	//自分から線が何本出ているかカウント
	int countLine = 0;
	//サイズを変更中か
	bool isChangeRadius = false;
	//削除するか
	bool isDelete = false;
};