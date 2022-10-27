#include "blinn_phong.hlsli"

float ComputeShadow(Texture2D shadow_map, SamplerComparisonState shadow_samp, float2 uv, float depth)
{
    float3 dim;
    shadow_map.GetDimensions(0, dim.x, dim.y, dim.z);
    float2 delta = 1.0f / dim.xy;

#ifdef BIGPCF
    float2 ddx_uv = ddx(uv);
    float2 ddy_uv = ddy(uv);
    float2 ddxy_depth = { ddx(depth),ddy(depth) };
    float2x2 dduv_xy = { ddy_uv.y,-ddy_uv.x,-ddx_uv.y,ddx_uv.x };
    float2 dduv_depth = mul(ddxy_depth, dduv_xy) / determinant(dduv_xy);
#endif

    float shadow = 0;
    [unroll]
    for (int y = -1; y <= 1; y++)
    {
        [unroll]
        for (int x = -1; x <= 1; x++)
        {
            float2 offset = float2(x, y) * delta;
            float d = depth;
#ifdef BIGPCF
            d += dot(offset, dduv_depth);
#endif
            shadow += shadow_map.SampleCmpLevelZero(shadow_samp, uv + offset, d).r;
        }
    }
    shadow /= 9;

    return shadow;
}

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
SamplerComparisonState shadow_samp : register(s0, space1);

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
    float shadow = ComputeShadow(shadow_map, shadow_samp, posT.xy, posT.z);

    float3 color = 0;
    if (light_enable > 0.5f)
    {
        color += mat.diffuse_albedo * light_ambient;
        color += (1 - shadow) * ComputeLight(light, mat, normal, to_eye, input.posW);
    }
    float4 output = float4(saturate(color), 1);

    return output;
}
