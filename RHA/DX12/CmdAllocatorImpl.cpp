#include "Shared/Exception/CreationFailedException.hpp"
#include "DX12/CmdAllocatorImpl.hpp"
#include "DX12/CmdListImpl.hpp"

namespace RHA
{
	namespace DX12
	{
		CmdAllocatorImpl::CmdAllocatorImpl(DeviceResources *resources, D3D12_COMMAND_LIST_TYPE type) :
			type{ type },
			resources{ resources }
		{
			const auto result
			{
				resources->GetDevice()->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator))
			};
			CheckCreation(result);
			
		}

			void CmdAllocatorImpl::CheckCreation(const HRESULT result)
			{
				if(FAILED(result))
				{
					throw Exception::CreationFailed{ "Could not create dx12 command allocator" };
				}
			
			}


		
		UniquePtr<CmdList> CmdAllocatorImpl::AllocateList()
		{			
			return std::make_unique<CmdListImpl>(*resources, *this);
			
		}


		
		HRESULT CmdAllocatorImpl::Reset()
		{
			return allocator->Reset();
			
		}

		
	}

	
}
