#pragma once

#include <d3d11.h>

class DXTexture
{
private:
	ID3D11Texture2D* texture;
	ID3D11UnorderedAccessView* uav;
public:
	DXTexture(IDXGISwapChain*, ID3D11Device*, int);
	~DXTexture();
};

