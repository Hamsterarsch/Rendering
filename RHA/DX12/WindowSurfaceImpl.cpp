#include "DX12/WindowSurfaceImpl.hpp"
#include <dxgi1_2.h>


namespace RHA
{
	namespace DX12
	{
		WindowSurfaceImpl::WindowSurfaceImpl(DeviceResources &resources, HWND window, unsigned width, unsigned height)
		{
			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc{};
			fsDesc.Windowed = true;

			constexpr unsigned AUTO_OBTAIN{ 0 };
			constexpr unsigned NO_MSAA{ 1 };
			
			DXGI_SWAP_CHAIN_DESC1 swapDesc{};
			swapDesc.Width = AUTO_OBTAIN;
			swapDesc.Height = AUTO_OBTAIN;
			

			swapDesc.Stereo = false;
			swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapDesc.SampleDesc.Count = NO_MSAA;
			swapDesc.BufferCount = bufferCount;
			swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			/*
			const auto result
			{//todo: needs a cmd queue to be performed - implement context
				resources.GetDxgiFactory()->CreateSwapChainForHwnd()
			}
			*/
		}

		
	}
	
}