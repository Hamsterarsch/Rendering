#pragma once
#include "DX12/DeviceResources.hpp"
#include "DX12/CmdList.hpp"


namespace RHA
{
	namespace DX12
	{
		class CmdListImpl : public CmdList
		{
			private: DxPtr<ID3D12CommandList> list;
			

			public: CmdListImpl(DeviceResources &resources, class CmdAllocatorImpl &allocator);

			public: inline virtual DxPtr<ID3D12CommandList> GetList() override { return list; }

			public: virtual DxPtr<ID3D12GraphicsCommandList> AsGraphicsList() override;
								
		};
		
		
	}
	
	
}
