#pragma once

#include <d3d11.h>

class DXConstantBuffer
{
friend class DXWrapper;
private:
	ID3D11Buffer* buffer;
	D3D11_MAPPED_SUBRESOURCE msr;

public:
	DXConstantBuffer(ID3D11Device*, unsigned int);
	~DXConstantBuffer();

	void* map(ID3D11DeviceContext*);
	void unmap(ID3D11DeviceContext*);
};


