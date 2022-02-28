#pragma once
#include"WindowApp.h"
#include <DirectXMath.h>
#include<wrl.h>
#include<dinput.h>
#define DIRECTINPUT_VERSION 0x0800

class Input final
{
public:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

private://シングルトンパターン
	Input() = default;
	~Input() = default;

public://シングルトンパターン
	Input(const Input& input) = delete;
	Input& operator=(const Input& input) = delete;

	static Input* GetInstance();

public://メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApp">ウィンドウズインスタンス</param>
	void Initialize(WindowApp* winApp);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 特定のキーが押されているかのチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0等)</param>
	/// <returns>押されているならtrue</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0等)</param>
	/// <returns>トリガーで押されているならtrue</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// マウスの左ボタンが押されているかのチェック
	/// </summary>
	/// <returns>押されているならtrue</returns>
	bool PushMouseLeft();

	/// <summary>
	/// マウスのカーソルボタンが押されているかのチェック
	/// </summary>
	/// <returns>押されているならtrue</returns>
	bool PushMouseCenter();

	/// <summary>
	/// マウスの右ボタンが押されているかのチェック
	/// </summary>
	/// <returns>押されているならtrue</returns>
	bool PushMouseRight();

	/// <summary>
	/// トリガーでのマウスの左ボタンが押されているかのチェック
	/// </summary>
	/// <returns>トリガーで押されているならtrue</returns>
	bool TriggerMouseLeft();

	/// <summary>
	/// トリガーでのマウスのカーソルボタンが押されているかのチェック
	/// </summary>
	/// <returns>トリガーで押されているならtrue</returns>
	bool TriggerMouseCenter();

	/// <summary>
	/// トリガーでのマウスの右ボタンが押されているかのチェック
	/// </summary>
	/// <returns>トリガーで押されているならtrue</returns>
	bool TriggerMouseRight();

	/// <summary>
	/// 左スティックの傾け方向をチェック
	/// </summary>
	/// <returns>押し込み右なら+1、左なら-1</returns>
	char LEFT_PadStickX();

	/// <summary>
	/// 左スティックの傾け方向をチェック
	/// </summary>
	/// <returns>押し込み下なら+1、上なら-1</returns>
	char LEFT_PadStickY();

	/// <summary>
	/// 右スティックの傾け方向をチェック
	/// </summary>
	/// <returns>押し込み右なら+1、左なら-1</returns>
	char RIGHT_PadStickX();

	/// <summary>
	/// 右スティックの傾け方向をチェック
	/// </summary>
	/// <returns>押し込み下なら+1、上なら-1</returns>
	char RIGHT_PadStickY();

	/// <summary>
	/// ゲームパッドのボタンチェック
	/// </summary>
	/// <returns>特定のボタンが押されているかのチェック</returns>
	bool PushPadButtons(BYTE keyNumber);

	/// <summary>
	/// トリガーでのゲームパッドのボタンチェック
	/// </summary>
	/// <returns>特定のボタンが押されているかのチェック</returns>
	bool TriggerPadButtons(BYTE keyNumber);

public://enum
	
	//ゲームパッドキーコンフィグ
	enum PudButton {
		PAD_A = 0,//0
		PAD_B,//1
		PAD_X,//2
		PAD_Y,//3
		PAD_LB,//4
		PAD_RB,//5
		PAD_BUCK,//6
		PAD_START,//7
		PAD_PUSH_L_STICK,//8
		PAD_PUSH_R_STICK,//9
		PAD_UP,//10
		PAD_LEFT,//11
		PAD_DOWN,//12
		PAD_RIGHT,//13
		//LT,RTがわからない
	};

private://変数
	//キー
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8>devkeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};//前回キーの状態

	//マウス
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};

	//ゲームパッド
	ComPtr<IDirectInputDevice8> devStick;
	LPVOID parameter;
	DIJOYSTATE  padData;
	DIJOYSTATE  padDataPre;
	bool isPad = false;
};