#pragma once
#include <Windows.h>
#include <array>
#include "DX12/DescriptorHeapImpl.hpp"
#include "DX12/WindowSurface.hpp"


namespace RHA
{
	namespace DX12
	{
		class WindowSurfaceImpl : public WindowSurface
		{
			private: static constexpr short bufferCount{ 2 };

			private: DxPtr<IDXGISwapChain1> swapChain;

			private: std::array<DxPtr<ID3D12Resource>, bufferCount> renderTargets;

			private: DescriptorHeapImpl viewHeap;
			

			
			public: WindowSurfaceImpl(class DeviceResources *resources, class Queue *queue, HWND window);

				private: void CreateSwapChain(class DeviceResources *resources, class Queue *queue, HWND window);
			
					private: static void CheckSwapChainCreation(HRESULT result);

				private: void CreateViewsForChainBuffers(class DeviceResources *resources);
			
					private: static void CheckBufferQuery(HRESULT result);

			
			public: inline D3D12_CPU_DESCRIPTOR_HANDLE GetBackbuffer() { return viewHeap.GetHandleCpu(0); }
			
			public: void Present();

			
			
		};

		
	}
	
	
}
