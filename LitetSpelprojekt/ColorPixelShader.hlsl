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

cbuffer Material : register(b1)
{
    float3 diffuse;
    float opacity;
    float3 ambient;
    float shininess;
    float3 emissive;
    float reflectivity;
    float3 specular;
}

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 baseColor = float4(0.6f, 0.6, 0.6, 1);
    
    float diffuseShadow = saturate(dot(input.normal, cameraPosition));
    
    float4 finalColor = float4(diffuse, 1.0f);

    //FOG

    float4 fogColor = float4(0.25f, 0.25f, 0.25f, 1.0f);
    float fogStart = 50.0f;
    float fogRange = 350.0f;

    float fogDistance = distance(cameraPosition, input.worldPosition);

    float fogFactor = saturate((fogDistance - fogStart) / fogRange);
    finalColor = lerp(finalColor, fogColor, fogFactor);

    return finalColor;
}