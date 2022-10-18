struct PSInput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
	float4 color;
	color = input.color;
	return color;
}
