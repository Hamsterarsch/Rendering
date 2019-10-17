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

			private: static constexpr float clearColor[4]{ 0,0,0,1 };

			private: DxPtr<IDXGISwapChain1> swapChain;

			private: std::array<DxPtr<ID3D12Resource>, bufferCount> renderTargets;

			private: DescriptorHeapImpl viewHeap;
			

			
			public: WindowSurfaceImpl(class DeviceResources *resources, class Queue *queue, HWND window);

				private: void CreateSwapChain(class DeviceResources *resources, class Queue *queue, HWND window);
			
					private: static void CheckSwapChainCreation(HRESULT result);

				private: void CreateViewsForChainBuffers(class DeviceResources *resources);
			
					private: static void CheckBufferQuery(HRESULT result);


			public: inline virtual unsigned GetBufferCount() const override { return bufferCount; }
			
			public: virtual void ClearBuffer(DxPtr<ID3D12GraphicsCommandList> list, unsigned index) override;
			
			public: virtual void Present() override;
			
		};

		
	}
	
	
}
