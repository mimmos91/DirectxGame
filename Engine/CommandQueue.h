#pragma once
//ID3D12CommandQueue 의미: 
//I는 인터페이스 
//D3D12은  dll 이름 : D3는 3D를 꺼꾸로 한 것 같어..
//D12 는 DirectX12버전.

class SwapChain;
class DescriptorHeap;

class CommandQueue
{
public:

	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();

	//실질적으로 게임이 그려지는 부분
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return _cmdList; }


private:
	//CommandQueue:  DX12에 등장
	// 외주 요청 시 하나씩 요청하면 비효율적이니까
	//[외주 목록]에 일감을 차곡차곡 기록했다가 한 방에 요청하는 것
	ComPtr<ID3D12CommandQueue>			_cmdQueue;
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	//Fence : 울타리(?)
	// CPU/GPU 동기화를 위한 간단한 도구
	//대기열의 모든(특점지점까지) 명령을 처리하는 것 = 명령 대기열을 비운다 = 방출한다(flush)
	//-> 이 때 필요한게 fence 
	//Fence가 특점지점(외주로 준 지점)을 명시해줘서 fence까지 왔을 때 flush 됨.
	ComPtr<ID3D12Fence>							_fence;
	uint32														_fenceValue = 0;
	HANDLE													_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>							_swapChain;
	shared_ptr<DescriptorHeap>					_descHeap; //back buffer 동작함

};

