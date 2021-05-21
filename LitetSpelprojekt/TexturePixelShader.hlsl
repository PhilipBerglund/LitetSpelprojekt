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

cbuffer light : register(b3)
{
    matrix lightViewProj;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float diffuse = saturate(dot(input.normal, float3(40, 40, 40)));
    float4 ambient = (0.5f, 0.5f, 0.5f, 1);
    float4 lightDiffuse = (0.5f, 0.5f, 0.5f, 1);
    
    float4 finalColor = diffuseTexture.Sample(wrapSampler, input.uv);
    finalColor = finalColor * ambient * lightDiffuse;
    
    //Flytta ljuset till lightWVP
    float4 positionInLightSpace = mul(input.worldPosition, lightViewProj);
    
    positionInLightSpace.xy /= positionInLightSpace.w; //"Perspective divide" för att projicera texturkoordinaterna på shadow map

    //Skalar till uv-koordinater (0-1)
    float2 smTexUV = float2(0.5f * positionInLightSpace.x + 0.5f, -0.5f * positionInLightSpace.y + 0.5f);
    
    float4 depthMap = shadowDSV.Sample(wrapSampler, smTexUV);
    
    //Beräkna djup på pixel
    float depth = positionInLightSpace.z / positionInLightSpace.w;
    
    float shadowMapSize = 4096;
    float dx = 1.0f / shadowMapSize;
    
    //2x2 sampling, samplar de närmsta pixlarna ("point sampling")
    float s0 = shadowDSV.Sample(wrapSampler, smTexUV).r;
    float s1 = shadowDSV.Sample(wrapSampler, smTexUV + float2(dx, 0)).r;
    float s2 = shadowDSV.Sample(wrapSampler, smTexUV + float2(0, dx)).r;
    float s3 = shadowDSV.Sample(wrapSampler, smTexUV + float2(dx, dx)).r;
    
    //Jämför djup + bias(för att minska artifakter)
    float bias = 0.00005;
    float result1 = depth <= s0 + bias;
    float result2 = depth <= s1 + bias;
    float result3 = depth <= s2 + bias;
    float result4 = depth <= s3 + bias;

    //Transformera shadow map uv positioner till texel space
    float2 texelPos = shadowMapSize * smTexUV;
    
    //Lerp resultat
    float2 t = frac(texelPos);
    float result = lerp(lerp(s0, s1, t.x), lerp(s2, s3, t.x), t.y);
    
    finalColor = finalColor * result;
    
    return finalColor;
}
