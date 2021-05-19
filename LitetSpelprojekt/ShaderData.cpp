#include "ShaderData.h"
#include "ShaderLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ShaderData::ShaderData()
{
	HRESULT hr;
	std::string byteCode;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//SAMPLERS
	Graphics::GetDeviceContext().PSSetSamplers(0, 1, Graphics::GetWrapSampler());

	//BUFFER
	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &viewProjBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}

	//-----RAIN PARTICLE SYSTEM-----
	//SHADERS
	std::string vs_path = "../x64/Debug/RainVertexShader.cso";
	if (!LoadVertexShader(rainVS, vs_path, byteCode))
		return;

	std::string ps_path = "../x64/Debug/RainPixelShader.cso";
	if (!LoadPixelShader(rainPS, ps_path))
		return;

	std::string gs_path = "../x64/Debug/RainGeometryShader.cso";
	if (!LoadGeometryShader(rainGS, gs_path))
		return;

	//INPUT LAYOUT
	const unsigned int particleNumElements = 2;
	D3D11_INPUT_ELEMENT_DESC particle2InputDesc[particleNumElements] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},  
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	hr = Graphics::GetDevice().CreateInputLayout(particle2InputDesc, particleNumElements, byteCode.c_str(), byteCode.length(), &rainLayout);

	//LOAD TEXTURE
	LoadTexture("RainParticle.png");
	//---------------------------------------//

	//-----Smoke PARTICLE SYSTEM-----
	//SHADERS
	vs_path = "../x64/Debug/SmokeVertexShader.cso";
	if (!LoadVertexShader(smokeVS, vs_path, byteCode))
		return;

	ps_path = "../x64/Debug/SmokePixelShader.cso";
	if (!LoadPixelShader(smokePS, ps_path))
		return;

	gs_path = "../x64/Debug/SmokeGeometryShader.cso";
	if (!LoadGeometryShader(smokeGS, gs_path))
		return;

	//INPUT LAYOUT
	D3D11_INPUT_ELEMENT_DESC smokeInputDesc[particleNumElements] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	hr = Graphics::GetDevice().CreateInputLayout(smokeInputDesc, particleNumElements, byteCode.c_str(), byteCode.length(), &smokeLayout);

	//-----REGULAR-----
	//SHADERS
	vs_path = "../x64/Debug/VertexShader.cso";
	if (!LoadVertexShader(regularVertexShader, vs_path, byteCode))
		return;

	ps_path = "../x64/Debug/PixelShader.cso";
	if (!LoadPixelShader(regularPixelShader, ps_path))
		return;

	ps_path = "../x64/Debug/TexturePixelShader.cso";
	if (!LoadPixelShader(regularTexturePixelShader, ps_path))
		return;

	ps_path = "../x64/Debug/ColorPixelShader.cso";
	if (!LoadPixelShader(regularColorPixelShader, ps_path))
		return;

	//INPUT LAYOUT
	const unsigned int regularNumElements = 7;
	D3D11_INPUT_ELEMENT_DESC regularInputDesc[regularNumElements]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = Graphics::GetDevice().CreateInputLayout(regularInputDesc, regularNumElements, byteCode.c_str(), byteCode.length(), &regularLayout);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE INPUT LAYOUT");
		return;
	}

	//BUFFERS
	bufferDesc.ByteWidth = sizeof(MatricesCbuf);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &regularMatricesBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}

	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &regularLightMatrixBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}

	bufferDesc.ByteWidth = sizeof(MaterialCBuf);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &regularMaterialBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}

	bufferDesc.ByteWidth = sizeof(CameraCbuf);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &regularCameraBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}

	bufferDesc.ByteWidth = sizeof(JointCbuf);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &jointBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}
}

