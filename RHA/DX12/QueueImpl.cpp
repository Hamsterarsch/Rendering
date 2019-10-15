#include "DX12/DeviceResources.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "DX12/QueueImpl.hpp"
#include "DX12/CmdList.hpp"


namespace RHA
{
	namespace DX12
	{
		QueueImpl::QueueImpl(class DeviceResources *resources, const D3D12_COMMAND_LIST_TYPE type)
		{
			D3D12_COMMAND_QUEUE_DESC desc{};

			constexpr int FIRST_ADAPTER{ 0 };
			
			desc.Type = type;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.NodeMask = FIRST_ADAPTER;
			desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

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

		
	}
	
	
}
