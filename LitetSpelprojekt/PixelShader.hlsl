Texture2D testTexture : register(t0);
SamplerState testSampler;

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
    float4 worldPosition : WORLD_POSITION;
};

float4 main(PixelShaderInput input) : SV_TARGET
{

    float4 finalColor = float4(0, 0, 0, 0);
    
    return finalColor;
}


