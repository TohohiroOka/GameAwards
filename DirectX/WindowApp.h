#pragma once
#include <Windows.h>

class WindowApp
{
public:
	//ウィンドウプロ―ジャの生成
	static LRESULT WindowProcdure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	MSG msg = {};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="windowWidth">画面幅</param>
	/// <param name="windowHeight">画面高さ</param>
	/// <param name="gameName">タイトルバー名</param>
	void Initialize(const int windowWidth, const int windowHeight, const wchar_t* gameName);

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>アプリケーションを終了するか否か</returns>
	bool Update();

	/// <summary>
	/// 登録解除
	/// </summary>
	void Release();

private:

	//横幅
	static UINT windowWidth;
	//立幅
	static UINT windowHeight;
	//ウィンドウクラスの設定
	static WNDCLASSEX winClass;
	//ウィンドウオブジェクト
	static HWND hwnd;

public:

	/// <summary>
	/// ウィンドウオブジェクトの取得
	/// </summary>
	/// <returns>hwnd</returns>
	static HWND GetHwnd() { return hwnd; }

	/// <summary>
	/// ウィンドウクラス設定の取得
	/// </summary>
	/// <returns>hInstance</returns>
	static HINSTANCE GetWinInstance() { return winClass.hInstance; }

	/// <summary>
	/// 横幅の取得
	/// </summary>
	/// <returns>windowWidth</returns>
	static UINT GetWindowWidth() { return windowWidth; }

	/// <summary>
	/// 縦幅の取得
	/// </summary>
	/// <returns>windowHeight</returns>
	static UINT GetWindowHeight() { return windowHeight; }
};

