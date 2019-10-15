#include "DX12/DeviceResources.hpp"
#include "DX12/Queue.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "DX12/WindowSurfaceImpl.hpp"


namespace RHA
{
	namespace DX12
	{
		WindowSurfaceImpl::WindowSurfaceImpl(DeviceResources *resources, Queue *queue, HWND window) :
			viewHeap{ resources, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, bufferCount, false }
		{
			CreateSwapChain(resources, queue, window);

			CreateViewsForChainBuffers(resources);
			
		}

			void WindowSurfaceImpl::CreateSwapChain(DeviceResources *resources, Queue *queue, HWND window)
			{
				DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc{};
				fsDesc.Windowed = true;
								
				constexpr unsigned AUTO_OBTAIN{ 0 };
				constexpr unsigned NO_MSAA{ 1 };

				DXGI_SWAP_CHAIN_DESC1 swapDesc{};
				swapDesc.Width = AUTO_OBTAIN;
				swapDesc.Height = AUTO_OBTAIN;
				swapDesc.Stereo = false;
				swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
				swapDesc.SampleDesc.Count = NO_MSAA;
				swapDesc.BufferCount = bufferCount;
				swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

				constexpr decltype(nullptr) NO_OUTPUT_RESTRICTION{ nullptr };

				const auto result
				{
					resources->GetDxgiFactory()->CreateSwapChainForHwnd
					(
						queue->GetQueue().Get(),
						window,
						&swapDesc,
						&fsDesc,
						NO_OUTPUT_RESTRICTION,
						&swapChain
					)
				};
				CheckSwapChainCreation(result);
			
			}

				void WindowSurfaceImpl::CheckSwapChainCreation(HRESULT result)
				{
					if (FAILED(result))
					{
						throw Exception::CreationFailed{ "Could not create dx12 window surface for windows window" };
					}
				
				}

			void WindowSurfaceImpl::CreateViewsForChainBuffers(DeviceResources *resources)
			{
				for (size_t bufferIndex{ 0 }; bufferIndex < renderTargets.size(); ++bufferIndex)
				{
					const auto result
					{
						swapChain->GetBuffer(bufferIndex, IID_PPV_ARGS(&renderTargets.at(bufferIndex)))
					};
					CheckBufferQuery(result);

					resources->GetDevice()->CreateRenderTargetView(renderTargets.at(bufferIndex).Get(), nullptr, viewHeap.GetHandleCpu(bufferIndex));
				}
			
			}

				void WindowSurfaceImpl::CheckBufferQuery(HRESULT result)
				{
					if (FAILED(result))
					{
						throw Exception::CreationFailed{ "Could not get buffer for dx12 window surface" };
					}
				
				}

		
	}

	
}
