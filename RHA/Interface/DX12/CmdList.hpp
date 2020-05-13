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
					

			public: virtual void RecordSetPipelineState(ID3D12PipelineState *pipelineState) = 0;

			public: virtual void RecordSetGraphicsSignature(ID3D12RootSignature *signature) = 0;

			public: virtual void RecordSetComputeSignature(ID3D12RootSignature *signature) = 0;
			
			public: virtual void RecordCopyResource(ID3D12Resource *destination, ID3D12Resource *source) = 0;

			public: virtual void RecordCopyBufferRegion(ID3D12Resource *dstBuffer, size_t dstOffset, ID3D12Resource *srcBuffer, size_t srcOffset, size_t sizeInBytes) = 0;

			public: virtual void RecordDispatch(unsigned groupCountX, unsigned groupCountY, unsigned groupCountZ) = 0;
			
			
			public: virtual void RecordBarrierAliasing(ID3D12Resource *resourceBefore, ID3D12Resource *resourceAfter) = 0;

			public: virtual void RecordBarrierTransition(ID3D12Resource *resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) = 0;


			public: virtual void RecordSetGraphicsSignatureCbv(unsigned parameterIndex, D3D12_GPU_VIRTUAL_ADDRESS bufferAddress) = 0;

			public: virtual void RecordSetGraphicsSignatureTable(unsigned parameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE startAddress) = 0;
			
			public: virtual void RecordSetComputeSignatureCbv(unsigned parameterIndex, D3D12_GPU_VIRTUAL_ADDRESS bufferAddress) = 0;

			public: virtual void RecordSetComputeSignatureTable(unsigned parameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE startAddress) = 0;
			
			
			public: virtual void RecordSetRenderTargets
			(
				unsigned numTargets,
				const D3D12_CPU_DESCRIPTOR_HANDLE *targetDescriptors,
				bool isTargetDescriptorARangeStart,
				const D3D12_CPU_DESCRIPTOR_HANDLE *dsv
			) = 0;

			public: virtual void RecordClearRtv
			(
				D3D12_CPU_DESCRIPTOR_HANDLE descriptor,
				const float(&color)[4]
			) = 0;
			
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

			public: virtual void RecordSetViewports
			(
				const D3D12_VIEWPORT *viewports,
				size_t numViewports
			) = 0;

			public: virtual void RecordSetScissorRects
			(
				const D3D12_RECT *rects,
				size_t numRects
			) = 0;

			
			public: virtual void StopRecording() = 0;

			public: virtual void StartRecording(CmdAllocator *allocator) = 0;

			public: virtual void StartRecording(CmdAllocator *allocator, ID3D12PipelineState *initialPipeline) = 0;
					
		};

		
	}

	
}
