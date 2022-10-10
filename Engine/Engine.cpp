#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "SwapChain.h"
#include "CommandQueue.h"
#include "RootSignature.h"


void Engine::Init(const WindowInfo& window)
{
	_window = window;
	ResizeWindow(window.width, window.height);

	//�׷��� ȭ�� ũ�⸦ ����
	_viewport = { 0,0,static_cast<FLOAT>(window.width),static_cast<FLOAT>(window.height),0.0f,1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, window.width, window.height);


	// viewport: left top front right bottom back


	_device = make_shared<Device>();
	_cmdQueue = make_shared<CommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_rootSignature = make_shared<RootSignature>();
	//_descHeap = make_shared<DescriptorHeap>();
	_constantBuffer = make_shared<ConstantBuffer>();

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(window,_device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init(_device->GetDevice());
	_constantBuffer->Init(sizeof(Transform), 256); // �ʹ� �þ�� ȿ�������� ����

	//_descHeap->Init(_device->GetDevice(), _swapChain);

}



void Engine::Render()
{
	RenderBegin();


	RenderEnd();
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport,&_scissorRect);

}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{

	_window.width = width;
	_window.height = height;

	RECT rect = {0, 0, width, height};
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);

}