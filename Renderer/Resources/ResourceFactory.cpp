#include "DX12/Facade.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "Resources/ResourceTypes.hpp"
#include "Resources/ResourceFactory.hpp"
#include "Utility/Alignment.hpp"
#include "Shared/Debugging.hpp"
#include "RendererFacadeImpl.hpp"
#include "Resources/DataSource.hpp"
#include "Commands/Internal/AliasingTransitionCommand.hpp"
#include "Commands/Internal/CopyBufferRegionCommand.hpp"
#include "Commands/Internal/CopyBufferToTextureCommand.hpp"
#include "Commands/Internal/DiscardResourceCommand.hpp"
#include "Commands/Basic/TransitionResourceCommand.hpp"


namespace Renderer::DX12
{
	using namespace RHA::DX12;
	
	ResourceFactory::ResourceFactory
	(
		DeviceResources *resources,
		RendererFacade &renderer,
		ResourceRegistry &registry,
		UniquePtr<AllocatableGpuMemory> &&bufferMemory,
		UniquePtr<AllocatableGpuMemory> &&textureMemory,
		UniquePtr<AllocatableGpuMemory> &&depthTextureMemory,
		UniquePtr<AllocatableGpuMemory> &&bufferReadbackMemory,
		UniquePtr<AllocatableGpuMemory> &&uploadMemory
	)	:			
		resources{ resources },
		renderer{ &renderer },
		registry{ &registry },
		clearValue{ nullptr },
		bufferMemory{ std::move(bufferMemory) },
		textureMemory{ std::move(textureMemory) },
		depthTextureMemory{ std::move(depthTextureMemory) },
		bufferReadbackMemory{ std::move(bufferReadbackMemory) },
		uploadMemory{ std::move(uploadMemory) }
	{
		depthTextureClearValue.DepthStencil.Depth = 1;
		depthTextureClearValue.DepthStencil.Stencil = 0;
				
	}

	ResourceFactory::~ResourceFactory() noexcept = default;
	

	
	ResourceHandle::t_hash ResourceFactory::MakeBufferWithData
	(
		const DataSource *dataSources,
		const unsigned char numDataSources,
		const size_t resourceSizeInBytes,
		const D3D12_RESOURCE_STATES desiredState,
		const D3D12_RESOURCE_FLAGS flags
	)
	{
		return MakeBufferWithDataInternal(*bufferMemory, ResourceTypes::Buffer, dataSources, numDataSources, resourceSizeInBytes, desiredState, flags);
		
	}

		ResourceHandle::t_hash ResourceFactory::MakeBufferWithDataInternal
		(
			AllocatableGpuMemory &memorySource,
			const ResourceTypes allocationType,			
			const DataSource *dataSources,
			const unsigned char numDataSources,
			const size_t resourceSizeInBytes,
			const D3D12_RESOURCE_STATES desiredState,
			const D3D12_RESOURCE_FLAGS flags
		)
		{
			auto resource{ MakePlacedBufferResource(memorySource, allocationType, RHA::Utility::IncreaseValueToAlignment(resourceSizeInBytes, 256), flags, D3D12_RESOURCE_STATE_COPY_DEST) };

			auto dataSourceResource{ MakeUploadBuffer(dataSources, numDataSources, resourceSizeInBytes) };
			renderer->SubmitCommand(MakeUnique<Commands::CopyBufferRegionCommand>(dataSourceResource, resource, resourceSizeInBytes, 0, 0));
			renderer->SubmitCommand(MakeUnique<Commands::TransitionResourceCommand>(resource, D3D12_RESOURCE_STATE_COPY_DEST, desiredState));
			renderer->RetireHandle(dataSourceResource);
		
			return resource;
		
		}

			ResourceHandle::t_hash ResourceFactory::MakePlacedBufferResource
			(
				AllocatableGpuMemory &memorySource,
				const ResourceTypes allocationType,
				const size_t sizeInBytes,
				const D3D12_RESOURCE_FLAGS resourceFlags,
				const D3D12_RESOURCE_STATES resourceState
			)
			{
				ResourceAllocation alloc{ this, allocationType };
				alloc.allocation = memorySource.Allocate(sizeInBytes);
							
				const auto desc{ MakeBufferDesc(sizeInBytes, resourceFlags) };
				constexpr decltype(nullptr) BUFFER_CLEAR_VALUE{ nullptr };
				const auto result
				{		
					resources->GetDevice()->CreatePlacedResource
					(
						alloc.allocation.heap, 
						alloc.allocation.offsetToAllocation,
						&desc,
						resourceState,
						BUFFER_CLEAR_VALUE,
						IID_PPV_ARGS(&alloc.resource)
					)
				};
				CheckGpuResourceCreation(result);

				const auto outResource{ registry->Register(std::move(alloc)) };
				renderer->SubmitCommand(MakeUnique<Commands::AliasingTransitionCommand>(0, outResource));
		
				return outResource;
			
			}
		
