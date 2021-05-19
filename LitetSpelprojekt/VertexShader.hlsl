struct VertexShaderInput
{
	float3 position : POSITION;
    float3 normal : NORMAL;
	float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float4 weights : WEIGHTS;
    float4 boneIDs : BONEIDS;
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

StructuredBuffer<float4x4> anim;

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	
    float4x4 boneTX[4];
    boneTX[0] = anim[input.boneIDs[0]] * input.weights[0];
    boneTX[1] = anim[input.boneIDs[1]] * input.weights[1];
    boneTX[2] = anim[input.boneIDs[2]] * input.weights[2];
    boneTX[3] = anim[input.boneIDs[3]] * input.weights[3];
    float4x4 combined = boneTX[0] * boneTX[1] * boneTX[2] * boneTX[3];
    
    if (input.boneIDs[0] != -1)
        output.position = mul(float4(input.position, 1.0f), combined);
    
    else
        output.position = float4(input.position, 1.0f);
    
    output.worldPosition = mul(output.position, worldMatrix);
    output.position = mul(float4(input.position, 1.0f), WVPMatrix);
	
	output.uv = input.uv;
	
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix));

	return output;
}