
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

float4 main( PS_INPUT input ) : SV_TARGET
{
	float4 output = input.col;

	return output;
}