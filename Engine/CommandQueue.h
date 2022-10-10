#pragma once
//ID3D12CommandQueue �ǹ�: 
//I�� �������̽� 
//D3D12��  dll �̸� : D3�� 3D�� ���ٷ� �� �� ����..
//D12 �� DirectX12����.

class SwapChain;
class DescriptorHeap;

class CommandQueue
{
public:

	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();

	//���������� ������ �׷����� �κ�
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return _cmdList; }


private:
	//CommandQueue:  DX12�� ����
	// ���� ��û �� �ϳ��� ��û�ϸ� ��ȿ�����̴ϱ�
	//[���� ���]�� �ϰ��� �������� ����ߴٰ� �� �濡 ��û�ϴ� ��
	ComPtr<ID3D12CommandQueue>			_cmdQueue;
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	//Fence : ��Ÿ��(?)
	// CPU/GPU ����ȭ�� ���� ������ ����
	//��⿭�� ���(Ư����������) ����� ó���ϴ� �� = ��� ��⿭�� ���� = �����Ѵ�(flush)
	//-> �� �� �ʿ��Ѱ� fence 
	//Fence�� Ư������(���ַ� �� ����)�� ������༭ fence���� ���� �� flush ��.
	ComPtr<ID3D12Fence>							_fence;
	uint32														_fenceValue = 0;
	HANDLE													_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>							_swapChain;
	shared_ptr<DescriptorHeap>					_descHeap; //back buffer ������

};

