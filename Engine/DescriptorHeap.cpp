#include "pch.h"
#include "SwapChain.h"

void DescriptorHeap::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;
	//RenderTargetView는 swap chain에 만들어놨음
	
	// Descriptor (DX12) = View (~DX11)
	// [서술자 힙]으로 RTV(RenderTargetView) 생성
	// DX11의 RTV(RenderTargetView), DSV(DepthStencilView), 
	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)를 전부!

	_rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT; // 몇개 만들어줄까요?? 스왑체인 버퍼만큼요~
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;

	// 같은 종류의 데이터끼리 배열로 관리
	// RTV 목록 : [ ] [ ]
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		//i * _rtvHeapSize : 지금 있는 것에서 heapsize를 곱해서 
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _rtvHeapSize); 
		//CD~ d3d12 helper에 있는 것들
		//handle 원격 개념/ 
		device->CreateRenderTargetView(swapChain->GetRenderTarget(i).Get(), nullptr, _rtvHandle[i]);
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetBackBufferView() // 현재 swap chain에서 빼서 보여줌.
{
	return GetRTV(_swapChain->GetCurrentBackBufferIndex()); //RenderTargetView는 swap chain에 만들어놨음
}