#pragma once
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include <DirectXMath.h>

/// <summary>
/// 入力
/// </summary>
class Input final
{
public: //エイリアス
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //列挙
	//マウスボタン
	enum MouseButton
	{
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_WHEEL
	};

private: //シングルトン化
	//コンストラクタを隠蔽
	Input() = default;
	//デストラクタを隠蔽
	~Input() = default;

public:
	//コピーコンストラクタを無効化
	Input(const Input &input) = delete;
	//代入演算子を無効化
	void operator = (const Input &input) = delete;

public: //メンバ関数
	
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>入力</returns>
	static Input *GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name = "keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name = "keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// キーを離したかチェック
	/// </summary>
	/// <param name = "keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>離したか</returns>
	bool ReleaseKey(BYTE keyNumber);

	/// <summary>
	/// マウスボタンの押下をチェック
	/// </summary>
	/// <param name = "mouseButton">マウスボタン</param>
	/// <returns>押されているか</returns>
	bool PushMouseButton(const int mouseButton);

	/// <summary>
	/// マウスボタンのトリガーをチェック
	/// </summary>
	/// <param name = "mouseButton">マウスボタン</param>
	/// <returns>トリガーか</returns>
	bool TriggerMouseButton(const int mouseButton);

	/// <summary>
	/// マウスボタンを離したかチェック
	/// </summary>
	/// <param name = "mouseButton">マウスボタン</param>
	/// <returns>離したか</returns>
	bool ReleaseMouseButton(const int mouseButton);

	/// <summary>
	/// マウスポインターの座標を取得
	/// </summary>
	/// <returns>マウスポインターの座標</returns>
	DirectX::XMFLOAT2 GetMousePoint();

	/// <summary>
	/// 1フレームのマウスの移動量の取得
	/// </summary>
	/// <returns>1フレームのマウスの移動量</returns>
	DirectX::XMFLOAT2 GetMouseVelocity();

	/// <summary>
	/// 1フレームのマウスホイールの移動量の取得
	/// </summary>
	/// <returns>1フレームのマウスホイールの移動量</returns>
	float GetMouseWheelVelocity();

private: //メンバ変数
	//DirectInputのインスタンス生成
	ComPtr<IDirectInput8> dinput;
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//全キーの状態
	BYTE key[256] = {};
	//前回の全キーの状態
	BYTE keyPre[256] = {};
	//マウスのデバイス
	ComPtr<IDirectInputDevice8> devmouse;
	//マウスの状態
	DIMOUSESTATE mouse;
	//前回のマウスの状態
	DIMOUSESTATE mousePre;
	//マウスポインター
	POINT mousePoint;
};