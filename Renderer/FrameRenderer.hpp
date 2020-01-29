#pragma once
#include "Shared/PtrTypes.hpp"
#include "RenderCommand.hpp"
#include <vector>
#include <d3d12.h>


namespace RHA
{
	namespace DX12
	{
		class WindowSurface;
		class DepthSurface;
		class DescriptorHeap;
		class Fence;
		class CmdAllocator;
		class Queue;
		class DeviceResources;
	}
}

struct ID3D12Resource;


namespace Renderer
{
	namespace DX12
	{
		using namespace RHA::DX12;
		
		class FrameRenderer
		{
			//takes in commands and batches them for gpu submission. renders everything to non swap chain targets

			private: DeviceResources *resources;

			private: Queue *queue;

			private: UniquePtr<CmdAllocator> allocator;

			private: UniquePtr<Fence> fence;

			private: HANDLE event;
			
			private: std::vector<UniquePtr<RenderCommand>> commands;

			private: ResourceRegistry *registry;

			private: WindowSurface *windowSurface;

			private: DepthSurface *depthSurface;
			
			private: static constexpr size_t recordsPerCommandList{ 50 };
					 					 			

			public: FrameRenderer();
			
			public: FrameRenderer(DeviceResources *resources, Queue *queue, ResourceRegistry &registry, WindowSurface &windowSurface, DepthSurface &depthSurface);

			public: FrameRenderer(FrameRenderer &&other) noexcept;

			public: FrameRenderer &operator=(FrameRenderer &&rhs) noexcept;
			
			public: FrameRenderer(const FrameRenderer &) = delete;

			public: FrameRenderer &operator=(const FrameRenderer &) = delete;

			
			public: ~FrameRenderer() noexcept;


			public: void AddCommand(UniquePtr<RenderCommand> &&command);

			public: void ExecuteCommands();
			
			public: void WaitForCompletion();
			
			public: void Reinitialize();

			public: inline bool IsInvalid() { return resources == nullptr; }


								
		};
		
		
	}
	
	
}
