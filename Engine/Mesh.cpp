#include "pch.h"
#include "Mesh.h"
#include "Engine.h"


void Mesh::Init(vector<Vertex>& vec)
{
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	//device -> ���� ����!! cpu�� �ڷḦ �ѱ�. (GDDR4 RAM ->  DDR2 RAM)
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &vec[0], bufferSize);
	_vertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // ���� 1�� ũ��
	_vertexBufferView.SizeInBytes = bufferSize; // ������ ũ��	
}

void Mesh::Render()
{
	//cmd list -> �ϰ��� command queue�� ��Ҵٰ� �ѹ��� 
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �ﰢ�� ����Ʈ)
	
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15) // _vertexBufferView ���ҽ� ��


	//TODO
	//1)Buffer���ٰ� ������ ����
	//2) Bufferd�� �ּҸ� register���ٰ� ���� 
	//commandque�� �̿��ϸ� ���� �ڷḦ ����. 
	GEngine->GetConstantBuffer()->PushData(0, &_transform,sizeof(_transform));
	GEngine->GetConstantBuffer()->PushData(1, &_transform,sizeof(_transform));
	//CMD_LIST->SetGraphicsRootConstantBufferView(0.????)

	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}