#include "Shader.h"
#include "ShaderLoader.h"

Shader::Shader()
{
	this->vertexShader = nullptr;
	this->pixelShader = nullptr;
	this->layout = nullptr;
	this->VS_Buffer = nullptr;
}

void Shader::ShutDown()
{
	if (vertexShader)
	{
		vertexShader->Release();
		this->vertexShader = nullptr;
	}

	if (pixelShader)
	{
		pixelShader->Release();
		this->pixelShader = nullptr;
	}

	if (layout)
	{
		layout->Release();
		this->layout = nullptr;
	}

	if (VS_Buffer)
	{
		VS_Buffer->Release();
		this->VS_Buffer = nullptr;
	}
}

bool Shader::Initialize(ID3D11Device* device, HWND window)
{
	std::string byteCode;
	if (!LoadVertexShader(device, vertexShader, vs_path, byteCode))
		return false;

	if (!LoadPixelShader(device, pixelShader, ps_path))
		return false;

	//INPUT LAYOUT
	const UINT numElements = 3;
	D3D11_INPUT_ELEMENT_DESC inputDesc[numElements] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if (FAILED(device->CreateInputLayout(inputDesc, numElements, byteCode.c_str(), byteCode.length(), &layout)))
		return false;

	//BUFFERS
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VS);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&bufferDesc, nullptr, &VS_Buffer)))
		return false;

	return true;
}

void Shader::Render(ID3D11DeviceContext* context, ID3D11ShaderResourceView* depthSRV, Model model, Light light, XMMATRIX viewMatrix, XMMATRIX perspectiveMatrix)
{
	XMMATRIX WVP = model.GetMatrix() * viewMatrix * perspectiveMatrix;
	XMMATRIX light_WVP = model.GetMatrix() * light.GetViewMatrix() * light.GetPerspectiveMatrix();
}
