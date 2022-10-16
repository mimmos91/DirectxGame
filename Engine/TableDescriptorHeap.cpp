#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init(uint32 count)
{
	_groupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = count * REGISTER_COUNT; // view ����
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_groupSize = _handleSize * REGISTER_COUNT; //

}

void TableDescriptorHeap::Clear() 
{
	_currentGroupIndex = 0; // current Index�� ����. ������ ���� ����ϴϱ�.... Ŀ���� ����
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
} //���� �� group index�� �ش��ϴ� �׷� index �ø�.

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
//	//RenderTargetView�� swap chain�� ��������
//	
//	// Descriptor (DX12) = View (~DX11)
//	// [������ ��]���� RTV(RenderTargetView) ����
//	// DX11�� RTV(RenderTargetView), DSV(DepthStencilView), 
//	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)�� ����!
//
//	_rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//
//	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
//	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT; // � ������ٱ��?? ����ü�� ���۸�ŭ��~
//	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
//	rtvDesc.NodeMask = 0;
//
//	// ���� ������ �����ͳ��� �迭�� ����
//	// RTV ��� : [ ] [ ]
//	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap));
//
//	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
//
//	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
//	{
//		//i * _rtvHeapSize : ���� �ִ� �Ϳ��� heapsize�� ���ؼ� 
//		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _rtvHeapSize); 
//		//CD~ d3d12 helper�� �ִ� �͵�
//		//handle ���� ����/ 
//		device->CreateRenderTargetView(swapChain->GetRenderTarget(i).Get(), nullptr, _rtvHandle[i]);
//	}
//}
//
//D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetBackBufferView() // ���� swap chain���� ���� ������.
//{
//	return GetRTV(_swapChain->GetCurrentBackBufferIndex()); //RenderTargetView�� swap chain�� ��������
//}