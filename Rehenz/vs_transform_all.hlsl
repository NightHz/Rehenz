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

struct CBObj
{
    matrix world;
    matrix inv_world;
};

StructuredBuffer<CBObj> obj_infos : register(t0);

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
    float3 normW : NORMAL;
    float2 uv : TEXCOORD;
};

VSOutput main(VSInput input, uint id : SV_InstanceID)
{
    VSOutput output;
    output.posW = mul(float4(input.posL, 1), obj_infos[id].world).xyz;
    vector posV = mul(float4(output.posW, 1), frame_info.view);
    output.posH = mul(posV, frame_info.proj);
    output.color = input.color;
    matrix inv_world = obj_infos[id].inv_world;
    float3x3 normal_world = transpose(float3x3(inv_world[0].xyz, inv_world[1].xyz, inv_world[2].xyz));
    output.normW = normalize(mul(input.normL, normal_world));
    output.uv = input.uv;
    return output;
}
