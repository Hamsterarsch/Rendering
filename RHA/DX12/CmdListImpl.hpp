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

				private: DxPtr<ID3D12GraphicsCommandList> QueryGraphicsList();

				private: static void CheckCreation(HRESULT result);
			
						 			
			public: inline virtual DxPtr<ID3D12CommandList> GetList() override { return list; }

			public: virtual DxPtr<ID3D12GraphicsCommandList> AsGraphicsList() override { return glist; }

			
			public: virtual void RecordClearDsv
			(
				D3D12_CPU_DESCRIPTOR_HANDLE descriptor,
				D3D12_CLEAR_FLAGS flags,
				float depthValue,
				unsigned char stencilValue
			) override;
			
				public: virtual void RecordClearDsv
				(
					D3D12_CPU_DESCRIPTOR_HANDLE descriptor,
					D3D12_CLEAR_FLAGS flags,
					float depthValue,
					unsigned char stencilValue,
					unsigned int numRects,
					const D3D12_RECT *clearRects
				) override;

			
			public: virtual void RecordSetPipelineState(ID3D12PipelineState *pipelineState) override;

			public: virtual void RecordSetGraphicsSignature(ID3D12RootSignature *signature) override;
			
			public: virtual void RecordCopyResource(ID3D12Resource *destination, ID3D12Resource *source) override;

			public: virtual void RecordSetRenderTargets
			(
				unsigned numTargets,
				const D3D12_CPU_DESCRIPTOR_HANDLE *targetDescriptors,
				bool isTargetDescriptorARangeStart,
				const D3D12_CPU_DESCRIPTOR_HANDLE *dsv
			) override;
			
			
			public: virtual void StopRecording() override;

				private: static void CheckClose(HRESULT result);
			
			
			public: virtual void StartRecording(CmdAllocator *allocator) override;
								
				public: virtual void StartRecording(CmdAllocator *allocator, ID3D12PipelineState *initialPipeline) override;

					private: static void CheckReset(HRESULT result);
								
		};
		
		
	}
	
	
}
