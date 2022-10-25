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

TextureCube envmap : register(t0);

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
    float3 L = eye_pos - input.posW;
    float3 R = refract(-L, input.normW, 0.95f);
    float4 output = float4(saturate(input.color.xyz * 0.9f * envmap.Sample(samp_linear, R).xyz), 1);

    return output;
}
