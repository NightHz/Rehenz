#include "samplers.hlsli"

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

#define N 256

[numthreads(N, 1, 1)]
void main(uint3 dt_id : SV_DispatchThreadID, uint3 gt_id : SV_GroupThreadID)
{
    // compute pos and normal in view space
    float2 uv = float2(dt_id.xy) / ssao_map.Length.xy;
    float4 posH = float4((uv * 2 - 1) * float2(1, -1), ss_depth_map.SampleLevel(samp_linear_clamp, uv, 0).r, 1);
    float4 posV = mul(posH, frame_info.inv_proj);
    float3 pos = posV.xyz / posV.w;
    float3 normal = ss_normal_map.SampleLevel(samp_linear_clamp, uv, 0).xyz * 2 - 1;

    // generate samples
    float3 samples[14];
    uint sample_count = 14;
    float f = sqrt(1 / 3);
    samples[0] = float3(1, 0, 0); samples[1] = float3(-1, 0, 0);
    samples[2] = float3(0, 1, 0); samples[3] = float3(0, -1, 0);
    samples[4] = float3(0, 0, 1); samples[5] = float3(0, 0, -1);
    samples[6] = float3(f, f, f); samples[7] = float3(-f, f, f);
    samples[8] = float3(f, -f, f); samples[9] = float3(f, f, -f);
    samples[10] = float3(f, -f, -f); samples[11] = float3(-f, f, -f);
    samples[12] = float3(-f, -f, f); samples[13] = float3(-f, -f, -f);

    // compute occlusion
    float occlusion = 0;
    float occlusion_radiu = 0.2f;
    for (uint i = 0; i < sample_count; i++)
    {
        float3 q = pos + samples[i] * occlusion_radiu;
        float4 qH = mul(float4(q, 1), frame_info.proj);
        qH.xy /= qH.w;
        float2 uv_r = (qH.xy * float2(1, -1) + 1) / 2;
        float4 rH = float4(qH.xy, ss_depth_map.SampleLevel(samp_linear_clamp, uv_r, 0).r, 1);
        float4 rV = mul(rH, frame_info.inv_proj);
        float3 r = rV.xyz / rV.w;

        float dz = pos.z - r.z; // z distance
        float dot_nr = max(dot(normal, normalize(r - pos)), 0); // cos<n,r-p>
        if (dz > 0.03f)
            occlusion += dot_nr * saturate((1.5f - dz) / 1.5f);
    }
    occlusion /= sample_count;

    // improve occlusion
    occlusion = saturate(1 - pow(1 - occlusion, 4));
    ssao_map[dt_id.xy] = float4(occlusion, 0, 0, 0);
}
