#include "DX12/Queue.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "DX12/FenceImpl.hpp"


namespace RHA
{
	namespace DX12
	{
		FenceImpl::FenceImpl(DeviceResources *resource)
		{
			const auto result
			{
				resource->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence))
			};
			CheckFenceCreation(result);			
			
		}

			void FenceImpl::CheckFenceCreation(const HRESULT result)
			{
				if(FAILED(result))
				{
					throw Exception::CreationFailed{ "could not create dx12 fence" };
				}
			
			}

		void FenceImpl::Signal(size_t value)
		{
			fence->Signal(value);
			
		}

		void FenceImpl::Signal(size_t value, Queue* queue)
		{
			queue->GetQueue()->Signal(fence.Get(), value);
			
		}

		
	}
	
	
}