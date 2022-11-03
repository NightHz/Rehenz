#include "blinn_phong.hlsli"
#include "samplers.hlsli"

float2 pos2uv(float2 pos)
{
    return (pos * float2(1, -1) + 1) / 2;
}

float2 uv2pos(float2 uv)
{
    return (uv * 2 - 1) * float2(1, -1);
}

float2 uv_align(float2 uv, float2 size)
{
    return (clamp(floor(uv * size), 0, size - 1) + 0.5f) / size;
}

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
    float mat_roughness;
    float3 light_direction;
    float _pad2;
    matrix _pad3;
    matrix _pad4;
    float4 _pad5;
    matrix light_view_proj_tex;
    matrix inv_proj;
};

cbuffer CBFrame : register(b0)
{
    CBFrame frame_info;
};

Texture2D shadow_map : register(t0);
SamplerComparisonState shadow_samp : register(s0, space1);
Texture2D ss_normal_map : register(t1);
Texture2D ss_depth_map : register(t2);

float3 GetPosV(float2 uv, float2 ss_size)
{
    uv = uv_align(uv, ss_size);
    float4 posH = float4(uv2pos(uv), ss_depth_map.SampleLevel(samp_point_clamp, uv, 0).r, 1);
    float4 posV = mul(posH, frame_info.inv_proj);
    return posV.xyz / posV.w;
}

float3 GetNormalV(float2 uv)
{
    return ss_normal_map.SampleLevel(samp_point_clamp, uv, 0).xyz * 2 - 1;
}

float ComputeSSAO(float2 uv)
{
    float3 dim;
    ss_normal_map.GetDimensions(0, dim.x, dim.y, dim.z);

    // compute position and normal
    float3 p = GetPosV(uv, dim.xy);
    float3 pW = mul(float4(p, 1), frame_info.inv_view).xyz;
    float3 n = GetNormalV(uv);

    // generate sample kernel
    const float f = sqrt(1 / 3);
    const float3 ks[14] = {
        float3(1, 0, 0),  float3(-1, 0, 0),
        float3(0, 1, 0),  float3(0, -1, 0),
        float3(0, 0, 1),  float3(0, 0, -1),
        float3(f, f, f),  float3(-f, f, f),
        float3(f, -f, f), float3(f, f, -f),
        float3(f, -f, -f),float3(-f, f, -f),
        float3(-f, -f, f),float3(-f, -f, -f)
    };
    const uint ksn = 14;

    // compute ambient occlusion
    float ao = 0;
    const float radius = 0.2f;
    const float bias = 0.15f;
    const float bias_l = 0.001f;
    const float max_l = 1.4f;
    const float scale = 2.5f;
    for (uint i = 0; i < ksn; i++)
    {
        // compute q and r
        float3 qW = pW + ks[i] * radius;
        float3 qV = mul(float4(qW, 1), frame_info.view).xyz;
        float4 qH = mul(float4(qV, 1), frame_info.proj);
        qH.xyz /= qH.w;
        float3 r = GetPosV(pos2uv(qH.xy), dim.xy);

        // compute l and v
        float3 diff = r - p;
        float l = length(diff);
        float3 v = diff / l;

        // compute ao
        float dot_nr = max(dot(n, v) - bias, 0);
        float c1 = l < bias_l ? 0 : smoothstep(max_l, max_l * 0.1f, l);
        float c2 = (1 / (1 + l * scale));
        ao += dot_nr * c1 * c2;
    }
    ao /= ksn;

    // improve occlusion
    ao = saturate(1 - pow(1 - ao, 10));
    return ao;
}

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
    mat.roughness = frame_info.mat_roughness;
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
#ifdef SHADOW
    float shadow = ComputeShadow(shadow_map, shadow_samp, posT.xy, posT.z);
#else
    float shadow = 0;
#endif

    float4 posH = mul(mul(float4(input.posW, 1), frame_info.view), frame_info.proj);
    float2 ss_uv = (posH.xy / posH.w * float2(1, -1) + 1) / 2;
#ifdef SSAO
    float ao = ComputeSSAO(ss_uv);
#else
    float ao = 0;
#endif

    float3 color = 0;
    if (light_enable > 0.5f)
    {
        color += (1 - ao) * mat.diffuse_albedo * light_ambient;
        color += (1 - shadow) * ComputeLight(light, mat, normal, to_eye, input.posW);
    }
    float4 output = float4(saturate(color), 1);

    return output;
}
