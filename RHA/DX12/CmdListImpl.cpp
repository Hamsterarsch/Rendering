#include "DX12/CmdAllocatorImpl.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "CmdListImpl.hpp"


namespace RHA
{
	namespace DX12
	{
		CmdListImpl::CmdListImpl(DeviceResources& resources, CmdAllocatorImpl &allocator)
		{
			constexpr int FIRST_ADAPTER{ 0 };
			
			const auto result
			{
				resources.GetDevice()->CreateCommandList
				(
					FIRST_ADAPTER,
					allocator.GetType(),
					allocator.GetAllocator().Get(),
					nullptr,
					IID_PPV_ARGS(&list)
				)
			};
		
			if(FAILED(result))
			{
				throw Exception::CreationFailed{ "Could not create dx12 command list" };
			}
		}

		DxPtr<ID3D12GraphicsCommandList> CmdListImpl::AsGraphicsList()
		{
			DxPtr<ID3D12GraphicsCommandList> out;

			const auto result
			{
				list.As(&out)
			};

			if(FAILED(result))
			{
				throw Exception::Exception{ "Conversion to dx12 graphics command list failed" };
			}

			return out;
			
		}

		
	}
	
	
}