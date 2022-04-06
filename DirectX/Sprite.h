#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class DirectXCommon;

/// <summary>
/// スプライト
/// </summary>
class Sprite
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};

	/// <summary>
	/// 定数バッファ用データ構造体
	/// </summary>
	struct ConstBufferData
	{
		XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="texnumber">テクスチャ番号</param>
	/// <param name="filename">画像ファイル名</param>
	/// <returns>成否</returns>
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">左右反転するか</param>
	/// <param name="isFlipY">上下反転するか</param>
	/// <returns>Sprite</returns>
	static Sprite *Create(UINT texNumber, XMFLOAT2 anchorpoint = { 0.5f, 0.5f }, bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// 解放処理
	/// </summary>
	static void AllDelete();

protected: // 静的メンバ変数
	// テクスチャの最大枚数
	static const int srvCount = 512;
	// 頂点数
	static const int vertNum = 4;
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState;
	// 射影行列
	static XMMATRIX matProjection;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff[srvCount];

public: // メンバ関数
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">左右反転するか</param>
	/// <param name="isFlipY">上下反転するか</param>
	/// <returns>成否</returns>
	bool Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	//getter
	const XMFLOAT2 &GetPosition() { return position; }
	const float GetRotation() { return rotation; }
	const XMFLOAT2 &GetSize() { return size; }
	const XMFLOAT2 &GetTexSize() { return texSize; }
	const XMFLOAT2 &GetTexLeftTop() { return texLeftTop; }
	const XMFLOAT4 &GetColor() { return color; }
	const XMFLOAT2 &GetAnchorpoint() { return anchorpoint; }
	const bool GetIsFlipX() { return isFlipX; }
	const bool GetIsFlipY() { return isFlipY; }

	//setter
	void SetTexNumber(UINT texNumber) { this->texNumber = texNumber; };
	void SetPosition(const XMFLOAT2& position) { this->position = position; }
	void SetRotation(float rotation) { this->rotation = rotation; }
	void SetSize(const XMFLOAT2& size) { this->size = size; }
	void SetTexSize(const XMFLOAT2& texSize) { this->texSize = texSize; }
	void SetTexLeftTop(const XMFLOAT2& texLeftTop) { this->texLeftTop = texLeftTop; }
	void SetColor(const XMFLOAT4& color) { this->color = color; }
	void SetAnchorpoint(const XMFLOAT2& anchorpoint) { this->anchorpoint = anchorpoint; }
	void SetIsFlipX(bool isFlipX) { this->isFlipX = isFlipX; };
	void SetIsFlipY(bool isFlipY) { this->isFlipY = isFlipY; };

protected: // メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// テクスチャ番号
	UINT texNumber = 0;
	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	XMFLOAT2 position{ 0, 0 };
	// スプライト幅、高さ
	XMFLOAT2 size = { 100.0f, 100.0f };
	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ワールド行列
	XMMATRIX matWorld{};
	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ始点
	XMFLOAT2 texLeftTop = { 0, 0 };
	// テクスチャ幅、高さ
	XMFLOAT2 texSize = { 500.0f, 500.0f };

protected: // メンバ関数
	/// <summary>
	/// 頂点データ転送
	/// </summary>
	void TransferVertices();
};