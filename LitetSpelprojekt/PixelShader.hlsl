Texture2D testTexture : register(t0);
SamplerState testSampler;

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
    float4 worldPosition : WORLD_POSITION;
};

cbuffer Camera
{
    float3 cameraPosition;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 baseColor = float4(1.0f, 0.6, 0.6, 1);
    
    //FOG ? 
    //float4 fogColor = { 0.4, 0.4, 0.4, 1.0 };
    //float fogDensity = 0.5;
    //float distance = length(input.worldPosition - cameraPosition);
    //float fogFactor = pow(1.0 / 2.71828, distance);
    //float4 finalColor = fogFactor * baseColor + (1.0 - fogFactor) * fogColor;
    
    //SIMPLE DIFFUSE (UNDEFINED POINT LIGHT AT CAMERA POSITION)
    
    float diffuse = saturate(dot(input.normal, float3(40, 40, 40)));
    
    float4 finalColor = float4(input.normal, 1.0f);
    
    return finalColor;
}


