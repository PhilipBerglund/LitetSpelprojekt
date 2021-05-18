#pragma once

#include "Graphics.h"
#include "Camera.h"
#include "Random.h"
#include "ShadowMap.h"
#include "Light.h"

#include <directXmath.h>
using namespace DirectX;

class ShaderData
{
	friend class ParticleShader;
	friend class RegularShader;
	friend class ShadowMapShader;
private:
	//-----GENERAL-----
	//CAMERA
	XMFLOAT3 cameraPosition = {};
	XMMATRIX viewMatrix = {};
	XMMATRIX projectionMatrix = {};

	//DIRECTIONAL_LIGHT
	XMMATRIX lightViewMatrix = {};
	XMMATRIX lightProjectionMatrix = {};

	//-----SMOKE PARTICLE SYSTEM----
	ComPtr<ID3D11VertexShader> smokeVS;
	ComPtr<ID3D11GeometryShader> smokeGS;
	ComPtr<ID3D11PixelShader> smokePS;

	ComPtr<ID3D11InputLayout> smokeLayout;
	//If textures wanted on particle
	ComPtr<ID3D11ShaderResourceView> smokeTexSRV;

	//-----RAIN PARTICLE SYSTEM-----
	//BUFFERS
	ComPtr<ID3D11Buffer> viewProjBuffer;

	//SHADERS
	ComPtr<ID3D11VertexShader> rainVS;
	ComPtr<ID3D11GeometryShader> rainGS;
	ComPtr<ID3D11PixelShader> rainPS;

	//MISC
	ComPtr<ID3D11InputLayout> rainLayout;
	//If textures wanted on particle
	ComPtr<ID3D11ShaderResourceView> rainTexSRV;

	//-----SHADOW MAP-----
	//BUFFERS
	ComPtr<ID3D11Buffer> shadowBuffer;
	XMFLOAT4X4 shadowMapMatrix;
	ComPtr<ID3D11Buffer> lightBuffer;

	//SHADERS
	ComPtr<ID3D11VertexShader> shadowMapVS;

	//INPUT LAYOUT
	ComPtr<ID3D11InputLayout> shadowLayout;

	//ShadowMap
	ShadowMap shadowMap;

	//-----REGULAR-----
	//BUFFERS
	ComPtr<ID3D11Buffer> regularMatricesBuffer;
	struct MatricesCbuf
	{
		XMFLOAT4X4 worldMatrix = {};
		XMFLOAT4X4 WVPMatrix = {};
	};

	XMFLOAT4X4 lightWVPMatrix = {};
	ComPtr<ID3D11Buffer> regularLightMatrixBuffer;

	struct MaterialCBuf
	{
		float diffuse[3] = { 0 };
		float opacity = 0;
		float ambient[3] = { 0 };
		float shininess = 0;
		float emissive[3] = { 0 };
		float reflectivity = 0;
		float specular[3] = { 0 };
		float padding = 0;
	};
	ComPtr<ID3D11Buffer> regularMaterialBuffer;

	struct CameraCbuf
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};
	ComPtr<ID3D11Buffer> regularCameraBuffer;

	//SHADERS
	ComPtr<ID3D11VertexShader> regularVertexShader;
	ComPtr<ID3D11PixelShader> regularPixelShader;
	ComPtr<ID3D11PixelShader> regularTexturePixelShader;
	ComPtr<ID3D11PixelShader> regularColorPixelShader;

	//MISC
	ComPtr<ID3D11InputLayout> regularLayout;

	bool LoadTexture(std::string fileName);
public:
	ShaderData();
	void Update(const Camera& camera, const Light& light);
};