#pragma once

#include <d3d11.h>

class DXShader
{
private:
	ID3D11ComputeShader* shader;
public:
	DXShader(ID3D11Device*, const BYTE*, unsigned int size);
	~DXShader();

	ID3D11ComputeShader* getShader();
};

