struct VertexShaderInput
{
	float3 position : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
};

struct VertexShaderOutput
{
	float4 position: SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
    float4 worldPosition : WORLD_POSITION;
};

cbuffer Matrices : register(b0)
{
    float4x4 worldMatrix;
    float4x4 WVPMatrix;
    float4x4 perspectiveMatrix;
	
    float4x4 lightViewMatrix;
    float4x4 lightPerspectiveMatrix;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	
    output.worldPosition = mul(float4(input.position, 1.0f), worldMatrix);
    output.position = mul(float4(input.position, 1.0f), WVPMatrix);
	
	output.uv = input.uv;
	
    output.normal = mul(input.normal, (float3x3)worldMatrix);

	return output;
}