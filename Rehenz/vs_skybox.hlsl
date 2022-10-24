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

struct VSInput
{
    float3 posL : POSITION;
    float3 normL : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 posH : SV_POSITION;
    float3 posL : POSITION;
};

VSOutput main(VSInput input)
{
    float3 eye_pos = mul(float4(0, 0, 0, 1), frame_info.inv_view).xyz;

    VSOutput output;
    float3 posW = input.posL * 10 + eye_pos;
    vector posV = mul(float4(posW, 1), frame_info.view);
    output.posH = mul(posV, frame_info.proj);
    output.posL = input.posL;
    return output;
}
