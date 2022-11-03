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

struct GSInput
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float3 posL : POSITION1;
	float4 color : COLOR;
	float3 normW : NORMAL;
	float2 uv : TEXCOORD;
};

struct GSOutput
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float3 posL : POSITION1;
	float4 color : COLOR;
	float3 normW : NORMAL;
	float2 uv : TEXCOORD;
	uint rt_id : SV_RenderTargetArrayIndex;
};

[maxvertexcount(18)]
void main(triangle GSInput input[3], inout TriangleStream<GSOutput> output)
{
	float3 eye_pos = mul(float4(0, 0, 0, 1), frame_info.inv_view).xyz;
	float3x3 z = 0;
	float3x3 rotates[6] = { z,z,z,z,z,z };
	rotates[0] = float3x3(0, 0, 1, 0, 1, 0, -1, 0, 0); // +x
	rotates[1] = float3x3(0, 0, -1, 0, 1, 0, 1, 0, 0); // -x
	rotates[2] = float3x3(1, 0, 0, 0, 0, 1, 0, -1, 0); // +y
	rotates[3] = float3x3(1, 0, 0, 0, 0, -1, 0, 1, 0); // -y
	rotates[4] = float3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);  // +z
	rotates[5] = float3x3(-1, 0, 0, 0, 1, 0, 0, 0, -1);// -z

	for (uint j = 0; j < 6; j++)
	{
		for (uint i = 0; i < 3; i++)
		{
			GSOutput v;
			float4 posV = float4(mul(input[i].posW - eye_pos, rotates[j]), 1);
			v.posH = mul(posV, frame_info.proj);
			v.posW = input[i].posW;
			v.posL = input[i].posL;
			v.color = input[i].color;
			v.normW = input[i].normW;
			v.uv = input[i].uv;
			v.rt_id = j;
			output.Append(v);
		}
		output.RestartStrip();
	}
}