void ShaderData::Update(const Camera& camera)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	//-----GENERAL-----
	cameraPosition = camera.GetPosition();
	viewMatrix = camera.GetViewMatrix();
	projectionMatrix = camera.GetPerspectiveMatrix();

	//-----PARTICLE_SYSTEM-----
	//VIEW PROJ BUFFER
	hr = Graphics::GetDeviceContext().Map(viewProjBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return;
	}

	XMFLOAT4X4 viewProjMatrix;
	XMStoreFloat4x4(&viewProjMatrix, XMMatrixTranspose(projectionMatrix) * XMMatrixTranspose(viewMatrix));

	memcpy(mappedResource.pData, &viewProjMatrix, sizeof(XMFLOAT4X4));
	Graphics::GetDeviceContext().Unmap(viewProjBuffer.Get(), 0);
	Graphics::GetDeviceContext().VSSetConstantBuffers(2, 1, viewProjBuffer.GetAddressOf());

	//-----SECOND_PARTICLE_SYSTEM-----
	Graphics::GetDeviceContext().GSSetConstantBuffers(0, 1, viewProjBuffer.GetAddressOf());
	Graphics::GetDeviceContext().GSSetConstantBuffers(1, 1, regularCameraBuffer.GetAddressOf());
	Graphics::GetDeviceContext().PSSetShaderResources(0, 1, rainTexSRV.GetAddressOf());

	//-----REGULAR-----
	//CAMERA BUFFER
	hr = Graphics::GetDeviceContext().Map(regularCameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return;
	}

	memcpy(mappedResource.pData, &cameraPosition, sizeof(XMFLOAT3));
	Graphics::GetDeviceContext().Unmap(regularCameraBuffer.Get(), 0);
	Graphics::GetDeviceContext().PSSetConstantBuffers(0, 1, regularCameraBuffer.GetAddressOf());
}

bool ShaderData::LoadTexture(std::string fileName)
{
	ID3D11Texture2D* texture;

	//Width, Height and channel values
	int textureWidth = 100;
	int textureHeight = 100;
	int channels = 4; //RGBA

	//Loads the image from the Debug folder
	unsigned char* imageData = stbi_load(fileName.c_str(), &textureWidth, &textureHeight, &channels, 4);

	std::vector<unsigned char> textureData;
	textureData.resize(textureWidth * textureHeight * 4);

	//Loops for each pixel and assigns the colour depending on the image loaded into imageData
	for (int h = 0; h < textureHeight; ++h)
	{
		for (int w = 0; w < textureWidth; ++w)
		{
			unsigned int pos0 = w * 4 + textureWidth * 4 * h;
			textureData[pos0 + 0] = imageData[pos0 + 0];
			textureData[pos0 + 1] = imageData[pos0 + 1];
			textureData[pos0 + 2] = imageData[pos0 + 2];
			textureData[pos0 + 3] = imageData[pos0 + 3];
		}
	}

	//Description of the Texture
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = textureWidth; //Width
	desc.Height = textureHeight; //Height
	desc.MipLevels = 1; //Miplevels are used for optimizing images ,speeding up rendering times. Dont need more than 1
	desc.ArraySize = 1; //Amount of textures
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //RGBA
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0; //Lowest quality, no need for more
	desc.Usage = D3D11_USAGE_IMMUTABLE;  //No CPU access
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; //Binds as Shader
	desc.CPUAccessFlags = 0; //No CPU access
	desc.MiscFlags = 0; //No additional flags

	//The subresource data 
	D3D11_SUBRESOURCE_DATA  data; //Pointer to the initialization data
	data.pSysMem = &textureData[0];
	data.SysMemPitch = textureWidth * 4; //Distance (bytes) between beginning of a row in a texture to the next one 
	data.SysMemSlicePitch = 0; // --Only used for 3D texture data-- Distance (bytes) from the beginning of depth level to the next
	
	HRESULT hr = Graphics::GetDevice().CreateTexture2D(&desc, &data, &texture);

	if (FAILED(hr))
	{
		ERROR("FAILED TO CREATE TEXTURE");
		return false;
	}

	hr = Graphics::GetDevice().CreateShaderResourceView(texture, nullptr, rainTexSRV.GetAddressOf());
	if (FAILED (hr))
	{
		Error("FAILED TO CREATE TEXTURE SRV!!!!wtf :O");
		return false;
	}

	return false;
}