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
		
			CheckCreation(result);
			glist = QueryGraphicsList();
			
		}

			void CmdListImpl::CheckCreation(const HRESULT result)
			{
				if(FAILED(result))
				{
					throw Exception::CreationFailed{ "Could not create dx12 command list" };
				}
				
			}

			DxPtr<ID3D12GraphicsCommandList> CmdListImpl::QueryGraphicsList()
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


					
		void CmdListImpl::RecordSetPipelineState(ID3D12PipelineState *pipelineState)
		{
			glist->SetPipelineState(pipelineState);
			
		}


		
		void CmdListImpl::RecordSetGraphicsSignature(ID3D12RootSignature *signature)
		{
			glist->SetGraphicsRootSignature(signature);
			
		}


		
		void CmdListImpl::RecordSetComputeSignature(ID3D12RootSignature *signature)
		{
			glist->SetComputeRootSignature(signature);
						
		}

		

		void CmdListImpl::RecordCopyResource(ID3D12Resource *destination, ID3D12Resource *source)
		{
			glist->CopyResource(destination, source);
			
		}

		void CmdListImpl::RecordCopyBufferRegion
		(
			ID3D12Resource *dstBuffer, 
			const size_t dstOffset, 
			ID3D12Resource *srcBuffer,
			const size_t srcOffset,
			const size_t sizeInBytes
		)
		{
			glist->CopyBufferRegion(dstBuffer, dstOffset, srcBuffer, srcOffset, sizeInBytes);
			
		}


		
		void CmdListImpl::RecordDispatch
		(
			const unsigned groupCountX, 
			const unsigned groupCountY, 
			const unsigned groupCountZ
		)		
		{
			glist->Dispatch(groupCountX, groupCountY, groupCountZ);
			
		}

		

		void CmdListImpl::RecordBarrierAliasing(ID3D12Resource *resourceBefore, ID3D12Resource *resourceAfter)
		{
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
			barrier.Aliasing.pResourceBefore = resourceBefore;
			barrier.Aliasing.pResourceAfter = resourceAfter;

			glist->ResourceBarrier(1, &barrier);			
			
		}


		
		void CmdListImpl::RecordBarrierTransition(ID3D12Resource *resource, const D3D12_RESOURCE_STATES stateBefore, const D3D12_RESOURCE_STATES stateAfter)
		{
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = resource;
			barrier.Transition.StateBefore = stateBefore;
			barrier.Transition.StateAfter = stateAfter;

			glist->ResourceBarrier(1, &barrier);
			
		}

		

		void CmdListImpl::RecordSetGraphicsSignatureCbv(const unsigned parameterIndex, const D3D12_GPU_VIRTUAL_ADDRESS bufferAddress)
		{
			glist->SetGraphicsRootConstantBufferView(parameterIndex, bufferAddress);
			
		}


		
		void CmdListImpl::RecordSetGraphicsSignatureTable(const unsigned parameterIndex, const D3D12_GPU_DESCRIPTOR_HANDLE startAddress)
		{
			glist->SetGraphicsRootDescriptorTable(parameterIndex, startAddress);
			
		}

		

		void CmdListImpl::RecordSetComputeSignatureCbv(const unsigned parameterIndex, const D3D12_GPU_VIRTUAL_ADDRESS bufferAddress)
		{
			glist->SetComputeRootConstantBufferView(parameterIndex, bufferAddress);
			
		}


		
		void CmdListImpl::RecordSetComputeSignatureTable
		(
			const unsigned parameterIndex,
			const D3D12_GPU_DESCRIPTOR_HANDLE startHandle
		)
		{
			glist->SetComputeRootDescriptorTable(parameterIndex, startHandle);
			
		}

		

		void CmdListImpl::RecordSetRenderTargets
		(
			const unsigned numTargets,
			const D3D12_CPU_DESCRIPTOR_HANDLE *targetDescriptors,
			const bool isTargetDescriptorARangeStart,
			const D3D12_CPU_DESCRIPTOR_HANDLE *dsv
		)
		{
			glist->OMSetRenderTargets(numTargets, targetDescriptors, isTargetDescriptorARangeStart, dsv);
			
		}


				
		void CmdListImpl::RecordClearRtv(D3D12_CPU_DESCRIPTOR_HANDLE descriptor, const float(&color)[4])
		{
			glist->ClearRenderTargetView(descriptor, color, 0, nullptr);
			
		}

		

		void CmdListImpl::RecordClearDsv
		(
			const D3D12_CPU_DESCRIPTOR_HANDLE descriptor,
			const D3D12_CLEAR_FLAGS flags,
			const float depthValue,
			const unsigned char stencilValue
		)
		{
			RecordClearDsv(descriptor, flags, depthValue, stencilValue, 0, nullptr);
			
		}

			void CmdListImpl::RecordClearDsv
			(
				const D3D12_CPU_DESCRIPTOR_HANDLE descriptor,
				const D3D12_CLEAR_FLAGS flags,
				const float depthValue,
				const unsigned char stencilValue, 
				const unsigned numRects,
				const D3D12_RECT *clearRects
			)
			{
				glist->ClearDepthStencilView(descriptor, flags, depthValue, stencilValue, numRects, clearRects);
			
			}

		

		void CmdListImpl::RecordSetViewports(const D3D12_VIEWPORT *const viewports, const size_t numViewports)
		{
			glist->RSSetViewports(numViewports, viewports);
			
		}

		

		void CmdListImpl::RecordSetScissorRects(const D3D12_RECT *const rects, const size_t numRects)
		{
			glist->RSSetScissorRects(numRects, rects);
			
		}

		

		void CmdListImpl::StopRecording()
		{
			const auto result{ glist->Close() };			
			CheckClose(result);
			
		}

			void CmdListImpl::CheckClose(const HRESULT result)
			{
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
			StartRecording(allocator, nullptr);
			
		}

			void CmdListImpl::StartRecording(CmdAllocator *allocator, ID3D12PipelineState *initialPipeline)
			{
				const auto result
				{
					glist->Reset(allocator->GetAllocator().Get(), initialPipeline)
				};
				CheckReset(result);
				
			}

				void CmdListImpl::CheckReset(const HRESULT result)
				{
					if(FAILED(result))
					{
						throw Exception::Exception{ "Could not reset dx12 graphics cmd list" };
					}
			
				}

		
	}
	
	
}
