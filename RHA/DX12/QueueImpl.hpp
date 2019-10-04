#pragma once
#include <d3d12.h>
#include "DxPtrTypes.hpp"


namespace RHA
{
	namespace DX12
	{
		//work submission to queue
		class QueueImpl
		{
			private: DxPtr<ID3D12CommandQueue> queue;


			public: QueueImpl(class DeviceResources &resources, D3D12_COMMAND_LIST_TYPE type);
			
			
		};
		
		
	}
	
	
}