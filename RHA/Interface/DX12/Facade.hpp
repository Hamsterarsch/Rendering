#pragma once
#include "Shared/PtrTypes.hpp"
#include "RenderingExportHelper.hpp"


namespace RHA
{
	namespace DX12
	{
		RHA_DLLSPEC class Facade
		{
		public:
			static UniquePtr<class DeviceResources> MakeDeviceResources();

			
		};

		
	}

	
}
