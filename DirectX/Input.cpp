#include "Input.h"
#include "WindowApp.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input *Input::GetInstance()
{
	static Input instance;

	return &instance;
}

void Input::Initialize()
{
	HRESULT result;

	//DirectInputのインスタンス生成
	result = DirectInput8Create(
		WindowApp::GetWinInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);

	///-----------------キー入力初期化-----------------///

	//キーボードデバイスの設定
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	//入力データ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	//排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(
		WindowApp::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);


	///----------------マウス入力初期化----------------///

	//マウスデバイスの設定
	result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);
	//入力データ形式のセット
	result = devmouse->SetDataFormat(&c_dfDIMouse);	//標準形式
	//排他制御レベルのセット
	result = devmouse->SetCooperativeLevel(
		WindowApp::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void Input::Update()
{
	HRESULT result;

	///-----------------キー入力更新-----------------///

	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));
	//キーボード情報の取得開始
	result = devkeyboard->Acquire();
	//全キーの入力状態を取得する
	result = devkeyboard->GetDeviceState(sizeof(key), key);

	///----------------マウス入力更新----------------///

	//前回のマウス入力を保存
	mousePre = mouse;
	//マウス情報の取得開始
	result = devmouse->Acquire();
	//マウスの入力状態を取得する
	result = devmouse->GetDeviceState(sizeof(mouse), &mouse);
	//マウス座標を取得する
	GetCursorPos(&mousePoint);
	//スクリーン座標をクライアント座標に変換する
	ScreenToClient(WindowApp::GetHwnd(), &mousePoint);
}

bool Input::PushKey(BYTE keyNumber)
{
	//指定のキーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	//指定のキーを押した瞬間ならtrueを返す
	if (key[keyNumber] && !keyPre[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::ReleaseKey(BYTE keyNumber)
{
	//指定のキーを離した瞬間ならtrueを返す
	if (!key[keyNumber] && keyPre[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::PushMouseButton(const int mouseButton)
{
	//指定のボタンを押していればtrueを返す
	if (mouse.rgbButtons[mouseButton]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::TriggerMouseButton(const int mouseButton)
{
	//指定のボタンを押した瞬間ならtrueを返す
	if (mouse.rgbButtons[mouseButton] &&
		!mousePre.rgbButtons[mouseButton]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::ReleaseMouseButton(const int mouseButton)
{
	//指定のボタンを離した瞬間ならtrueを返す
	if (!mouse.rgbButtons[mouseButton] &&
		mousePre.rgbButtons[mouseButton]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

DirectX::XMFLOAT2 Input::GetMousePoint()
{
	return DirectX::XMFLOAT2((float)mousePoint.x, (float)mousePoint.y);
}

DirectX::XMFLOAT2 Input::GetMouseVelocity()
{
	return DirectX::XMFLOAT2((float)mouse.lX, (float)mouse.lY);
}

float Input::GetMouseWheelVelocity()
{
	float wheel = mouse.lZ / 120.0f;
	return wheel;
}