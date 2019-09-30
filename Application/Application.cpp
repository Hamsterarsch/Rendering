#include "Window/Windows/Window.hpp"
#include "Types/Dimensions2D.hpp"
#include "DX12/Facade.hpp"


int main()
{
	try
	{
		Windows::Window window{ {1920, 1080}, L"Window", L"UniqueClassName" };

		auto resc
		{
			RHA::DX12::Facade::MakeDeviceResources()
		};

		
	}
	catch(...)
	{		
		return -1;
	}

	return 0;
	
}
