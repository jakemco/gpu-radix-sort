#include "DXTexture.h"


DXTexture::DXTexture(IDXGISwapChain* sc, ID3D11Device* dev, int id)
{
	sc->GetBuffer(id, __uuidof(ID3D11Texture2D), (void**) &this->texture);
	dev->CreateUnorderedAccessView((ID3D11Resource*) this->texture, NULL, &this->uav);

}


DXTexture::~DXTexture()
{
	this->texture->Release();
}
