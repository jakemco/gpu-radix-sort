#pragma once

#include <d3d11.h>

class DXStructuredBuffer
{
private:
	ID3D11Buffer* buffer;
	ID3D11UnorderedAccessView* uav;
	ID3D11ShaderResourceView* srv;


public:
	DXStructuredBuffer(ID3D11Device*, unsigned int, unsigned int);
	~DXStructuredBuffer();

	ID3D11UnorderedAccessView ** getUAV();
	ID3D11ShaderResourceView ** getSRV();
};

