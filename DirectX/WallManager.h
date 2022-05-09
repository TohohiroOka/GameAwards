#pragma once
#include "WallObject.h"

class WallManager
{
private:// エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:

	//各stepごとのオブジェクト個数
	enum WALL_STEP {
		step1 = 100,
		step2 = 50,
		step3 = 30,
	};

	//壁の情報
	struct STATUS
	{
		bool isAlive = false;//生きているか
		int hp = 10;//ヒットポイント
		WALL_STEP wallNum = WALL_STEP::step1;//壁オブジェクトの個数
	};

public:

	/// <summary>
	/// 壁生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static WallManager* Create();

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WallManager();

	/// <summary>
	/// 毎フレーム処理
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
	/// ダメージを与える
	/// </summary>
	/// <param name="damagePower">ダメージを与える威力</param>
	void Damage(int damagePower);

	/// <summary>
	/// 破壊
	/// </summary>
	void Break();

	/// <summary>
	/// 休憩状態にする
	/// </summary>
	void SetBreakTime();

	/// <summary>
	/// 新たな壁生成状態にする
	/// </summary>
	void SetCreateWall();

	/// <summary>
	/// 破壊された瞬間か
	/// </summary>
	/// <returns></returns>
	bool GetTriggerBreak();

	/// <summary>
	/// リザルトシーン用に動かす状態にする
	/// </summary>
	void SetChangeResult();

	//getter
	int GetHP() { return status.hp; }
	bool GetIsCreate() { return isCreate; }
	bool GetIsAlive() { return status.isAlive; }
	void SetEffect() { isSetEffect = 1; }

private:

	/// <summary>
	/// モデルの読み込み
	/// </summary>
	void LoadModel();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 演出のセット
	/// </summary>
	void SetUpEffect();

	/// <summary>
	/// 壁破壊後から生成までの休憩時間
	/// </summary>
	void BreakTime();

	/// <summary>
	/// 新たな壁生成
	/// </summary>
	void CreateWall();

	/// <summary>
	/// HP割合に応じて色を変化させる
	/// </summary>
	void ChangeColor();

	/// <summary>
	/// リザルトシーン用に動かす
	/// </summary>
	void ChangeResult();

private:

	//モデル
	Model* model[10] = { nullptr };
	//壁の情報
	STATUS status;
	//基準の最大HP
	const int baseMaxHP = 10;
	//壁の最大HP
	int maxHP = 0;
	//休憩時間タイマー
	int breakTimer = 0;
	//休憩中か
	bool isBreakTime = false;
	//壁生成初期サイズ
	XMFLOAT3 createStartScale = {};
	//壁生成最終サイズ
	XMFLOAT3 createEndScale = {};
	//壁生成タイマー
	int createTimer = 0;
	//壁生成回数
	int createCount = 0;
	//壁生成中か
	bool isCreate = false;
	//リザルトシーン用に動かしす時間タイマー
	int changeResultTimer = 0;
	//壁をリザルトシーン用に動かしているか
	bool isChangeResult = false;
	//壊されたか
	bool isBreak = false;
	//オブジェクトのインスタンス
	std::list<std::unique_ptr<WallObject>> object;
	//演出セット時のイテレータ
	std::list<std::unique_ptr<WallObject>>::iterator nowItr;
	//オブジェクトへの演出フラグセット用
	unsigned char isSetEffect = 0;
	//演出開始からの秒数
	int effectTime = 0;
};