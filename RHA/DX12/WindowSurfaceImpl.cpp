#include "DX12/DeviceResources.hpp"
#include "DX12/Queue.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "DX12/WindowSurfaceImpl.hpp"
#include <string>
#include "DX12/Facade.hpp"


namespace RHA
{
	namespace DX12
	{
		WindowSurfaceImpl::WindowSurfaceImpl(DeviceResources *resources, Queue *queue, HWND window) :
			viewHeap{ resources, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, bufferCount, false },			
			currentBackbufferIndex{ 0 },
			targetedVerticalBlank{ 0 }
		{
			CreateSwapChain(resources, queue, window);

			CreateViewsForChainBuffers(resources);
					   						
		}

			void WindowSurfaceImpl::CreateSwapChain(DeviceResources *resources, Queue *queue, HWND window)
			{
				DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc{};
				fsDesc.Windowed = false;				
								
				constexpr unsigned AUTO_OBTAIN{ 0 };
				constexpr unsigned NO_MSAA{ 1 };

				DXGI_SWAP_CHAIN_DESC1 swapDesc{};
				swapDesc.Width = AUTO_OBTAIN;
				swapDesc.Height = AUTO_OBTAIN;
				swapDesc.Stereo = false;
				swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
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

				InitializeFullscreenState();			
				QueryViewportInformation();
				
			}

				void WindowSurfaceImpl::CheckSwapChainCreation(HRESULT result)
				{
					if (FAILED(result))
					{
						throw Exception::CreationFailed{ "Could not create dx12 window surface for windows window" };
					}
				
				}

				void WindowSurfaceImpl::InitializeFullscreenState()
				{
					swapChain->SetFullscreenState(true, nullptr);
					swapChain->ResizeBuffers(0, 0, 0,DXGI_FORMAT_UNKNOWN, 0);
			
				}

				void WindowSurfaceImpl::QueryViewportInformation()
				{
					DXGI_SWAP_CHAIN_DESC1 obtainedDesc{};
					swapChain->GetDesc1(&obtainedDesc);

					defaultViewport.Width = obtainedDesc.Width;
					defaultViewport.Height = obtainedDesc.Height;
					defaultViewport.MinDepth = 0;
					defaultViewport.MaxDepth = 1;
					defaultViewport.TopLeftX = defaultViewport.TopLeftX = 0;

					defaultRect.left = defaultRect.top = 0;
					defaultRect.bottom = obtainedDesc.Height;
					defaultRect.right = obtainedDesc.Width;
				
				}

		void WindowSurfaceImpl::CreateViewsForChainBuffers(DeviceResources *resources)
			{
				for (size_t bufferIndex{ 0 }; bufferIndex < buffers.size(); ++bufferIndex)
				{
					const auto result
					{
						swapChain->GetBuffer(bufferIndex, IID_PPV_ARGS(&buffers.at(bufferIndex)))
					};
					CheckBufferQuery(result);

					resources->GetDevice()->CreateRenderTargetView(buffers.at(bufferIndex).Get(), nullptr, viewHeap.GetHandleCpu(bufferIndex));
				}
			
			}

				void WindowSurfaceImpl::CheckBufferQuery(HRESULT result)
				{
					if (FAILED(result))
					{
						throw Exception::CreationFailed{ "Could not get buffer for dx12 window surface" };
					}
				
				}

						

		WindowSurfaceImpl::~WindowSurfaceImpl()
		{
			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc;			
			swapChain->GetFullscreenDesc(&fsDesc);

			if(!fsDesc.Windowed)
			{
				swapChain->SetFullscreenState(false, nullptr);
			}
			
		}

		
		
		void WindowSurfaceImpl::Present()
		{
			currentBackbufferIndex = (currentBackbufferIndex + 1) % bufferCount;

			static constexpr DXGI_PRESENT_PARAMETERS params{ 0, nullptr, nullptr, nullptr };
			swapChain->Present1(targetedVerticalBlank, 0, &params);
			
		}

		
		
		void WindowSurfaceImpl::RecordPipelineBindings(ID3D12GraphicsCommandList *list, const D3D12_CPU_DESCRIPTOR_HANDLE *depthDescriptor)
		{
			auto rtv{ viewHeap.GetHandleCpu(currentBackbufferIndex) };
			list->OMSetRenderTargets(1, &rtv, false, depthDescriptor);
			
			list->RSSetViewports(1, &defaultViewport);
			list->RSSetScissorRects(1, &defaultRect);
			
		}


				
		void WindowSurfaceImpl::RecordPreparationForRendering(ID3D12GraphicsCommandList *list)
		{
			D3D12_RESOURCE_BARRIER toOutputTarget{};
			toOutputTarget.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			toOutputTarget.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
			toOutputTarget.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			toOutputTarget.Transition.pResource = GetBackbuffer();
			list->ResourceBarrier(1, &toOutputTarget);

			auto rtv{ viewHeap.GetHandleCpu(currentBackbufferIndex) };
			list->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
			
		}
			
			ID3D12Resource *WindowSurfaceImpl::GetBackbuffer()
			{
				return buffers.at(currentBackbufferIndex).Get();
			
			}

		
		
		void WindowSurfaceImpl::RecordPreparationForPresenting(ID3D12GraphicsCommandList *list)
		{
			D3D12_RESOURCE_BARRIER toPresentable{};
			toPresentable.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			toPresentable.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			toPresentable.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
			toPresentable.Transition.pResource = GetBackbuffer();

			list->ResourceBarrier(1, &toPresentable);
			
		}

		
	}

	
}
