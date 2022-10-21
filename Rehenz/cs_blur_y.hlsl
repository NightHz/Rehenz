struct CB
{
    float w0;
    float w1;
    float w2;
    float w3;
    float w4;
    float w5;
    float w6;
    float w7;
    float w8;
    float w9;
    float w10;
    float _pad1;
};

cbuffer CB : register(b0)
{
    CB blur_info;
};

Texture2D tex_in : register(t0);
RWTexture2D<float4> tex_out : register(u0);

#define N 256
#define R 5
#define M (N+2*R)

groupshared float4 cache[M];

[numthreads(1, N, 1)]
void main(uint3 dt_id : SV_DispatchThreadID, uint3 gt_id : SV_GroupThreadID)
{
    // cache [0,R)
    if (gt_id.y < R)
    {
        uint y = max(dt_id.y - R, 0);
        cache[gt_id.y] = tex_in[uint2(dt_id.x, y)];
    }
    // cache [N+R,N+2*R)
    if (gt_id.y >= N - R)
    {
        uint y = min(dt_id.y + R, tex_in.Length.y - 1);
        cache[gt_id.y + 2 * R] = tex_in[uint2(dt_id.x, y)];
    }
    // cache [R,N+R)
    {
        uint y = min(dt_id.y, tex_in.Length.y - 1);
        cache[gt_id.y + R] = tex_in[uint2(dt_id.x, y)];
    }

    // sync
    GroupMemoryBarrierWithGroupSync();

    // blur
    float ws[11] = (float[11])blur_info;
    float4 color = float4(0, 0, 0, 0);
    for (int i = -R; i <= R; i++)
        color += ws[i + R] * cache[gt_id.y + R + i];
    tex_out[dt_id.xy] = saturate(color);
}
