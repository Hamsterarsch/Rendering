#pragma once
#include <d3d12.h>
#include "DxPtrTypes.hpp"
#include "DX12/Queue.hpp"


namespace RHA
{
	namespace DX12
	{
		class QueueImpl : public Queue
		{
			private: DxPtr<ID3D12CommandQueue> queue;


			public: QueueImpl(class DeviceResources *resources, D3D12_COMMAND_LIST_TYPE type);

			public: virtual DxPtr<ID3D12CommandQueue> GetQueue() override { return queue; }
			
			public: virtual void SubmitCommandList(CmdList *list) override;

			public: virtual void Signal(size_t value, Fence *fence) override;
			
		};
		
		
	}
	
	
}
