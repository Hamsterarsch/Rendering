#pragma once
#include "Dx12/DeviceResources.hpp"
#include "DX12/CmdAllocator.hpp"


namespace RHA
{
	namespace DX12
	{
		class CmdAllocatorImpl : public CmdAllocator
		{
			private: DxPtr<ID3D12CommandAllocator> allocator;
			
			private: D3D12_COMMAND_LIST_TYPE type;

			private: SharedPtr<DeviceResources> resources;


			public: CmdAllocatorImpl(const SharedPtr<DeviceResources> &resources, D3D12_COMMAND_LIST_TYPE type);

			public: virtual inline D3D12_COMMAND_LIST_TYPE GetType() const override { return type; }
			
			public: virtual inline DxPtr<ID3D12CommandAllocator> GetAllocator() override { return allocator; }

			public: virtual UniquePtr<class CmdList> AllocateList() override;
			
			
		};
		
		
	}
	
	
}

