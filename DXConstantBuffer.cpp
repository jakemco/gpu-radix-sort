#include "DXConstantBuffer.h"

DXConstantBuffer::DXConstantBuffer(ID3D11Device* device, unsigned int size)
{
	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));

	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.ByteWidth = ((size + 15) / 16) * 16; // must be multiple of 16 bytes

	HRESULT hr = device->CreateBuffer(&Desc, NULL, &(this->buffer));

	if (hr != S_OK) throw 1;
}

DXConstantBuffer::~DXConstantBuffer()
{
	//this->buffer->Release();
}

void* DXConstantBuffer::map(ID3D11DeviceContext* context) {
	context->Map((ID3D11Resource*) this->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &this->msr);
	return (msr.pData);
}

void DXConstantBuffer::unmap(ID3D11DeviceContext* context) {
	context->Unmap((ID3D11Resource *)this->buffer, 0);
}