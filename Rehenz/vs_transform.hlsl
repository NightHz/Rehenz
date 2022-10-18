cbuffer CBObj : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
    matrix transform;
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
    float3 posW : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.posH = mul(float4(input.posL, 1), transform);
    output.posW = mul(float4(input.posL, 1), world).xyz;
    output.color = input.color;
    output.uv = input.uv;
    return output;
}
