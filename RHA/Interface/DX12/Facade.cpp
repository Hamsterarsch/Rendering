#include "Interface/DX12/Facade.hpp"
#include "DX12/DeviceResourcesImpl.hpp"

namespace RHA
{
	namespace DX12
	{
		UniquePtr<DeviceResources> Facade::MakeDeviceResources()
		{
			return std::make_unique<DeviceResourcesImpl>();
			
		}

		
	}

	
}