				D3D12_RESOURCE_DESC ResourceFactory::MakeBufferDesc(const size_t sizeInBytes, const D3D12_RESOURCE_FLAGS flags)
				{
					D3D12_RESOURCE_DESC desc{};
					desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
					desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
					desc.Width = sizeInBytes;
					desc.DepthOrArraySize = 1;
					desc.Height = 1;
					desc.MipLevels = 1;
					desc.SampleDesc.Count = 1;
					desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
					desc.Flags = flags;
			
					return desc;
				
				}
	
				void ResourceFactory::CheckGpuResourceCreation(const HRESULT result)
				{
					if(FAILED(result))
					{
						Exception::DebugBreak();
						throw Exception::CreationFailed{ "ResourceFactory:: could not create a dx12 placed buffer on gpu heap to upload to" };
					}
				
				}
	

	
			ResourceHandle::t_hash ResourceFactory::MakeUploadBuffer(const DataSource *dataSources, const unsigned char numDataSources, const size_t resourceSizeInBytes)
			{				
				const auto uploadResourceHandle{ MakePlacedBufferResource(*uploadMemory, ResourceTypes::UploadResource, resourceSizeInBytes, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ) };
				auto *uploadResource{ registry->GetResource(uploadResourceHandle) };
				
				void *uploadDataStart{ nullptr };

				const D3D12_RANGE nothingReadRange{ 0, 0 };
				if(FAILED(uploadResource->Map(0, &nothingReadRange, &uploadDataStart)))
				{
					throw Exception::Exception{ "ResourceFactory: Could not map texture upload resource to upload texture data"};
				}
		
				size_t offsetIntoResource{ 0 };

				for(unsigned char dataSourceIndex{ 0 }; dataSourceIndex < numDataSources; ++dataSourceIndex)			
				{
					const auto &dataSource{ dataSources[dataSourceIndex] };

					auto *dataDst{ static_cast<char *>(uploadDataStart)+offsetIntoResource };
					if(dataSource.data)
					{
						std::memcpy(dataDst, dataSource.data, dataSource.sizeInBytes);
					}
					else
					{
						std::memset(dataDst, 0, dataSource.sizeInBytes);
					}
					offsetIntoResource += dataSource.sizeInBytes;													
				}

				const auto uninitializedBytes{ resourceSizeInBytes-offsetIntoResource };
				if(uninitializedBytes > 0)
				{
					std::memset(static_cast<char *>(uploadDataStart)+offsetIntoResource, 0, uninitializedBytes);
				}
						
				D3D12_RANGE writeRange{ 0, resourceSizeInBytes };
				uploadResource->Unmap(0, &writeRange);
									
				return uploadResourceHandle;
			
			}
	

	
	ResourceHandle::t_hash ResourceFactory::MakeReadbackBuffer
	(		
		const size_t sizeInBytes, 	
		const D3D12_RESOURCE_FLAGS flags
	)
	{		
		const auto readbackResource
		{
			MakePlacedBufferResource
			(
				*bufferReadbackMemory,
				ResourceTypes::ReadbackBuffer,
				sizeInBytes,
				flags,
				D3D12_RESOURCE_STATE_COPY_DEST
			)
		};

		renderer->SubmitCommand(MakeUnique<Commands::AliasingTransitionCommand>(0, readbackResource));
		return readbackResource;
		
	}



	ResourceHandle::t_hash ResourceFactory::MakeTextureWithData
	(
		const void *data,
		const size_t width,
		const size_t height,
		const D3D12_RESOURCE_STATES desiredState,
		const D3D12_RESOURCE_FLAGS flags
	)
	{
		clearValue = nullptr;
		return MakeTextureWithDataInternal(*textureMemory, ResourceTypes::Texture, data, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 4, desiredState, flags);
		
	}

		ResourceHandle::t_hash ResourceFactory::MakeTextureWithDataInternal
		(
			AllocatableGpuMemory &memorySource,
			const ResourceTypes allocationType,
			const void *data,
			const size_t width,
			const size_t height,
			const DXGI_FORMAT format,
			const unsigned texelSizeInBytes,
			const D3D12_RESOURCE_STATES desiredState,
			const D3D12_RESOURCE_FLAGS flags
		)
		{			
			D3D12_PLACED_SUBRESOURCE_FOOTPRINT pf;
			pf.Footprint =
			{
				format,
				static_cast<UINT>(width),
				static_cast<UINT>(height),
				1,
				static_cast<UINT>(RHA::Utility::IncreaseValueToAlignment(texelSizeInBytes*width, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT))
			};
			pf.Offset = 0;
		
			const auto textureSizeInBytes
			{
				pf.Footprint.Width
				* pf.Footprint.Height
				* pf.Footprint.Depth
				* texelSizeInBytes
			};	//todo: check if width should actually be row pitch	
		
			const auto dataSource{ MakeUploadTexture(data, textureSizeInBytes, pf.Footprint, texelSizeInBytes) };
			const auto texture
			{
				MakePlacedTextureResource
				(
					memorySource,
					allocationType,
					format,
					textureSizeInBytes,
					pf.Footprint.Width,
					pf.Footprint.Height,
					pf.Footprint.Depth,
					flags,
					D3D12_RESOURCE_STATE_COPY_DEST
				)
			};

			renderer->SubmitCommand(MakeUnique<Commands::CopyBufferToTextureCommand>(dataSource, texture, pf, 0));
			renderer->SubmitCommand(MakeUnique<Commands::TransitionResourceCommand>(texture, D3D12_RESOURCE_STATE_COPY_DEST, desiredState));
			renderer->RetireHandle(dataSource);
		
			return texture;
		
		
		}

