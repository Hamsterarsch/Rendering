#pragma once
#include "DX12/DeviceResources.hpp"
#include "DX12/CmdAllocator.hpp"
#include "DX12/Queue.hpp"
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

			static UniquePtr<CmdAllocator> MakeCmdAllocator(DeviceResources *resources, D3D12_COMMAND_LIST_TYPE type);

			static UniquePtr<Queue> MakeQueue(DeviceResources *resources, D3D12_COMMAND_LIST_TYPE type);
			
		};

		
	}

	
}
