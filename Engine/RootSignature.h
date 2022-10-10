#pragma once

//[계약서 혹은 결재 같은 것]
//그리기 호출 전에 응용 프로그램이 반드시 랜더링에 묶어야 하는 자원들이 무엇이고 
//그 자원들이 세이더 입력 레지스터들에 어떻게 대응 되는지를 정의
class RootSignature

{
public:
	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	ComPtr<ID3D12RootSignature>	_signature;
};
