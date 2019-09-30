#pragma once
#include "Shared/PtrTypes.hpp"
#include "RenderingExportHelper.hpp"


namespace RHA
{
	namespace DX12
	{
		class RHA_DLLSPEC Facade
		{
		public:
			static UniquePtr<class DeviceResources> MakeDeviceResources();

			
		};

		
	}

	
}
