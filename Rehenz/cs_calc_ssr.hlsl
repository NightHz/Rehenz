Texture2D ss_normal_map : register(t0);
Texture2D ss_depth_map : register(t1);
Texture2D prev_screen : register(t2);
RWTexture2D<float4> ssr_map : register(u0);

#define N 256

[numthreads(N, 1, 1)]
void main(uint3 dt_id : SV_DispatchThreadID, uint3 gt_id : SV_GroupThreadID)
{
    ssr_map[dt_id.xy] = float4(0.1f, 0.1f, 0.1f, 1);
}
