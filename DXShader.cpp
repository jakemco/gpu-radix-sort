#include "DXShader.h"

#include <d3dcompiler.h>

HRESULT CompileComputeShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint,
	_In_ ID3D11Device* device, _Outptr_ ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !device || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	// We generally prefer to use the higher CS shader profile when possible as CS 5.0 is better performance on 11-class hardware
	LPCSTR profile = (device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*) errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}

DXShader::DXShader(ID3D11Device* device, const BYTE* bytecode, unsigned int size)
{
	HRESULT hr = device->CreateComputeShader(bytecode, size, NULL, &(this->shader));

	if (hr != S_OK) throw 1;
}



DXShader::~DXShader()
{
}

ID3D11ComputeShader* DXShader::getShader() {
	return this->shader;
}
