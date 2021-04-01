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

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
    output.position = float4(input.position, 1);
	output.uv = input.uv;
	
    output.normal = input.normal;

	return output;
}