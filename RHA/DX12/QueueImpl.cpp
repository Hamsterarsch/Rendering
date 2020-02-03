#include "DX12/DeviceResources.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "DX12/QueueImpl.hpp"
#include "DX12/CmdList.hpp"
#include "DX12/Fence.hpp"


namespace RHA
{
	namespace DX12
	{
		QueueImpl::QueueImpl(class DeviceResources *resources, const D3D12_COMMAND_LIST_TYPE type, const bool isHighPriority)
		{
			D3D12_COMMAND_QUEUE_DESC desc;
						
			desc.Type = type;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.Priority = isHighPriority ? D3D12_COMMAND_QUEUE_PRIORITY_HIGH : D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			
			constexpr int FIRST_ADAPTER{ 0 };
			desc.NodeMask = FIRST_ADAPTER;

			const auto result
			{
				resources->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue))
			};

			if(FAILED(result))
			{
				throw Exception::CreationFailed{ "Could not create dx12 command queue" };
			}
			
		}
		

		
		void QueueImpl::SubmitCommandList(CmdList *list)
		{			
			queue->ExecuteCommandLists(1, list->GetList().GetAddressOf());
			
		}


				
		void QueueImpl::Signal(const size_t value, Fence *fence)
		{
			fence->Signal(value, this);
			
		}


		
		void QueueImpl::Wait(const size_t minimumValue, Fence *fence)
		{
			const auto result
			{
				queue->Wait(fence->GetFence().Get(), minimumValue)
			};

			if(FAILED(result))
			{
				throw Exception::Exception{ "Could not schedule a wait for a dx12 fence on the queue" };
			}
			
		}

		
	}
	
	
}
