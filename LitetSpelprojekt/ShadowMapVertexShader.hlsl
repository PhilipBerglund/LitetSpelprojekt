struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 weights : WEIGHTS;
    float4 boneIDs : BONEIDS;
};

cbuffer light : register(b3)
{
    matrix lightWorldViewProj;
};

float4 main(VertexShaderInput input): SV_POSITION
{
    return mul(float4(input.position, 1.0f), lightWorldViewProj);
}