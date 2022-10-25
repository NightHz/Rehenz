#include "samplers.hlsli"

struct CBFrame
{
    matrix view;
    matrix inv_view;
    matrix proj;
};

cbuffer CBFrame : register(b0)
{
    CBFrame frame_info;
};

TextureCube cubemap : register(t0);

struct PSInput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 posL : POSITION1;
    float4 color : COLOR;
    float3 normW : NORMAL;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 eye_pos = mul(float4(0, 0, 0, 1), frame_info.inv_view).xyz;
    float eye_dis = length(eye_pos);
    float3 at = input.posL + eye_pos / (eye_dis + 100);

    float4 color;
    color = cubemap.Sample(samp_linear, at);
    return color;
}
