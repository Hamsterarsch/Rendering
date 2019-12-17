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

			private: DxPtr<ID3D12GraphicsCommandList> glist;
			

			public: CmdListImpl(DeviceResources &resources, class CmdAllocatorImpl &allocator);

			public: inline virtual DxPtr<ID3D12CommandList> GetList() override { return list; }

			public: virtual DxPtr<ID3D12GraphicsCommandList> AsGraphicsList() override;

			public: virtual void RecordCopyResource(ID3D12Resource *destination, ID3D12Resource *source) override;

			public: virtual void StopRecording() override;

			public: virtual void StartRecording(CmdAllocator *allocator) override;
								
		};
		
		
	}
	
	
}
