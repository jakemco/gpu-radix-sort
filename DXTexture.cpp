#include "DXTexture.h"

DXTexture::DXTexture(ID3D11Device* dev, unsigned int w, unsigned int h){
	
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = w;
	desc.Height = h;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;//D3D11.BindFlags.ShaderResource | D3D11.BindFlags.UnorderedAccess

	HRESULT hr = dev->CreateTexture2D(&desc, NULL, &this->texture);

	if (hr != S_OK) throw 1;
	if (this->texture == NULL) throw 2;
	dev->CreateUnorderedAccessView((ID3D11Resource*) this->texture, NULL, &this->uav);
	dev->CreateShaderResourceView((ID3D11Resource*) this->texture, NULL, &this->srv);


}

DXTexture::DXTexture(IDXGISwapChain* sc, ID3D11Device* dev, int id)
{
	sc->GetBuffer(id, __uuidof(ID3D11Texture2D), (void**) &this->texture);
	dev->CreateUnorderedAccessView((ID3D11Resource*) this->texture, NULL, &this->uav);

}


DXTexture::~DXTexture()
{
	this->texture->Release();
}
