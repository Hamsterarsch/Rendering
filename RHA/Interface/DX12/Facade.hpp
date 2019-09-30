#pragma once
#include "DeviceResources.hpp"
#include "Shared/PtrTypes.hpp"
#include "RenderingExportHelper.hpp"


namespace RHA
{
	namespace DX12
	{
		class RHA_DLLSPEC Facade
		{
		public:
			static UniquePtr<DeviceResources> MakeDeviceResources(D3D_FEATURE_LEVEL minimumFeatureLevel, bool shouldEnableDebugLayers);

			
		};

		
	}

	
}
