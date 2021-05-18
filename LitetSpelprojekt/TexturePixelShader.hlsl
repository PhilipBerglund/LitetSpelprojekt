Texture2D diffuseTexture : register(t0);
Texture2D shadowDSV : register(t1);
SamplerState wrapSampler : register(s0);

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 worldPosition : WORLD_POSITION;
};

cbuffer Camera : register(b0)
{
    float3 cameraPosition;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float diffuse = saturate(dot(input.normal, float3(40, 40, 40)));
    
    float4 finalColor = diffuseTexture.Sample(wrapSampler, input.uv);
    
    return finalColor;
}
