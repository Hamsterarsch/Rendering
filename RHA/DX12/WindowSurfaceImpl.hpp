#pragma once
#include <Windows.h>
#include <array>
#include "DX12/DescriptorHeapImpl.hpp"
#include "DX12/WindowSurface.hpp"
#include "DX12/CmdAllocatorImpl.hpp"
#include "DX12/CmdList.hpp"
#include "DX12/Fence.hpp"


namespace RHA
{
	namespace DX12
	{
		struct BufferData
		{
			DxPtr<ID3D12Resource> resource;

			UniquePtr<CmdList> clearCmd, presentCmd;

			UniquePtr<Fence> clearFence, presentFence;

			HANDLE clearEvent, presentEvent;

		};
		
		class WindowSurfaceImpl : public WindowSurface
		{			
			private: static constexpr short bufferCount{ 2 };

			private: static constexpr float clearColor[4]{ 0,0,0,1 };

			private: DxPtr<IDXGISwapChain1> swapChain;

			private: std::array<BufferData, bufferCount> bufferData;

			private: DescriptorHeapImpl viewHeap;

			private: CmdAllocatorImpl cmdAllocator;

			private: unsigned currentBackbufferIndex;

			private: D3D12_VIEWPORT defaultViewport;

			private: D3D12_RECT defaultRect;

			
					 			
			public: WindowSurfaceImpl(class DeviceResources *resources, class Queue *queue, HWND window);

				private: void CreateSwapChain(class DeviceResources *resources, class Queue *queue, HWND window);
			
					private: static void CheckSwapChainCreation(HRESULT result);

				private: void CreateViewsForChainBuffers(class DeviceResources *resources);
			
					private: static void CheckBufferQuery(HRESULT result);
							 				
				private: void PopulateBufferData(DeviceResources *resources);
			
					private: void CreateClearCommandForBuffer(unsigned bufferIndex);
			
					private: void CreatePresentCommandForBuffer(unsigned bufferIndex);
			
					private: void CreateEventsForBuffer(unsigned bufferIndex);
			
					private: void CreateFencesForBuffer(unsigned bufferIndex, DeviceResources *resource);


			public: virtual inline DxPtr<ID3D12Resource> GetResourceTemplate() override { return bufferData[0].resource; };
			

			public: virtual void ScheduleBackbufferClear(Queue *queue) override;
			
				private: BufferData &GetBackbufferData();

			
			public: virtual void ScheduleCopyToBackbuffer(Queue *queue, CmdList *targetList, ID3D12Resource *source) override;
			
			public: virtual void SchedulePresentation(Queue *queue) override;

			public: virtual void RecordPipelineBindings(ID3D12GraphicsCommandList *list, const D3D12_CPU_DESCRIPTOR_HANDLE *depthDescriptor) override;
								
		};

		
	}
	
	
}
