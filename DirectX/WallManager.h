#pragma once
#include "WallObject.h"
#include <array>

class WallManager
{
private:// エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:

	//各stepごとのオブジェクト個数
	enum class WALL_STEP {
		step1 = 120,
		step2 = 100,
		step3 = 80,
		step4 = 0,
	};

	//壁の情報
	struct STATUS
	{
		bool isUpdate = false;
		bool isAlive = false;//生きているか
		int maxHP = 0;//現在の最大ヒットポイント
		int hp = 10;//ヒットポイント
		WallManager::WALL_STEP wallNum = WallManager::WALL_STEP::step1;//壁オブジェクトの個数
		bool isBreak = false;//壊されたか
		bool isCreate = false;//壁生成中か
	};

	//演出の順番
	enum EFFECT_NUM
	{
		NONE,//ゲーム前待機
		SET_FIXED_POSITION_START,//スタート時の演出
		WAIT,//ゲーム中の待機
		SET_FIXED_POSITION_PLAY,//プレイ時の演出
		FALL_WAIT,//落下中の待機
		OUT_SCREEN,//ゲーム終了時に壁が画面外に行く
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
	/// <param name="allReset">リプレイ(false)に初期化内容を少し変える</param>
	void Reset(bool allReset = true);

	/// <summary>
	/// ダメージを与える
	/// </summary>
	/// <param name="damagePower">ダメージを与える威力</param>
	void Damage(int damagePower);

	/// <summary>
	/// 破壊された瞬間か
	/// </summary>
	/// <returns></returns>
	bool GetTriggerBreak();

	//getter
	int GetHP() { return status.hp; }
	bool SetIsUpdate() { return status.isAlive; }
	bool GetIsCreate() { return status.isCreate; }
	bool GetIsAlive() { return status.isAlive; }
	unsigned char GetIsSetEffect() { return isSetEffect; }
	void SetOutScreen() { isSetEffect = EFFECT_NUM::OUT_SCREEN; }

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
	/// 新たな壁生成
	/// </summary>
	void CreateWall();

	/// <summary>
	/// HP割合に応じて破壊
	/// </summary>
	bool PercentageDestruction();

private:

	//モデル
	std::array<Model*, 10> model = { nullptr };
	//壁の情報
	STATUS status;
	//基準の最大HP
	const int baseMaxHP = 10;
	//壁破壊回数
	unsigned int breakCount = 0;
	//生成用カウント
	int createCount = 0;
	//オブジェクトのインスタンス
	std::array<WallObject*, (int)WALL_STEP::step1> object;
	//オブジェクトへの演出フラグセット用カウント
	unsigned char objectCount = 0;
	//オブジェクトへの演出フラグセット用
	unsigned char isSetEffect = 0;
	//演出開始からの秒数
	int effectCount = 0;
	//サウンドの再生用
	int sound[2];
	bool isSound = false;//壁生成音のフラグ
	//0:壁生成音 1:壁破壊音
};