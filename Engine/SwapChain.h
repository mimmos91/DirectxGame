#pragma once
// ��ȯ �罽
// [���� ����] ���� ���� ���� �ִ� ��Ȳ�� ����: ����
// 1. � �������� ��� ������� ������
// 2. GPU�� ������ ��� (����)
// 3. ����� �޾Ƽ� ȭ�鿡 �׷��ش�

// [���� �����]�� ��� ����?
// - � ����(Buffer)�� �׷��� �ǳ��޶�� ��Ź�غ���
// - Ư�� ���̸� ���� -> ó���� �ǳ��ְ� -> ������� �ش� ���̿� �޴´� OK
// - �츮 ȭ�鿡 Ư�� ����(���� �����) ������ش�

// [???]: - �׷��� ȭ�鿡 ���� ����� ����ϴ� ���߿�, ���� ȭ�鵵 ���ָ� �ðܾ� ��
// - ���� ȭ�� ������� �̹� ȭ�� ��¿� �����
// SO...
// - Ư�� ����(buffer)�� 2�� ����, �ϳ��� ���� ȭ���� �׷��ְ�, �ϳ��� ���� �ñ��(��� �Դٰ���)
// - Double Buffering!

// - [1] [2] -> swapChain
// ���� ȭ�� [1]  <-> GPU �۾��� [2] BackBuffer

class SwapChain
{
public:
	
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void Present();
	void SwapIndex();

	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }
	ComPtr<ID3D12Resource> GetRenderTarget(int32 index) { return _rtvBuffer[index]; }

	uint32 GetCurrentBackBufferIndex() { return _backBufferIndex; }
	ComPtr<ID3D12Resource> GetBackRTVBuffer() { return _rtvBuffer[_backBufferIndex]; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV() { return _rtvHandle[_backBufferIndex]; }

private:
	void CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void CreateRTV(ComPtr<ID3D12Device> device);


private:
	ComPtr<IDXGISwapChain>		_swapChain;
	ComPtr<ID3D12Resource>		_rtvBuffer[SWAP_CHAIN_BUFFER_COUNT];
	//render: �׸��� tartget: ����
	//SWAP_CHAIN_BUFFER_COUNT ���� �̰� ���µ� �������ϱ� engine.h�� enum�� ������~!!
	uint32										_backBufferIndex = 0;
	//���� ���۾��� �ϰ� �ִ� index -> 0,1,0,1 �� �ٲ���.


	ComPtr<ID3D12DescriptorHeap>			_rtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];




};

