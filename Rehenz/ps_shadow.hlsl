#include "blinn_phong.hlsli"

struct CBFrame
{
    matrix view;
    matrix inv_view;
    matrix proj;
    float3 light_intensity;
    float _pad1;
    float3 light_direction;
    float _pad2;
    matrix _pad3;
    matrix _pad4;
    float4 _pad5;
    matrix light_view_proj_tex;
};

cbuffer CBFrame : register(b0)
{
    CBFrame frame_info;
};

Texture2D shadow_map : register(t1);
SamplerState shadow_samp : register(s0, space1);

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
    BPMaterial mat;
    mat.diffuse_albedo = input.color.xyz;
    mat.fresnel_r0 = float3(0.01f, 0.01f, 0.01f);
    mat.roughness = 0.125f;
    float light_enable = 1;
    float3 light_ambient = float3(0.2f, 0.2f, 0.2f);
    BPLight light;
    light.type = 1;
    light.intensity = frame_info.light_intensity;
    light.direction = frame_info.light_direction;

    float3 normal = normalize(input.normW);
    float3 eye_pos = mul(float4(0, 0, 0, 1), frame_info.inv_view).xyz;
    float3 to_eye = eye_pos - input.posW;
    float dist_eye = length(to_eye);
    to_eye /= dist_eye;

    float4 posT = mul(float4(input.posW, 1), frame_info.light_view_proj_tex);
    posT.xyz /= posT.w;
    float depth = posT.z;
    float light_depth = shadow_map.Sample(shadow_samp, posT.xy).r;

    float3 color = 0;
    if (light_enable > 0.5f)
    {
        color += mat.diffuse_albedo * light_ambient;
        if (depth <= light_depth)
            color += ComputeLight(light, mat, normal, to_eye, input.posW);
    }
    float4 output = float4(saturate(color), 1);

    return output;
}
