cbuffer buffer
{
    matrix viewProj;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct GS_OUTPUT
{
	float4 position : SV_POSITION;
    float4 color : COLOR;
};

//Excluded from build, change in properties -> general
[maxvertexcount(2)]
GS_OUTPUT main(point VS_OUTPUT input[1] : SV_POSITION, 
                inout LineStream<GS_OUTPUT> lineStream)
{
    float3 p0 = input[0].position;
    float3 p1 = input[0].position + 0.07f;
    
    GS_OUTPUT v0;
    v0.position = mul(float4(p0, 1.0f), viewProj);
    v0.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    
    GS_OUTPUT v1;
    v1.position = mul(float4(p1, 1.0f), viewProj);
    v1.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
    
    lineStream.Append(v1);
}