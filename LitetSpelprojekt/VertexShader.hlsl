struct VertexShaderInput
{
	float3 position : POSITION;
    float3 normal : NORMAL;
	float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float4 weights : WEIGHTS;
    int4 boneIDs : BONEIDS;
};

struct VertexShaderOutput
{
	float4 position: SV_POSITION;
    float3 normal : NORMAL;
	float2 uv : UV;
    float4 worldPosition : WORLD_POSITION;
};

cbuffer Matrices : register(b0)
{
    float4x4 worldMatrix;
    float4x4 WVPMatrix;
};

cbuffer LightMatrix : register(b1)
{
    float4x4 lightWVPMatrix;
}

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	
    output.worldPosition = mul(float4(input.position, 1.0f), worldMatrix);
    output.position = mul(float4(input.position, 1.0f), WVPMatrix);
	
	output.uv = input.uv;
	
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix));

	return output;
}