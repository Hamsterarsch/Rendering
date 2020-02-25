#pragma once
#include "Shared/PtrTypes.hpp"
#include "RenderCommand.hpp"
#include <vector>
#include "Resources/HandleWrapper.hpp"
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
		
		//takes in commands and batches them for gpu submission. renders everything to non swap chain targets
		class FrameRenderer
		{
			private: DeviceResources *resources;

			private: Queue *queue;

			private: UniquePtr<CmdAllocator> allocator;

			private: UniquePtr<CmdList> list;

			private: UniquePtr<Fence> fence;
					 					 			
			private: HANDLE event;
			
			private: std::vector<UniquePtr<RenderCommand>> commands;

			private: ResourceRegistry *registry;

			private: WindowSurface *windowSurface;

			private: DepthSurface *depthSurface;

			private: size_t commandsRecordedToList;
			
			private: static constexpr size_t recordsPerCommandList{ 50 };

			private: static constexpr size_t fenceCmdCompletionValue{ 1 }, fenceQueueReleaseValue{ 2 };

			private: HandleWrapper globalBufferHandle;

			

			public: FrameRenderer();
			
			public: FrameRenderer(DeviceResources *resources, Queue *queue, ResourceRegistry &registry, WindowSurface &windowSurface, DepthSurface &depthSurface, HandleWrapper &&globalBuffer);

			public: FrameRenderer(FrameRenderer &&other) noexcept;

			public: FrameRenderer &operator=(FrameRenderer &&rhs) noexcept;
			
			public: FrameRenderer(const FrameRenderer &) = delete;

			public: FrameRenderer &operator=(const FrameRenderer &) = delete;
			
			public: ~FrameRenderer() noexcept;

			public: void UnregisterResources();
			
				private: inline bool RegistryIsValid() const { return registry != nullptr; }

				private: void UnregisterAllCommands();
			
			
			public: inline bool IsInvalid() const { return resources == nullptr; }

			public: void AddCommand(UniquePtr<RenderCommand> &&command);

			public: int ExecuteCommands();

				private: void RecordRenderTargetPreparations();
			
				private: void RecordCommands();

					private: void RecordFixedCommandState(RenderCommand &cmd) const;
			
					private: bool ListCapacityIsReached() const;

					private: void SubmitCurrentList();

					private: void ResetCurrentList();

				private: void SetupCompletionFence();
			
			public: void WaitForCompletion();

													
		};
		
		
	}
	
	
}
