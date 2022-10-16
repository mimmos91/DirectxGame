#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init(uint32 count)
{
	_groupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = count * REGISTER_COUNT; // view 갯수
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_groupSize = _handleSize * REGISTER_COUNT; //

}

void TableDescriptorHeap::Clear() 
{
	_currentGroupIndex = 0; // current Index와 동일. 다음애 한테 줘야하니까.... 커서랑 동일
}

void TableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}



void TableDescriptorHeap::CommitTable()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _descHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	CMD_LIST->SetGraphicsRootDescriptorTable(0, handle);

	_currentGroupIndex++;
} //현재 내 group index에 해당하는 그룹 index 올림.

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint32>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(uint32 reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	handle.ptr += reg * _handleSize;
	return handle;
}
























//void DescriptorHeap::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
//{
//	_swapChain = swapChain;
//	//RenderTargetView는 swap chain에 만들어놨음
//	
//	// Descriptor (DX12) = View (~DX11)
//	// [서술자 힙]으로 RTV(RenderTargetView) 생성
//	// DX11의 RTV(RenderTargetView), DSV(DepthStencilView), 
//	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)를 전부!
//
//	_rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//
//	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
//	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT; // 몇개 만들어줄까요?? 스왑체인 버퍼만큼요~
//	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
//	rtvDesc.NodeMask = 0;
//
//	// 같은 종류의 데이터끼리 배열로 관리
//	// RTV 목록 : [ ] [ ]
//	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap));
//
//	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
//
//	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
//	{
//		//i * _rtvHeapSize : 지금 있는 것에서 heapsize를 곱해서 
//		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _rtvHeapSize); 
//		//CD~ d3d12 helper에 있는 것들
//		//handle 원격 개념/ 
//		device->CreateRenderTargetView(swapChain->GetRenderTarget(i).Get(), nullptr, _rtvHandle[i]);
//	}
//}
//
//D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetBackBufferView() // 현재 swap chain에서 빼서 보여줌.
//{
//	return GetRTV(_swapChain->GetCurrentBackBufferIndex()); //RenderTargetView는 swap chain에 만들어놨음
//}