#pragma once
#include "DX12/DeviceResources.hpp"


namespace RHA
{
	namespace DX12
	{
		class WindowSurfaceImpl
		{
			private: static constexpr short bufferCount{ 2 };
			
			public: WindowSurfaceImpl(DeviceResources &resources, HWND window, unsigned width, unsigned height);
			
		};

		
	}
	
	
}
