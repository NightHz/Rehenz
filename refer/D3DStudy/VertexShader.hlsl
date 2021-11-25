
struct VS_INPUT
{
	float4 pos : POSITION;
	float4 col : COLOR;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix mWorldViewProj;
	float4 aVector;
	float aFloat;
};

VS_OUTPUT main( VS_INPUT input )
{
	VS_OUTPUT output = (VS_OUTPUT)0;

//	output.pos = input.pos;
	output.pos = mul(input.pos, mWorldViewProj);
	output.col = input.col * aFloat;

	return output;
}