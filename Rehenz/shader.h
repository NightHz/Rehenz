#pragma once
#include "mesh.h"

namespace Rehenz
{
	extern VertexShader DefaultVertexShader;
	extern PixelShader DefaultPixelShader;
	extern PixelShader TexturePixelShader;

	extern VertexShader vs_transform;
	extern PixelShader ps_color;
	extern PixelShader ps_tex, ps_tex2;
	extern VertexShader vs_light;
	extern PixelShader ps_light;

}