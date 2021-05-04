struct Particle
{
    float3 position;
    float3 direction;
    float time;
};

StructuredBuffer<Particle> SimulationState;

struct VS_INPUT
{
    uint vertexID : SV_VertexID;
};

struct GS_INPUT
{
    float3 position : SV_Position;
};

GS_INPUT VSMAIN(in VS_INPUT input ) 
{
    GS_INPUT output;
    
    output.position.xyz = SimulationState[input.vertexID].position;
	
	return output;
}