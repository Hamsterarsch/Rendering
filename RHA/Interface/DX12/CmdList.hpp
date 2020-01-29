#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DxPtrTypes.hpp"
#include <d3d12.h>

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
					
			public: virtual void RecordClearDsv
			(
				D3D12_CPU_DESCRIPTOR_HANDLE descriptor,
				D3D12_CLEAR_FLAGS flags,
				float depthValue,
				unsigned char stencilValue
			) = 0;

			public: virtual void RecordClearDsv
			(
				D3D12_CPU_DESCRIPTOR_HANDLE descriptor,
				D3D12_CLEAR_FLAGS flags,
				float depthValue,
				unsigned char stencilValue,
				unsigned int numRects,
				const D3D12_RECT *clearRects
			) = 0;

			public: virtual void RecordSetPipelineState(ID3D12PipelineState *pipelineState) = 0;

			public: virtual void RecordSetGraphicsSignature(ID3D12RootSignature *signature) = 0;
			
			public: virtual void RecordCopyResource(ID3D12Resource *destination, ID3D12Resource *source) = 0;

			public: virtual void StopRecording() = 0;

			public: virtual void StartRecording(CmdAllocator *allocator) = 0;

			public: virtual void StartRecording(CmdAllocator *allocator, ID3D12PipelineState *initialPipeline) = 0;
					
		};

		
	}

	
}
