#pragma once
#include "d3d12util.h"
#include "d3d12device.h"
#include "d3d12resource.h"
#include "color.h"

namespace Rehenz
{
	class D3d12RenderTarget
	{
	private:
		std::shared_ptr<D3d12Texture> target;
		std::shared_ptr<D3d12Texture> zbuffer;

	public:
		const UINT width;
		const UINT height;
		const DXGI_FORMAT format;
		bool msaa;
		bool have_zbuffer;
		const Color target_bgc;
		const UINT target_rtv;
		const UINT zbuffer_dsv;

	public:
		D3d12RenderTarget(UINT _width, UINT _height, DXGI_FORMAT _format, bool _msaa, bool _have_zbuffer,
			Color _target_bgc, UINT _target_rtv, UINT _zbuffer_dsv, D3d12Device* device);
		D3d12RenderTarget(UINT _width, UINT _height, UINT _target_rtv, UINT _zbuffer_dsv, D3d12Device* device)
			: D3d12RenderTarget(_width, _height, DXGI_FORMAT_B8G8R8A8_UNORM, false, true, Color::yellow_green_o, _target_rtv, _zbuffer_dsv, device) {}
		D3d12RenderTarget(const D3d12RenderTarget&) = delete;
		D3d12RenderTarget& operator=(const D3d12RenderTarget&) = delete;
		~D3d12RenderTarget();

		inline operator bool()
		{
			return target != nullptr;
		}
		inline ID3D12Resource2* Get()
		{
			return target->Get();
		}

		void ClearRenderTargets(D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list);
		void SetRenderTargets(D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list);
	};
}