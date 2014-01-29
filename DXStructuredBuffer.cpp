#include "DXStructuredBuffer.h"


DXStructuredBuffer::DXStructuredBuffer(ID3D11Device* d, unsigned int s, unsigned int n)
{
	D3D11_BUFFER_DESC sbDesc;
	D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC sbSRVDesc;

	ZeroMemory(&sbDesc, sizeof(sbDesc));
	ZeroMemory(&sbUAVDesc, sizeof(sbUAVDesc));
	ZeroMemory(&sbSRVDesc, sizeof(sbSRVDesc));

	sbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sbDesc.StructureByteStride = s;
	sbDesc.ByteWidth = sbDesc.StructureByteStride * n;
	sbDesc.Usage = D3D11_USAGE_DEFAULT;
	HRESULT hr = d->CreateBuffer(&sbDesc, NULL, &this->buffer);

	if (hr != S_OK) throw 1;

	// UAV
	sbUAVDesc.Buffer.NumElements = sbDesc.ByteWidth / sbDesc.StructureByteStride;
	sbUAVDesc.Format = DXGI_FORMAT_UNKNOWN; //DXGI_FORMAT_R8G8B8A8_UNORM
	sbUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

	hr = d->CreateUnorderedAccessView((ID3D11Resource *) this->buffer, &sbUAVDesc, &this->uav);

	if (hr != S_OK) throw 1;

	// SRV
	sbSRVDesc.Buffer.ElementWidth = sbDesc.StructureByteStride;
	sbSRVDesc.Buffer.FirstElement = sbUAVDesc.Buffer.FirstElement;
	sbSRVDesc.Buffer.NumElements = sbUAVDesc.Buffer.NumElements;
	sbSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	sbSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

	hr = d->CreateShaderResourceView((ID3D11Resource*) this->buffer, &sbSRVDesc, &this->srv);

	if (hr != S_OK) throw 1;

}


DXStructuredBuffer::~DXStructuredBuffer()
{
	//this->buffer->Release();
	//this->uav->Release();
	//this->srv->Release();
}

ID3D11UnorderedAccessView ** DXStructuredBuffer::getUAV() {
	return &this->uav;
}

ID3D11ShaderResourceView ** DXStructuredBuffer::getSRV() {
	return &this->srv;
}
