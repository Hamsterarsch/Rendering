#pragma once
#include <Windows.h>
#include <array>
#include "DX12/DescriptorHeapImpl.hpp"
#include "DX12/WindowSurface.hpp"
#include "DX12/CmdList.hpp"


namespace RHA
{
	namespace DX12
	{		
		class WindowSurfaceImpl final : public WindowSurface
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
							 				
			public: ~WindowSurfaceImpl() override;

				public: bool IsValid() const;

					private: void Free();

			public: WindowSurfaceImpl(WindowSurfaceImpl && other) noexcept;

			public: WindowSurfaceImpl &operator=(WindowSurfaceImpl &&rhs) noexcept;

				private: void Invalidate();
			

			public: size_t  GetWidth() const override { return defaultViewport.Width; }

			public: size_t GetHeight() const override { return defaultViewport.Height; }

			public: void EnableVerticalSync() override { targetedVerticalBlank = 1; }

			public: void DisableVerticalSync() override { targetedVerticalBlank = 0; }
			
			public: DxPtr<ID3D12Resource> GetResourceTemplate() override { return buffers[0]; }
																	   			
			public: void Present() override;

			public: void RecordPipelineBindings(CmdList &list, const D3D12_CPU_DESCRIPTOR_HANDLE *depthDescriptor) override;

				private: ID3D12Resource *GetBackbuffer();

			public: void RecordPreparationForRendering(CmdList &list) override;

			public: void RecordPreparationForPresenting(CmdList &list) override;

			

			public: void GoFullscreen() override;

			public: void ResizeToWindow() override;
			
			public: void GoWindowed() override;

		};

		
	}
	
	
}
