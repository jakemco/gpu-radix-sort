#pragma once

#include <Windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include "DXTexture.h"
#include "DXStructuredBuffer.h"
#include "DXShader.h"
#include "DXConstantBuffer.h"

class DXWrapper
{
private:
	ID3D11Device* device;
	IDXGISwapChain* swapChain;
	ID3D11DeviceContext* context;

public:
	DXWrapper(HWND, int, int);
	~DXWrapper();

	void present();
	DXTexture getTexture(int id);
	DXTexture getTexture(unsigned int, unsigned int);
	DXStructuredBuffer getStructuredBuffer(unsigned int, unsigned int);
	DXShader getComputeShader(const BYTE*, unsigned int);

	DXConstantBuffer getConstantBuffer(unsigned int);
	template<typename S> S* map(DXConstantBuffer&);
	void unmap(DXConstantBuffer&);


	void setComputeShader(DXShader&);
	void setUAV(int, int, ID3D11UnorderedAccessView**);
	void setSRV(int, int, ID3D11ShaderResourceView**);
	void setConstantBuffer(int, int, DXConstantBuffer&);
	void runShader(unsigned int, unsigned int, unsigned int);
	void resetShader();

};

template<typename S>
S* DXWrapper::map(DXConstantBuffer& buffer) {
	return static_cast<S*>(buffer.map(context));
}




