#include "DX12/CmdAllocatorImpl.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "CmdListImpl.hpp"
#include "Shared/Exception/OutOfMemoryException.hpp"


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

			glist = AsGraphicsList();
			
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

		void CmdListImpl::RecordCopyResource(ID3D12Resource *destination, ID3D12Resource *source)
		{
			glist->CopyResource(destination, source);
			
		}

		void CmdListImpl::StopRecording()
		{
			const auto result{ glist->Close() };
			
			if(FAILED(result))
			{
				if(result == E_OUTOFMEMORY)
				{
					throw Exception::OutOfMemory{ "Insufficient memory to close dx12 cmd list" };
				}
				
				throw Exception::Exception{ "Could not close dx12 cmd list" };
			}
			
		}

		void CmdListImpl::StartRecording(CmdAllocator *allocator)
		{
			glist->Reset(allocator->GetAllocator().Get(), nullptr);
			
		}

		
	}
	
	
}
