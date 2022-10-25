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
};

cbuffer CBFrame : register(b0)
{
    CBFrame frame_info;
};

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
    float3 light_ambient = float3(0.1f, 0.1f, 0.1f);
    BPLight light;
    light.type = 1;
    light.intensity = frame_info.light_intensity;
    light.direction = frame_info.light_direction;

    float3 normal = normalize(input.normW);
    float3 eye_pos = mul(float4(0, 0, 0, 1), frame_info.inv_view).xyz;
    float3 to_eye = eye_pos - input.posW;
    float dist_eye = length(to_eye);
    to_eye /= dist_eye;

    float3 color = 0;
    if (light_enable > 0.5f)
    {
        color += mat.diffuse_albedo * light_ambient;
        color += ComputeLight(light, mat, normal, to_eye, input.posW);
    }
    float4 output = float4(saturate(color), 1);

#ifdef FOG
    float fog_start = 5;
    float fog_end = 10;
    float3 fog_color(0.746f, 0.746f, 0.746f);
    float s = saturate((dist_eye - fog_start) / (fog_end - fog_start));
    output = lerp(output, float4(fog_color, 1), s);
#endif

    return output;
}
