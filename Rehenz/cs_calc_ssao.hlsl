Texture2D ss_normal_map : register(t0);
Texture2D ss_depth_map : register(t1);
RWTexture2D<float4> ssao_map : register(u0);

#define N 256

[numthreads(N, 1, 1)]
void main(uint3 dt_id : SV_DispatchThreadID, uint3 gt_id : SV_GroupThreadID)
{
    ssao_map[dt_id.xy] = float4(0.9f, 0, 0, 0);
}
