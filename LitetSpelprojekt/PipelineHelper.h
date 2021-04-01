#pragma once
#include <DirectXMath.h>
#include <array>
#include <d3d11.h>




bool SetupPipeline(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout,
	ID3D11SamplerState* &sampler);

bool SetupRasterizerState(ID3D11Device* device, ID3D11RasterizerState* &rastState);