			ResourceHandle::t_hash ResourceFactory::MakePlacedTextureResource
			(
				AllocatableGpuMemory &memorySource,
				const ResourceTypes allocationType,
				const DXGI_FORMAT format,
				const size_t textureSizeInBytes,
				const size_t width,
				const size_t height,
				const unsigned depth,
				const D3D12_RESOURCE_FLAGS resourceFlags,
				const D3D12_RESOURCE_STATES resourceState
			)
			{
				ResourceAllocation alloc{ this, allocationType };
				alloc.allocation = memorySource.Allocate(textureSizeInBytes);
			
				D3D12_RESOURCE_DESC desc{};
				desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
				desc.Width = static_cast<UINT>(width);
				desc.Height = static_cast<UINT>(height);
				desc.DepthOrArraySize = depth;
				desc.Flags = resourceFlags;
				desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
				desc.Format = format;
				desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
				desc.MipLevels = 1;
				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;
								
				const auto result
				{		
					resources->GetDevice()->CreatePlacedResource
					(
						alloc.allocation.heap, 
						alloc.allocation.offsetToAllocation,
						&desc,
						resourceState,
						clearValue,
						IID_PPV_ARGS(&alloc.resource)
					)
				};
				CheckGpuResourceCreation(result);

				const auto outResource{ registry->Register(std::move(alloc)) };
				renderer->SubmitCommand(MakeUnique<Commands::AliasingTransitionCommand>(0, outResource));
		
				return outResource;
			
			}



	ResourceHandle::t_hash ResourceFactory::MakeUploadTexture
	(
		const void *data,
		const size_t textureSizeInBytes,
		const  D3D12_SUBRESOURCE_FOOTPRINT &footprint,
		const unsigned texelSizeInBytes
	)
	{		
		const auto uploadResourceHandle{ MakePlacedBufferResource(*uploadMemory, ResourceTypes::UploadResource, textureSizeInBytes, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ) };
				
		auto *uploadResource{ registry->GetResource(uploadResourceHandle) };							
		void *uploadDataStart{ nullptr };
		const D3D12_RANGE nothingReadRange{ 0, 0 };
		if(FAILED(uploadResource->Map(0, &nothingReadRange, &uploadDataStart)))
		{
			throw Exception::Exception{ "ResourceFactory: Could not map texture upload resource to upload texture data"};
		}
		auto *byteData{ static_cast<const char *>(data) };
		
		for(unsigned scanlineIndex{ 0 }; scanlineIndex < footprint.Height; ++scanlineIndex)
		{
			char *uploadPos{ static_cast<char *>(uploadDataStart) + scanlineIndex*footprint.RowPitch };
			const void *copyPos{ &byteData[scanlineIndex * footprint.Width * texelSizeInBytes] };
			std::memcpy(uploadPos, copyPos, footprint.Width * texelSizeInBytes);
		}

		D3D12_RANGE writeRange{ 0, textureSizeInBytes };				
		uploadResource->Unmap(0, &writeRange);
		
		renderer->SubmitCommand(MakeUnique<Commands::AliasingTransitionCommand>(0, uploadResourceHandle));
		return uploadResourceHandle;
						
	}


	
	ResourceHandle::t_hash ResourceFactory::MakeDepthTexture
	(
		const size_t width,
		const size_t height,
		const bool withStencil,
		const D3D12_RESOURCE_STATES desiredState, 
		const D3D12_RESOURCE_FLAGS flags
	)
	{
		constexpr auto texelSizeInBytes{ 4 };
		const auto sizeInBytes{ width * height * texelSizeInBytes };
		
		depthTextureClearValue.Format = withStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
		clearValue = &depthTextureClearValue;
		const auto depthTexture
		{
			MakePlacedTextureResource
			(
				*depthTextureMemory,
				ResourceTypes::DepthTexture,
				depthTextureClearValue.Format,
				sizeInBytes,
				width,
				height,
				1,						
				flags | D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
				desiredState
			)
		};

		//we have to discard here to make sure any existing texture metadata is cleared as to provide a valid and usable 'aliased' texture
		renderer->SubmitCommand(MakeUnique<Commands::DiscardResourceCommand>(depthTexture, 0, 1));
		return depthTexture;
						
	}

	

	void ResourceFactory::Deallocate(ResourceAllocation &allocation, const ResourceTypes type)
	{
		DeallocateInternal(allocation, type);
		
	}

	
}
