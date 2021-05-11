Texture2D particleTexture : register(t1);
SamplerState wrapSampler : register(s2);

cbuffer eyePos : register(b0)
{
    float3 eyePos;
}

struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    uint primitiveID : SV_PrimitiveID;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    //float opacity = 1.0f - smoothstep(0.0f, 0.0f, age)
    return float4(113.0f / 255.0f, 112.0f / 255.0f, 110.0f / 255.0f, 1.0f);
    input.normal = normalize(input.normal);
    
    float3 toEye = eyePos - input.position;
    float distanceToEye = length(toEye);
    
    //Normalize
    toEye /= distanceToEye;
    
    //Default variables
    float4 textureColor = float4(58.0f / 255, 64.0f / 255, 59 / 255, 1.0f);
    //float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    //float diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    //float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //uvw används vid texture array
    float3 uvw = float3(input.tex, input.primitiveID);
    
    //TextureColor gets particle texture
   //textureColor = particleTexture.Sample(wrapSampler, 1.0f);
    
    //Clip for transparency
    //clip(textureColor.a - 0.05f);
    
    //Combine to get final color
    float4 litColor = textureColor; /* * (ambient + diffuse) + specular*/

    return litColor;
}