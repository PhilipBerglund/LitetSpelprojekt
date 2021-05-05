//#pragma once
//#include <DirectXMath.h>
//#include <d3d11.h>
//
//#include "Print.h"
//#include "ComPtr.h"
//
//using namespace DirectX;
//
//class Scene;
//class Model;
//
//class Shader
//{
//private:
//	std::string vs_path = "../x64/Debug/VertexShader.cso";
//	std::string ps_path = "../x64/Debug/PixelShader.cso";
//	std::string texture_ps_path = "../x64/Debug/TexturePixelShader.cso";
//	std::string color_ps_path = "../x64/Debug/ColorPixelShader.cso";
//
//	ComPtr<ID3D11VertexShader> vertexShader;
//	ComPtr<ID3D11PixelShader> pixelShader;
//	ComPtr<ID3D11PixelShader> texturePixelShader;
//	ComPtr<ID3D11PixelShader> colorPixelShader;
//	ComPtr<ID3D11InputLayout> layout;
//
//	ComPtr<ID3D11Buffer> VS_Buffer;
//	struct VS
//	{
//		XMFLOAT4X4 worldMatrix;
//		XMFLOAT4X4 viewMatrix;
//		XMFLOAT4X4 perspectiveMatrix;
//		XMFLOAT4X4 WVP;
//
//		XMFLOAT4X4 lightViewMatrix;
//		XMFLOAT4X4 lightPerspectiveMatrix;
//	} vertexCbuf;
//
//	ComPtr<ID3D11Buffer> cameraBuffer;
//	struct PS
//	{
//		XMFLOAT3 cameraPosition;
//		float padding;
//	} cameraCbuf;
//
//	ComPtr<ID3D11Buffer> materialBuffer;
//	struct MATERIAL
//	{
//		float diffuse[3] = { 0 };
//		float opacity = 0;
//		float ambient[3] = { 0 };
//		float shininess = 0;
//		float emissive[3] = { 0 };
//		float reflectivity = 0;
//		float specular[3] = { 0 };
//		float padding = 0;
//	};
//
//	bool UpdateBuffers(const Model& model);
//public:
//	Shader() 
//	{
//		vertexCbuf = {};
//		cameraCbuf = {};
//	};
//	bool Initialize(HWND window);
//	void SetShader(const Scene& scene);
//	void Render(const Scene& scene);
//};