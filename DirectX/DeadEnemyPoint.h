#pragma once
#include "Object3d.h"
#include "BaseEnemy.h"

class DeadEnemyPoint
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
	/// 敵の死亡位置生成
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>敵の死亡位置</returns>
	static DeadEnemyPoint *Create(Model *model, BaseEnemy *enemy, float radius);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~DeadEnemyPoint();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <returns>成否</returns>
	bool Initialize(Model *model, BaseEnemy *enemy, float radius);

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
	/// 保持している敵と引数の敵を使用しているか確認
	/// </summary>
	/// <param name="enemy">敵</param>
	/// <returns>保持している敵と引数の敵を使用しているか</returns>
	bool CheckUseEnemy(BaseEnemy *enemy);

	/// <summary>
	/// 削除
	/// </summary>
	void SetDelete();

	//getter
	XMFLOAT3 GetPosition() { return deadPointObject->GetPosition(); }
	float GetRadius() { return radius; }
	bool GetIsChangeRadius() { return isChangeRadius; }
	bool GetIsDelete() { return isDelete; }

private:
	/// <summary>
	/// 半径の大きさ変更
	/// </summary>
	void ChangeRadius();

private:
	//死んだ位置オブジェクト
	Object3d *deadPointObject = nullptr;
	//円の中心となる敵(死亡)
	BaseEnemy *enemy = nullptr;
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