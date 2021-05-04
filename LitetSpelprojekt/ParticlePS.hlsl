Texture2D ParticleTexture : register(t0);
SamplerState LinearSampler : register(s0);

float4 PSMAIN(in PS_INPUT input) : SV_TARGET
{
	float4 color = ParticleTexture.Sample(LinearSampler, input.texcoords);
    color = color * input.color;
    return (color);
}