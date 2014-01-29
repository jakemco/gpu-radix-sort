#include "DXStructuredBuffer.h"


DXStructuredBuffer::DXStructuredBuffer(ID3D11Device* d, unsigned int s, unsigned int n)
{
	D3D11_BUFFER_DESC sbDesc;
	D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;

	sbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sbDesc.StructureByteStride = s;
	sbDesc.ByteWidth = sbDesc.StructureByteStride * n;
	sbDesc.Usage = D3D11_USAGE_DEFAULT;
	d->CreateBuffer(&sbDesc, NULL, &this->buffer);

	// UAV
	sbUAVDesc.Buffer.NumElements = sbDesc.ByteWidth / sbDesc.StructureByteStride;
	sbUAVDesc.Format = DXGI_FORMAT_UNKNOWN; //DXGI_FORMAT_R8G8B8A8_UNORM
	sbUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

	d->CreateUnorderedAccessView((ID3D11Resource *) this->buffer, &sbUAVDesc, &this->uav);
}


DXStructuredBuffer::~DXStructuredBuffer()
{
}

ID3D11UnorderedAccessView * const * DXStructuredBuffer::getUAV() {
	return &this->uav;
}
