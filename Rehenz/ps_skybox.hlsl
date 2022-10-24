#include "samplers.hlsli"

TextureCube cubemap : register(t0);

struct PSInput
{
    float4 posH : SV_POSITION;
    float3 posL : POSITION;
};

float4 main(PSInput input) : SV_TARGET
{
    float4 color;
    color = cubemap.Sample(samp_linear, input.posL);
    return color;
}
