#include "shader.h"

namespace Rehenz
{
	VertexShader DefaultVertexShader = [](const VertexShaderData& data, const Vertex& v0)->Vertex
	{
		Vertex v(v0);
		v.p = v.p * data.transform;
		return v;
	};

	PixelShader DefaultPixelShader = [](const PixelShaderData& data, const Vertex& v0)->Color
	{
		(data); // unreferenced
		return v0.c;
	};

	PixelShader TexturePixelShader = [](const PixelShaderData& data, const Vertex& v0)->Color
	{
		if (data.texture != nullptr)
			return data.texture->GetColor(v0.uv);
		else
			return v0.c;
	};

}