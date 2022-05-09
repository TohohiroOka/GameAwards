#pragma once
#include"Sprite.h"

class UIFrame
{
private: // GCAX
// Microsoft::WRL::ðÈª
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::ðÈª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// UIðÍ¤g¶¬
	/// </summary>
	/// <returns>UIðÍ¤g</returns>
	static UIFrame* Create(int frameTexNum);

public:
	/// <summary>
	/// fXgN^
	/// </summary>
	~UIFrame();

	/// <summary>
	/// ú»
	/// </summary>
	/// <returns>¬Û</returns>
	bool Initialize(int frameTexNum);

	/// <summary>
	/// t[
	/// </summary>
	void Update();

	/// <summary>
	/// `æ
	/// </summary>
	void Draw();

private:
	//gXvCg
	Sprite* frameSprite = nullptr;
};