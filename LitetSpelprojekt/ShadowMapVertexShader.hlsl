cbuffer light : register(b0)
{
    matrix lightViewProj;
};



float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos; 
}