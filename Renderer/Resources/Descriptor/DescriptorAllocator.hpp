#pragma once
#include <d3d12.h>
#include "Resources/Descriptor/DescriptorChunk.hpp"

namespace RHA
{
	namespace DX12
	{
		class DeviceResources;
	}
}


namespace Renderer
{	
	namespace DX12
	{
		class DescriptorMemory;
		
		class DescriptorAllocator
		{			
			private: struct ChunkData
			{
				DescriptorChunk chunk;
				
				int offsetToTableStart;

				size_t offsetToAfterTable;
				
			};

			private: ChunkData view, sampler;

			private: RHA::DX12::DeviceResources *resources;

			private: DescriptorMemory *parent;


			
			public: DescriptorAllocator(RHA::DX12::DeviceResources *resources, DescriptorMemory *parent, const DescriptorChunk &viewChunk, const DescriptorChunk &samplerChunk);

			public: DescriptorAllocator(DescriptorAllocator &&Other) noexcept;
			
			public: ~DescriptorAllocator() noexcept;
								
			
			public: DescriptorAllocator(const DescriptorAllocator &) = delete;
			
			public: DescriptorAllocator &operator=(const DescriptorAllocator &) = delete;

			public: DescriptorAllocator &operator=(DescriptorAllocator &&) = delete;

								
			public: void OpenNewTable();
			
			public: D3D12_GPU_DESCRIPTOR_HANDLE GetCurrentTableStartForView() const;
			
			public: D3D12_GPU_DESCRIPTOR_HANDLE GetCurrentTableStartForSampler() const;
			
			public: void CreateDefaultedSrv(ID3D12Resource *resource, size_t tableOffset);

				private: void CreateSrvInternal(ID3D12Resource *resource, size_t tableOffset, const D3D12_SHADER_RESOURCE_VIEW_DESC *desc);
			
					private: static size_t GetTargetDescriptorIndex(const ChunkData &forChunkData, size_t offsetFromTableStartToDescriptor);
									
					private: void CheckIfValidOpenTable() const;

					private: D3D12_CPU_DESCRIPTOR_HANDLE GetViewHandleCpu(size_t index) const;

					private: void UpdateAfterTableIndex(const ChunkData &forChunkData, size_t offsetFromTableStartToDescriptor);

			public: void CreateSrvBuffer(ID3D12Resource *resource, size_t tableOffset, size_t firstIndex, size_t numElements, size_t strideInBytes);
			
						 		
			public: void CreateCbv(ID3D12Resource *resource, size_t tableOffset, size_t bufferSizeInBytes);

			public: void CreateSampler(const D3D12_SAMPLER_DESC &desc, size_t tableOffset);

			public: void CreateUavBuffer(ID3D12Resource *resource, size_t tableOffset, size_t firstIndex, size_t numElements, size_t strideInBytes);

				private: void CreateUavBufferInternal(ID3D12Resource *resource, ID3D12Resource *counter, size_t tableOffset, size_t firstIndex, size_t numElements, size_t strideInBytes, DXGI_FORMAT format);

			public: void CreateUavBufferWithCounter(ID3D12Resource *resource, ID3D12Resource *counter, size_t tableOffset, size_t firstIndex, size_t numElements, size_t strideInBytes);

			public: void CreateUavBufferFormatted(ID3D12Resource *resource, size_t tableOffset, size_t firstIndex, size_t numElements, DXGI_FORMAT format);
				
		};

		
	}

	
}
