#pragma once

#include "Graphics.h"
#include "Camera.h"
#include "Random.h"

#include <directXmath.h>
using namespace DirectX;

class ShaderData
{
	friend class ParticleShader;
	friend class ParticleShader2;
	friend class ParticleSystem2; //Går troligtvis att ta bort, createRandomTexture flyttad hit
	friend class RegularShader;
private:
	//-----GENERAL-----
	//CAMERA
	XMFLOAT3 cameraPosition = {};
	XMMATRIX viewMatrix = {};
	XMMATRIX projectionMatrix = {};

	//-----PARTICLE SYSTEM-----
	//BUFFERS
	ComPtr<ID3D11Buffer> viewProjBuffer;

	//SHADERS
	ComPtr<ID3D11VertexShader> particleVertexShader;
	ComPtr<ID3D11PixelShader> particlePixelShader;

	//MISC
	ComPtr<ID3D11InputLayout> particleLayout;

	//-----SECOND PARTICLE SYSTEM-----
	//BUFFERS
	ComPtr<ID3D11Buffer> streamOutVB;
	ComPtr<ID3D11Buffer> initVB;
	ComPtr<ID3D11Buffer> drawVB;
	struct Particle2Vertex
	{
		XMFLOAT3 initPos;
		XMFLOAT3 initVel;
		XMFLOAT2 size;
		float particleAge;
		unsigned int type;
	};

	//SHADERS
	ComPtr<ID3D11VertexShader> particle2VS;
	ComPtr<ID3D11GeometryShader> particle2GS;
	ComPtr<ID3D11PixelShader> particle2PS;

	//MISC
	ComPtr<ID3D11InputLayout> particle2Layout;
	//For ability to randomize in shader
	ComPtr<ID3D11ShaderResourceView> particle2RandomTexSRV;
	ComPtr<ID3D11Texture1D> particle2RandomTexture;
	//If textures wanted on particle
	ComPtr<ID3D11ShaderResourceView> particle2TexArrSRV;

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
public:
	ShaderData();
	void Update(const Camera& camera);
	bool CreateRandomTexture();
};