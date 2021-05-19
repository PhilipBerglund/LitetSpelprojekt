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
    matrix lightWorldViewProj;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float diffuse = saturate(dot(input.normal, float3(40, 40, 40)));
    float4 ambient = (0.5f, 0.5f, 0.5f, 1);
    float4 lightDiffuse = (0.5f, 0.5f, 0.5f, 1);
    
    float4 finalColor = diffuseTexture.Sample(wrapSampler, input.uv);
    finalColor = finalColor * ambient * lightDiffuse;
    
    //Flytta ljuset till lightWVP
    float4 positionInLightSpace = mul(input.worldPosition, lightWorldViewProj);
    
    positionInLightSpace.xy /= positionInLightSpace.w; //"Perspective divide" för att projicera texturkoordinaterna på shadow map

    //Skalar till uv-koordinater (0-1)
    float2 smTexUV = float2(0.5f * (positionInLightSpace.x + 1.0f), -0.5f * (positionInLightSpace.y + 1.0f));
    
    float4 depthMap = shadowDSV.Sample(wrapSampler, smTexUV);
    
    //Beräkna djup på pixel
    float depth = positionInLightSpace.z / positionInLightSpace.w + 1 * 0.5f;
    
    float shadowMapSize = 2048;
    float dx = 1.0f / shadowMapSize;
    float bias = 0.005;
    
    //2x2 sampling, samplar de närmsta pixlarna ("point sampling")
    float s0 = shadowDSV.Sample(wrapSampler, smTexUV).r;
    float s1 = shadowDSV.Sample(wrapSampler, smTexUV + float2(dx, 0)).r;
    float s2 = shadowDSV.Sample(wrapSampler, smTexUV + float2(0, dx)).r;
    float s3 = shadowDSV.Sample(wrapSampler, smTexUV + float2(dx, dx)).r;
    
    //Jämför djup + bias(för att minska artifakter)
    float result1 = depth <= s0;
    float result2 = depth <= s1;
    float result3 = depth <= s2;
    float result4 = depth <= s3s;

    //Transformera shadow map uv positioner till texel space
    float2 texelPos = shadowMapSize * smTexUV.xy;
    
    //Lerp resultat
    float2 t = frac(texelPos);
    float result = lerp(lerp(result1, result2, t.x), lerp(result3, result4, t.x), t.y);
    
    finalColor = finalColor * result;
    
    return finalColor;
}
