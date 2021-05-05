struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer MATRIX
{
   float4x4 viewProj;
};

VS_OUTPUT main(VS_INPUT input) : SV_POSITION
{
    VS_OUTPUT output;
    
    output.position = mul(float4(input.position, 1.0f), viewProj);
    output.color = input.color;
    
	return output;
}