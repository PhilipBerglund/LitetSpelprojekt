#include "ShadowMap.h"
#include "Window.h"

ShadowMap::ShadowMap()
	: mWidth(4096), mHeight(4096), mDepthMapSRV(0), mDepthMapDSV(0)
{
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(mWidth);
	mViewport.Height = static_cast<float>(mHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	// Use typeless format because the DSV is going to interpret
	// the bits as DXGI_FORMAT_D24_UNORM_S8_UINT, whereas the SRV is going
	// to interpret the bits as DXGI_FORMAT_R24_UNORM_X8_TYPELESS.
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	ID3D11Texture2D* depthMap = 0;
	Graphics::GetDevice().CreateTexture2D(&texDesc, 0, &depthMap);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	Graphics::GetDevice().CreateDepthStencilView(depthMap, &dsvDesc, mDepthMapDSV.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	Graphics::GetDevice().CreateShaderResourceView(depthMap, &srvDesc, &mDepthMapSRV);
}

ShadowMap::ShadowMap(UINT width, UINT height)
	: mWidth(width), mHeight(height), mDepthMapSRV(0), mDepthMapDSV(0)
{
	// Viewport that matches the shadow map dimensions.
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(width);
	mViewport.Height = static_cast<float>(height);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	// Use typeless format because the DSV is going to interpret
	// the bits as DXGI_FORMAT_D24_UNORM_S8_UINT, whereas the SRV is going
	// to interpret the bits as DXGI_FORMAT_R24_UNORM_X8_TYPELESS.
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	ID3D11Texture2D* depthMap = 0;
	Graphics::GetDevice().CreateTexture2D(&texDesc, 0, &depthMap);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	Graphics::GetDevice().CreateDepthStencilView(depthMap, &dsvDesc, mDepthMapDSV.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	Graphics::GetDevice().CreateShaderResourceView(depthMap, &srvDesc, &mDepthMapSRV);

	// View saves a reference to the texture so we can release our
	// reference.
	//ReleaseCOM(depthMap);
}

ShadowMap::~ShadowMap()
{
}

ID3D11ShaderResourceView** ShadowMap::DepthMapSRV()
{
	return mDepthMapSRV.GetAddressOf();
}

ComPtr<ID3D11DepthStencilView> ShadowMap::DepthMapDSV()
{
	return mDepthMapDSV;
}

void ShadowMap::BindDsvAndSetNullRenderTarget()
{
	Graphics::GetDeviceContext().RSSetViewports(1, &mViewport);
	// Set null render target because we are only going to draw
	// to depth buffer. Setting a null render target will disable
	// color writes.
	ID3D11RenderTargetView* renderTargets[1] = { 0 };
	Graphics::GetDeviceContext().OMSetRenderTargets(1, renderTargets, mDepthMapDSV.Get());
}

void ShadowMap::UpdateLightAndShadow(DirectX::XMFLOAT3& lightDir, DirectX::XMFLOAT4X4& shadowMatrix)
{
	sceneBounds.radius = 600;

	//Normalize and store direction/rotation of light
	XMVECTOR lightDirection = XMVector3Normalize(XMLoadFloat3(&lightDir));
	XMStoreFloat3(&lightDir, lightDirection);

	//For shadow matrix
	XMVECTOR lightPosition = { 0.0f, 1.0f, 0.0f }; //-2 * sceneBounds.radius * lightDirection;
	XMVECTOR targetPosition = XMLoadFloat3(&sceneBounds.center);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(lightPosition, targetPosition, up);

	//Transform scene sphere to light space
	XMFLOAT3 sphereCenterLS;
	XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPosition, V));

	//Orthographic frustum in light space that encloses scene
	float l = sphereCenterLS.x - sceneBounds.radius;
	float b = sphereCenterLS.y - sceneBounds.radius;
	float n = sphereCenterLS.z - sceneBounds.radius;
	float r = sphereCenterLS.x + sceneBounds.radius;
	float t = sphereCenterLS.y + sceneBounds.radius;
	float f = sphereCenterLS.z + sceneBounds.radius;

	//Create left handed orthographics matrix and shadow viewProjection matrix
	XMMATRIX P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
	XMMATRIX S = XMMatrixTranspose(V * P);

	//Store shadow matrix
	XMStoreFloat4x4(&shadowMatrix, S);
}
