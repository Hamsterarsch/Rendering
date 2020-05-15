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
		class WindowSurfaceImpl : public WindowSurface
		{			
			private: static constexpr short bufferCount{ 2 };

			private: static constexpr float clearColor[4]{ 0,0,0,1 };

			private: DxPtr<IDXGISwapChain1> swapChain;

			private: std::array<DxPtr<ID3D12Resource>, bufferCount> buffers;

			private: DescriptorHeapImpl viewHeap;
					 
			private: unsigned currentBackbufferIndex;

			private: D3D12_VIEWPORT defaultViewport;

			private: D3D12_RECT defaultRect;

			private: unsigned char targetedVerticalBlank;

			
					 			
			public: WindowSurfaceImpl(class DeviceResources *resources, class Queue *queue, HWND window);

				private: void CreateSwapChain(class DeviceResources *resources, class Queue *queue, HWND window);
			
					private: static void CheckSwapChainCreation(HRESULT result);

					private: void InitializeFullscreenState();
			
					private: void QueryViewportInformation();

				private: void CreateViewsForChainBuffers(class DeviceResources *resources);
			
					private: static void CheckBufferQuery(HRESULT result);
							 				
			public: virtual ~WindowSurfaceImpl() override;
			

			public: virtual inline size_t  GetWidth() const override { return defaultViewport.Width; }

			public: virtual inline size_t GetHeight() const override { return defaultViewport.Height; }

			public: virtual inline void EnableVerticalSync() override { targetedVerticalBlank = 1; }

			public: virtual inline void DisableVerticalSync() override { targetedVerticalBlank = 0; }
			
			public: virtual inline DxPtr<ID3D12Resource> GetResourceTemplate() override { return buffers[0]; }
																	   			
			public: virtual void Present() override;

			public: virtual void RecordPipelineBindings(CmdList &list, const D3D12_CPU_DESCRIPTOR_HANDLE *depthDescriptor) override;

				private: ID3D12Resource *GetBackbuffer();

			public: virtual void RecordPreparationForRendering(CmdList &list) override;

			public: virtual void RecordPreparationForPresenting(CmdList &list) override;

		};

		
	}
	
	
}
