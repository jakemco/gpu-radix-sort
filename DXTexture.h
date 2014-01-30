#pragma once

#include <d3d11.h>

class DXTexture
{
private:
	ID3D11Texture2D* texture;
	ID3D11UnorderedAccessView* uav;
	ID3D11ShaderResourceView* srv;
public:
	DXTexture(ID3D11Device* dev, unsigned int w, unsigned int h);
	DXTexture(IDXGISwapChain*, ID3D11Device*, int);
	~DXTexture();

	ID3D11UnorderedAccessView** getUAV() {
		return &this->uav;
	}

	ID3D11ShaderResourceView** getSRV() {
		return &this->srv;
	}
};

