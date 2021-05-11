cbuffer buffer : register(b0)
{
    matrix viewProj;
};

cbuffer eyePos : register(b1)
{
    float3 eyePos;
}

cbuffer texCoords
{
    float2 texCo[4] =
    {
        float2(0.0f, 1.0f),
        float2(0.0f, 0.0f),
        float2(1.0f, 1.0f),
        float2(1.0f, 0.0f)
    };
};

struct GS_INPUT
{
    float4 position : SV_POSITION;
    float2 size : SIZE;
};

struct GS_OUTPUT
{
    float4 posH : SV_POSITION;
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    uint primitiveID : SV_PrimitiveID;
};

[maxvertexcount(4)]
void main(point GS_INPUT input[1], uint primID : SV_PrimitiveID,
                inout TriangleStream<GS_OUTPUT> outputStream)
{
    //Local coordinates
    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 look = eyePos - input[0].position.xyz;
    look.y = 0.0f;
    look = normalize(look);
    float3 right = cross(up, look) * -1;
    
    //Variables to use for corners
    float halfWidth = 0.5f * input[0].size.x;
    float halfHeight = 0.5f * input[0].size.y;
    
    //Corners of quad
    float4 v[4];
    v[0] = float4(input[0].position.xyz - halfWidth * right + halfHeight * up, 1.0f);
    v[1] = float4(input[0].position.xyz + halfWidth * right + halfHeight * up, 1.0f);
    v[2] = float4(input[0].position.xyz - halfWidth * right - halfHeight * up, 1.0f);
    v[3] = float4(input[0].position.xyz + halfWidth * right - halfHeight * up, 1.0f);
    
    //Transform to world space and output as triangle strip
    GS_OUTPUT gout;
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        gout.posH = mul(v[i], viewProj);
        gout.position = v[i].xyz;
        gout.normal = look;
        gout.tex = texCo[i];
        gout.primitiveID = primID;
        outputStream.Append(gout);
    }
}