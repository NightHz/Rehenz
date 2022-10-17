#include "d3d12object.h"

namespace Rehenz
{
	D3d12RenderTarget::D3d12RenderTarget(UINT _width, UINT _height, DXGI_FORMAT _format, bool _msaa, bool _have_zbuffer,
		Color _target_bgc, UINT _target_rtv, UINT _zbuffer_dsv, D3d12Device* device)
		: width(_width), height(_height), format(_format), msaa(_msaa), have_zbuffer(_have_zbuffer),
		target_bgc(_target_bgc), target_rtv(_target_rtv), zbuffer_dsv(_zbuffer_dsv)
	{
		// create target
		auto rc_desc = D3d12Util::GetTexture2dRcDesc(width, height, 1, format, 1, msaa, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		auto clear_value = D3d12Util::GetTexture2dClearStruct(format, target_bgc.v);
		target = std::make_shared<D3d12Texture>(rc_desc, D3D12_HEAP_TYPE_DEFAULT, device->Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, &clear_value);
		if (!target->Get())
		{
			target = nullptr;
			return;
		}
		device->Get()->CreateRenderTargetView(target->Get(), nullptr, device->GetRtv(target_rtv));

		// create zbuffer
		if (have_zbuffer)
		{
			rc_desc = D3d12Util::GetTexture2dRcDesc(width, height, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, 1, msaa, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
			clear_value = D3d12Util::GetDepthStencilClearStruct(DXGI_FORMAT_D24_UNORM_S8_UINT, 1, 0);
			zbuffer = std::make_shared<D3d12Texture>(rc_desc, D3D12_HEAP_TYPE_DEFAULT, device->Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear_value);
			if (!zbuffer->Get())
			{
				target = nullptr;
				return;
			}
			device->Get()->CreateDepthStencilView(zbuffer->Get(), nullptr, device->GetDsv(zbuffer_dsv));
		}
		else
			zbuffer = nullptr;
	}

	D3d12RenderTarget::~D3d12RenderTarget()
	{
	}

	void D3d12RenderTarget::ClearRenderTargets(D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list)
	{
		cmd_list->ClearRenderTargetView(device->GetRtv(target_rtv), target_bgc.v, 0, nullptr);
		if (have_zbuffer)
			cmd_list->ClearDepthStencilView(device->GetDsv(zbuffer_dsv), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	}

	void D3d12RenderTarget::SetRenderTargets(D3d12Device* device, ID3D12GraphicsCommandList6* cmd_list)
	{
		auto rtv = device->GetRtv(target_rtv);
		auto dsv = device->GetDsv(zbuffer_dsv);
		if (have_zbuffer)
			cmd_list->OMSetRenderTargets(1, &rtv, true, &dsv);
		else
			cmd_list->OMSetRenderTargets(1, &rtv, true, nullptr);
	}

}