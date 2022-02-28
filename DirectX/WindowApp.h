#pragma once
#include <Windows.h>

class WindowApp
{
public:
	int windowWidth;
	int windowHeight;
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

	//外部で使う引数を渡す
	HWND GetHwnd() { return hwnd; }
	HINSTANCE getWinInstance() { return winClass.hInstance; }

private:
	WNDCLASSEX winClass{}; // ウィンドウクラスの設定
	HWND hwnd = nullptr;
};

