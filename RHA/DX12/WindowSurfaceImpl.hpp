#pragma once
#include <Windows.h>


namespace RHA
{
	namespace DX12
	{
		class WindowSurfaceImpl
		{
			private: static constexpr short bufferCount{ 2 };

			private: DxPtr<IDXGISwapChain1> swapChain;
			
			
			public: WindowSurfaceImpl(class DeviceResources *resources, class Queue *queue, HWND window, unsigned width, unsigned height);
			
		};

		
	}
	
	
}
