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

#define MAX_JOINTS 5
cbuffer JointMatrices : register(b2)
{
    float4x4 jointMatrices[MAX_JOINTS];
    int numJoints;
}

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;

    float4x4 boneTX;
    for (int i = 0; i < 1; ++i)
    {
        if (input.boneIDs[i] == -1)
            break;
        
        else if (input.weights[i] == 0)
            break;
        
        else if (i == 0)
            boneTX = jointMatrices[input.boneIDs[i]] * input.weights[i];
        
        else
            boneTX = mul(boneTX, jointMatrices[input.boneIDs[i]] * input.weights[i]);
    }
    
    if (input.boneIDs[0] != -1)
        output.position = mul(float4(input.position, 1.0f), boneTX);
    
    else
        output.position = float4(input.position, 1.0f);
    
    output.worldPosition = mul(output.position, worldMatrix);
    output.position = mul(output.position, WVPMatrix);
	
	output.uv = input.uv;
	
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix));

	return output;
}