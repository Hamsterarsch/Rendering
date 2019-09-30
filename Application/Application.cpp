#include "Window/Windows/Window.hpp"
#include "Types/Dimensions2D.hpp"
#include "DX12/Facade.hpp"
#include "DX12/DeviceResources.hpp"


int main()
{
	try
	{
		Windows::Window window{ {1920, 1080}, L"Window", L"UniqueClassName" };

		auto resc
		{
			RHA::DX12::Facade::MakeDeviceResources(D3D_FEATURE_LEVEL_12_0, true)
		};

		
	}
	catch(...)
	{		
		return -1;
	}

	return 0;
	
}
