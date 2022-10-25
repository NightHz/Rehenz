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
		return ColorSaturate(v0.c);
	};

	PixelShader TexturePixelShader = [](const PixelShaderData& data, const Vertex& v0)->Color
	{
		Color c;
		if (data.texture != nullptr)
			c = data.texture->GetColor(v0.uv);
		else
			c = v0.c;
		return ColorSaturate(c);
	};

	VertexShader Shader::vs_transform = DefaultVertexShader;

	PixelShader Shader::ps_color = DefaultPixelShader;

	PixelShader Shader::ps_tex = [](const PixelShaderData& data, const Vertex& v0)->Color
	{
		Color c = v0.c;
		if (data.texture != nullptr)
			c = c * data.texture->GetColor(v0.uv);
		return ColorSaturate(c);
	};

	PixelShader Shader::ps_tex2 = [](const PixelShaderData& data, const Vertex& v0)->Color
	{
		Color c(0.1f, 0.1f, 0.1f);
		if (data.texture2 != nullptr)
			c = data.texture2->GetColor(v0.uv2);
		return ColorSaturate(c);
	};

	VertexShader Shader::vs_light = [](const VertexShaderData& data, const Vertex& v0)->Vertex
	{
		static const Color ambient = Color::white * 0.2f;
		static const Color diffuse = Color::white * 1.0f;
		static const Vector light_dir(-0.5f, -0.7f, 0.3f);

		Vertex v(v0);
		v.p = v.p * data.transform;
		v.c = ambient;

		Matrix normal_world = MatrixTranspose(data.mat_inv_world);
		normal_world(0, 3) = normal_world(1, 3) = normal_world(2, 3) = normal_world(3, 3) = 0;
		Vector N = VectorNormalize(v.n * normal_world); // normal
		Vector L = VectorNormalize(-light_dir);         // to light
		float s = VectorDot(L, N);
		if (s > 0)
			v.c += diffuse * s;

		return v;
	};

	PixelShader Shader::ps_light = [](const PixelShaderData& data, const Vertex& v0)->Color
	{
		static const Color ambient = Color::white * 0.2f;
		static const Color diffuse = Color::white * 1.0f;
		static const Vector light_dir(-0.5f, -0.7f, 0.3f);

		Color c = ambient;

		Matrix normal_world = MatrixTranspose(data.mat_inv_world);
		normal_world(0, 3) = normal_world(1, 3) = normal_world(2, 3) = normal_world(3, 3) = 0;
		Vector N = VectorNormalize(v0.n * normal_world); // normal
		Vector L = VectorNormalize(-light_dir);          // to light
		float s = VectorDot(L, N);
		if (s > 0)
			c += diffuse * s;

		if (data.texture != nullptr)
			c = c * data.texture->GetColor(v0.uv);

		return ColorSaturate(c);
	};

}