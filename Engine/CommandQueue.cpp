#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent); //�̺�Ʈ Ȱ�� �� ������ ��. �� ���Ѵٰ� �ص� ����� ����
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;
	//_descHeap = descHeap;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	/*CreateCommandQueue(
	 D3D12_COMMAND_QUEUE_DESC		*pDesc,				:
     REFIID													riid,						: �����ϰ��� �ϴ� ��ɸ�Ͽ� �ش��ϴ� ID3D12CommendList �������̽� COMID
 	 void**													ppCommandList	: ������ ��� ����� ����Ű�� ������( ��� �Ű�����)
	 )*/
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));

	/*CreateCommandQueue(
	 D3D12_COMMAND_LIST_TYPE			type,							: ��� ��� ���� DIRECT or BUNDLE
	 REFIID													riid,								: �����ϰ��� �ϴ� ��ɸ�Ͽ� �ش��ϴ� ID3D12CommendList �������̽� COMID
	 void**													ppCommandAllocator	: ������ ��� �Ҵ��ڸ� ����Ű�� ������(��¸Ű�����)
	 )*/
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));


	/*CreateCommandList(
	 UINT												nodeMask,					:GPU�� �ϳ��� �ý��׿����� 0����
	 D3D12_COMMAND_LIST_TYPE		type,							: ��� ��� ���� DIRECT or BUNDLE
	 ID3D12CommandAllocator*			pCommandAllocator,	: ������ ��� ��Ͽ� ������ų �Ҵ��� ALLOCATOR
	 ID3D12PipelineState*						pInitialState,				: ��� ����� �ʱ� ���������� ���� ����(�׸����� x nullptr)
	 REFIID												riid,								: �����ϰ��� �ϴ� ��ɸ�Ͽ� �ش��ϴ� ID3D12CommendList �������̽� COMID
	 void**												ppCommandList			: ������ ��� ����� ����Ű�� ������( ��� �Ű�����)
	)*/	
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));
	
	// CommandList�� Close / Open ���°� �ִµ�
	// Open ���¿��� Command�� �ִٰ� Close�� ���� �����ϴ� ����
	_cmdList->Close();

	//CreateFence
	//-CPU�� GPU ����ȭ ����
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

}

void CommandQueue::WaitSync()
{
	//��Ƽ������ ���� �� ����ȭ�ϱ� cpu�� GPU ���Ҷ����� ��ٷȴٰ� �ٽ� �����ϴ� ��
	// 
	// Advance the fence value to mark commands up to this fence point.
	_fenceValue++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		// Fire event when GPU hits current fence.  
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}


void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	_cmdAlloc->Reset(); // ������ Ŭ����
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT, // ȭ�� ���
		D3D12_RESOURCE_STATE_RENDER_TARGET); // ���� �����

	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());
	GEngine->GetConstantBuffer()->Clear();
	GEngine->GetTableDescHeap()->Clear();

	ID3D12DescriptorHeap* descHeap = GEngine->GetTableDescHeap()->GetDescriptorHeap().Get();
	_cmdList->SetDescriptorHeaps(1, &descHeap);


	_cmdList->ResourceBarrier(1, &barrier);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// Specify the buffers we are going to render to.
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);
}

void CommandQueue::RenderEnd()
{
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // ���� �����
		D3D12_RESOURCE_STATE_PRESENT); // ȭ�� ��� -> �ڿ��ִ� �� ������ �ű� �� swap.

	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close();

	// Ŀ�ǵ� ����Ʈ ����
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	_swapChain->Present();

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	WaitSync();

	_swapChain->SwapIndex();
}
