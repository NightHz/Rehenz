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
    float3 normV = mul(float4(normalize(input.normW), 0), frame_info.view).xyz;
    return float4((normV + 1) / 2, 1);
}
