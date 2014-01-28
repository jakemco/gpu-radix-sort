#include "DXShader.h"


DXShader::DXShader(ID3D11Device* device, const BYTE* bytecode)
{
	device->CreateComputeShader(bytecode, sizeof(bytecode), NULL, &this->shader);
}


DXShader::~DXShader()
{
}

ID3D11ComputeShader* DXShader::getShader() {
	return this->shader;
}
