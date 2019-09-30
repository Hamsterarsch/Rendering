#include "Interface/DX12/Facade.hpp"
#include "DX12/DeviceResourcesImpl.hpp"

namespace RHA
{
	namespace DX12
	{
		UniquePtr<DeviceResources> Facade::MakeDeviceResources(const D3D_FEATURE_LEVEL minimumFeatureLevel, const bool shouldEnableDebugLayers)
		{
			return std::make_unique<DeviceResourcesImpl>(minimumFeatureLevel, shouldEnableDebugLayers);
			
		}

		
	}

	
}