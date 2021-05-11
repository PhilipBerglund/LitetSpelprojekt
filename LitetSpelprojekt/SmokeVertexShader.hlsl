struct VS_INPUT
{
    float4 position : POSITION;
    float2 size : SIZE;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 size : SIZE;
};

cbuffer MATRIXBUFFER : register(b2)
{
    float4x4 viewProj;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = input.position;
    output.size = input.size;
    
    return output;
}