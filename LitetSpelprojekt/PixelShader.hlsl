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
    float4 baseColor = float4(0.6f, 0.6, 0.6, 1);
    
    float diffuse = saturate(dot(input.normal, cameraPosition));
    
    float4 finalColor = float4(input.normal, 1.0f);
    
    return finalColor;
}


