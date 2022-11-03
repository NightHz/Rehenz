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

struct CBFrame
{
    matrix view;
    matrix inv_view;
    matrix proj;
    float4 _pad1;
    float4 _pad2;
    matrix _pad3;
    matrix _pad4;
    float4 _pad5;
    matrix _pad6;
    matrix inv_proj;
};

cbuffer CBFrame : register(b0)
{
    CBFrame frame_info;
};

Texture2D ss_normal_map : register(t0);
Texture2D ss_depth_map : register(t1);
RWTexture2D<float4> ssao_map : register(u0);

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

float CalcSSAO(float2 uv)
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
    return ao;
}

#define N 256

[numthreads(N, 1, 1)]
void main(uint3 dt_id : SV_DispatchThreadID, uint3 gt_id : SV_GroupThreadID)
{
    float2 uv = (float2(dt_id.xy) + 0.23f) / ssao_map.Length.xy;
    float occlusion = CalcSSAO(uv);

    // improve occlusion
    occlusion = saturate(1 - pow(1 - occlusion, 10));
    ssao_map[dt_id.xy] = float4(occlusion, 0, 0, 0);
}
