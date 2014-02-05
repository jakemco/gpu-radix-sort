#include "DXWrapper.h"

#include <Windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>

DXWrapper::DXWrapper(HWND window, int width, int height)
{
	const DXGI_SWAP_CHAIN_DESC sd = { { width, height, { 60, 1 }, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED, DXGI_MODE_SCALING_UNSPECIFIED }, { 1, 0 }, DXGI_USAGE_RENDER_TARGET_OUTPUT, 1, NULL, TRUE, DXGI_SWAP_EFFECT_SEQUENTIAL, 0 };

	DXGI_SWAP_CHAIN_DESC temp;
	temp = sd;
	temp.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_SHADER_INPUT;
	temp.OutputWindow = window;


	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&temp,
		&(this->swapChain),
		&(this->device),
		NULL,
		&(this->context)
	);
	
}


DXWrapper::~DXWrapper()
{
	this->context->ClearState();
	this->device->Release();
	this->swapChain->Release();
}

void DXWrapper::present() {
	this->swapChain->Present(0, 0);
}

DXTexture DXWrapper::getTexture(int id) {
	return DXTexture(this->swapChain, this->device, id);
}

DXTexture DXWrapper::getTexture(unsigned int w, unsigned int h) {
	return DXTexture(this->device, w, h);
}

DXStructuredBuffer DXWrapper::getStructuredBuffer(unsigned int stride, unsigned int num) {
	return DXStructuredBuffer(this->device, stride, num);
}

DXConstantBuffer DXWrapper::getConstantBuffer(unsigned int size) {
	return DXConstantBuffer(this->device, size);
}

DXShader DXWrapper::getComputeShader(const BYTE* bytecode, unsigned int size) {
	return DXShader(device, bytecode, size);
}

void DXWrapper::setComputeShader(DXShader& shader) {
	this->context->CSSetShader(shader.getShader(), NULL, 0);
}

void DXWrapper::setUAV(int i, int j, ID3D11UnorderedAccessView** u) {
	this->context->CSSetUnorderedAccessViews(i, j, u, NULL);
}

void DXWrapper::setSRV(int i, int j, ID3D11ShaderResourceView** s) {
	this->context->CSSetShaderResources(i, j, s);
}

void DXWrapper::runShader(unsigned int x, unsigned int y, unsigned int z) {
	this->context->Dispatch(x, y, z);
}

void DXWrapper::resetShader() {
	ID3D11ShaderResourceView* pNull[3] = { NULL, NULL, NULL };
	this->context->CSSetShaderResources(0, 3, pNull);

	ID3D11UnorderedAccessView* uNull[3] = { NULL, NULL, NULL };
	this->context->CSSetUnorderedAccessViews(0, 3, uNull, NULL);
}

void DXWrapper::unmap(DXConstantBuffer& buffer) {
	return buffer.unmap(context);
}

void DXWrapper::setConstantBuffer(int i, int j, DXConstantBuffer& b) {
	this->context->CSSetConstantBuffers(i, j, &b.buffer);
}

