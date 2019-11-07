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
			const auto result
			{
				fence->Signal(value)
			};

			if(FAILED(result))
			{
				throw Exception::Exception{ "Could not signal dx12 fence" };
			}
			
		}



		
		void FenceImpl::Signal(size_t value, Queue* queue)
		{
			const auto result
			{
				queue->GetQueue()->Signal(fence.Get(), value)
			};

			if(FAILED(result))
			{
				throw Exception::Exception{ "Could not schedule dx12 fence signal to queue" };
			}
			
		}



		
		void FenceImpl::SetEventOnValue(const size_t triggerValue, HANDLE eventHandle)
		{
			const auto result
			{
				fence->SetEventOnCompletion(triggerValue, eventHandle)
			};

			if(FAILED(result))
			{
				throw Exception::Exception{ "Could not set event on dx12 fence value" };
			}
			
		}
		
		
	}
	
	
}