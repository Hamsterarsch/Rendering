#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DxPtrTypes.hpp"

struct ID3D12GraphicsCommandList;
struct ID3D12CommandList;
struct ID3D12Resource;

namespace RHA
{
	namespace DX12
	{
		class CmdAllocator;

		class CmdList
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(CmdList)

			public: virtual DxPtr<ID3D12CommandList> GetList() = 0;
			
			public: virtual DxPtr<ID3D12GraphicsCommandList> AsGraphicsList() = 0;

			public: virtual void RecordCopyResource(ID3D12Resource *destination, ID3D12Resource *source) = 0;

			public: virtual void StopRecording() = 0;

			public: virtual void StartRecording(CmdAllocator *allocator) = 0;
					
		};

		
	}

	
}
