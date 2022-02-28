#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>

#include"WindowApp.h"

class DirectXCommon
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DirectXCommon();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DirectXCommon();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApp">ウィンドウズインスタンス</param>
	void Initialize(WindowApp* winApp);

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	/// <param name="winApp">ウィンドウズインスタンス</param>
	void CreateDepth(WindowApp* winApp);

	/// <summary>
	/// 描画前設定
	/// </summary>
	/// <param name="winApp">ウィンドウズインスタンス</param>
	void BeforeDraw(WindowApp* winApp);

	/// <summary>
	/// 描画後設定
	/// </summary>
	void AfterDraw();

	/// <summary>
	/// デバイスのgetter
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device* GetDevice() { return device.Get(); }

	/// <summary>
	/// コマンドリストのgetter
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList* getcmdList() { return cmdList.Get(); }

private:

	//デバイス
	ComPtr<ID3D12Device> device;
	//ファクトリー
	ComPtr<IDXGIFactory6> dxgiFactory;
	//コマンドリスト
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	//コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	//コマンドキュー
	ComPtr<ID3D12CommandQueue> cmdQueue;
	//スワップチェーン
	ComPtr<IDXGISwapChain4> swapchain;
	//バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	//レンダーターゲットビュー用ディスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuffer;
	//深度ステンシルビュー用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	//深度ビュー用のデスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	//フェンス
	ComPtr<ID3D12Fence> fence;
	//コマンドリスト完了までのカウント
	UINT64 fenceVal = 0;
};

