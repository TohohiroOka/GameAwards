#pragma once
#include "Sprite.h"

class Combo
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
	/// コンボ生成
	/// </summary>
	/// <param name="numberTexNum">数字テクスチャ番号</param>
	/// <param name="comboTexNum">Comboテクスチャ番号</param>
	/// <returns>コンボ</returns>
	static Combo* Create(int numberTexNum, int comboTexNum);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Combo();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="numberTexNum">数字テクスチャ番号</param>
	/// <param name="comboTexNum">Comboテクスチャ番号</param>
	/// <returns>成否</returns>
	bool Initialize(int numberTexNum, int comboTexNum);

	/// <summary>
	/// 更新
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
	/// コンボ数を増やす
	/// </summary>
	void AddCombo();

	/// <summary>
	/// コンボ終了
	/// </summary>
	void LostCombo();

	/// <summary>
	/// ゲームシーンの座標に移動状態にセット
	/// </summary>
	void SetMoveGamePos();

	/// <summary>
	/// リザルトシーンの座標に移動状態にセット
	/// </summary>
	void SetMoveResultPos();

	//getter
	int GetCombo() { return combo; }
	int GetMaxCombo() { return maxCombo; }
	bool GetIsMoveGamePosEnd() { return isMoveGamePosEnd; }

protected:
	/// <summary>
	/// コンボ終了タイマー更新
	/// </summary>
	void LostTimerUpdate();

	/// <summary>
	/// 表示用コンボスプライト変更
	/// </summary>
	void ChangeComboSprite();

	/// <summary>
	/// ゲームシーンの座標に移動
	/// </summary>
	void MoveGamePos();

	/// <summary>
	/// リザルトシーンの座標に移動
	/// </summary>
	void MoveResultPos();

private:
	//コンボ数スプライト
	static const int comboDigits = 3;
	Sprite* numberSprite[comboDigits] = { nullptr };
	//COMBOスプライト
	Sprite* comboSprite = nullptr;
	//コンボ数
	int combo = 0;
	//最大コンボ数
	int maxCombo = 0;
	//コンボ終了タイマー
	int lostComboTimer = 0;
	//ゲームシーンの座標に移動中か
	bool isMoveGamePos = false;
	//ゲームシーンの座標に移動終了したか
	bool isMoveGamePosEnd = false;
	//ゲームシーンの座標に移動する時間タイマー
	int moveGamePosTimer = 0;
	//リザルトシーンの座標に移動中か
	bool isMoveResultPos = false;
	//リザルトシーンの座標に移動終了したか
	bool isMoveResultPosEnd = false;
	//リザルトシーンの座標に移動する時間タイマー
	int moveResultPosTimer = 0;
};