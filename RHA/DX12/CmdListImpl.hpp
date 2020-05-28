#pragma once
#include "DX12/DeviceResources.hpp"
#include "DX12/CmdList.hpp"


namespace RHA::DX12
{
	class CmdListImpl final : public CmdList
	{
		private: DxPtr<ID3D12CommandList> list;

		private: DxPtr<ID3D12GraphicsCommandList> glist;

		
		
		public: CmdListImpl(DeviceResources &resources, class CmdAllocatorImpl &allocator);

			private: DxPtr<ID3D12GraphicsCommandList> QueryGraphicsList();

			private: static void CheckCreation(HRESULT result);
		
					 			
		public: inline DxPtr<ID3D12CommandList> GetList() override { return list; }

		public: DxPtr<ID3D12GraphicsCommandList> AsGraphicsList() override { return glist; }

		
		public: void RecordSetPipelineState(ID3D12PipelineState *pipelineState) override;

		public: void RecordSetGraphicsSignature(ID3D12RootSignature *signature) override;

		public: void RecordSetComputeSignature(ID3D12RootSignature *signature) override;
		
		public: void RecordCopyResource(ID3D12Resource *destination, ID3D12Resource *source) override;

		public: void RecordCopyBufferRegion(ID3D12Resource *dstBuffer, size_t dstOffset, ID3D12Resource *srcBuffer, size_t srcOffset, size_t sizeInBytes) override;

		public: void RecordDispatch(unsigned groupCountX, unsigned groupCountY, unsigned groupCountZ) override;
		
		
		public: void RecordBarrierAliasing(ID3D12Resource *resourceBefore, ID3D12Resource *resourceAfter) override;

		public: void RecordBarrierTransition(ID3D12Resource *resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) override;


		public: void RecordSetGraphicsSignatureCbv(unsigned parameterIndex, D3D12_GPU_VIRTUAL_ADDRESS bufferAddress) override;

		public: void RecordSetGraphicsSignatureTable(unsigned parameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE startAddress) override;
		
		public: void RecordSetComputeSignatureCbv(unsigned parameterIndex, D3D12_GPU_VIRTUAL_ADDRESS bufferAddress) override;

		public: void RecordSetComputeSignatureTable(unsigned parameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE startAddress) override;

		
		public: void RecordSetRenderTargets
		(
			unsigned numTargets,
			const D3D12_CPU_DESCRIPTOR_HANDLE *targetDescriptors,
			bool isTargetDescriptorARangeStart,
			const D3D12_CPU_DESCRIPTOR_HANDLE *dsv
		) override;

		public: void RecordClearRtv
		(
			D3D12_CPU_DESCRIPTOR_HANDLE descriptor,
			const float(&color)[4]
		) override;
		
		public: void RecordClearDsv
		(
			D3D12_CPU_DESCRIPTOR_HANDLE descriptor,
			D3D12_CLEAR_FLAGS flags,
			float depthValue,
			unsigned char stencilValue
		) override;
		
			public: void RecordClearDsv
			(
				D3D12_CPU_DESCRIPTOR_HANDLE descriptor,
				D3D12_CLEAR_FLAGS flags,
				float depthValue,
				unsigned char stencilValue,
				unsigned int numRects,
				const D3D12_RECT *clearRects
			) override;
		
		public: void RecordSetViewports
		(
			const D3D12_VIEWPORT *viewports,
			size_t numViewports
		) override;

		public: void RecordSetScissorRects
		(
			const D3D12_RECT *rects,
			size_t numRects
		) override;

		
		public: void StopRecording() override;

			private: static void CheckClose(HRESULT result);
		
		
		public: void StartRecording(CmdAllocator *allocator) override;
							
			public:  void StartRecording(CmdAllocator *allocator, ID3D12PipelineState *initialPipeline) override;

				private: static void CheckReset(HRESULT result);

						 			
		public: void RecordSetVertexBuffer(const D3D12_VERTEX_BUFFER_VIEW& view) override;
		
		public: void RecordSetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& view) override;
		
	};

	
}
