#pragma once

#include <Windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include "DXTexture.h"
#include "DXStructuredBuffer.h"
#include "DXShader.h"

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
	DXStructuredBuffer getStructuredBuffer(unsigned int, unsigned int);
	DXShader getComputeShader(const BYTE*);

	void setComputeShader(DXShader&);
	void attachStructuredBuffer(DXStructuredBuffer& buffer);
	void runShader(unsigned int, unsigned int, unsigned int);
	void resetShader();


};

