#include "DX12/DeviceResources.hpp"
#include "DX12/Queue.hpp"
#include "Shared/Exception/CreationFailedException.hpp"
#include "DX12/WindowSurfaceImpl.hpp"
#include <string>
#include "DX12/Facade.hpp"


namespace RHA::DX12
{

	WindowSurfaceImpl::WindowSurfaceImpl(DeviceResources *resources, Queue *queue, HWND window) :
		resources{ resources },
		viewHeap{ resources, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, bufferCount, false },			
		currentBackbufferIndex{ 0 },
		targetedVerticalBlank{ 0 }
	{
		CreateSwapChain(resources, queue, window);

		CreateViewsForChainBuffers();
				   						
	}

		void WindowSurfaceImpl::CreateSwapChain(DeviceResources *resources, Queue *queue, HWND window)
		{
			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc{};
			fsDesc.Windowed = true;				
						
			constexpr unsigned FIT_WINDOW{ 0 };
			constexpr unsigned NO_MSAA{ 1 };

			DXGI_SWAP_CHAIN_DESC1 swapDesc{};
			swapDesc.Width = FIT_WINDOW;
			swapDesc.Height = FIT_WINDOW;
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

		void WindowSurfaceImpl::CreateViewsForChainBuffers()
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
			currentBackbufferIndex = 0;
		
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
		if(IsValid())
		{
			Free();
		}
		
	}

		bool WindowSurfaceImpl::IsValid() const
		{
			return swapChain != nullptr;
		
		}

			void WindowSurfaceImpl::Free()
			{
				DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc;			
				swapChain->GetFullscreenDesc(&fsDesc);

				if(!fsDesc.Windowed)
				{
					swapChain->SetFullscreenState(false, nullptr);
					
				}
		
			}


	
	WindowSurfaceImpl::WindowSurfaceImpl(WindowSurfaceImpl &&other) noexcept
	{
		*this = std::move(other);
		
	}

		WindowSurfaceImpl &WindowSurfaceImpl::operator=(WindowSurfaceImpl &&rhs) noexcept
		{
			if(this == &rhs)
			{
				return *this;
				
			}
					   		
			if(this->IsValid())
			{
				this->Free();
			}

			WindowSurface::operator=(std::move(rhs));		
			swapChain				 = std::move(rhs.swapChain				);
			buffers					 = std::move(rhs.buffers				); 
			viewHeap 				 = std::move(rhs.viewHeap 				);
			currentBackbufferIndex	 = std::move(rhs.currentBackbufferIndex	);
			defaultViewport			 = std::move(rhs.defaultViewport		); 
			defaultRect				 = std::move(rhs.defaultRect			); 
			targetedVerticalBlank	 = std::move(rhs.targetedVerticalBlank	);

			rhs.Invalidate();

			return *this;
		
		}

			void WindowSurfaceImpl::Invalidate()
			{
				swapChain = nullptr;
		
			}



	void WindowSurfaceImpl::Present()
	{
		currentBackbufferIndex = (currentBackbufferIndex + 1) % bufferCount;

		static constexpr DXGI_PRESENT_PARAMETERS params{ 0, nullptr, nullptr, nullptr };
		swapChain->Present1(targetedVerticalBlank, 0, &params);
		
	}

	
	
	void WindowSurfaceImpl::RecordPipelineBindings(CmdList &list, const D3D12_CPU_DESCRIPTOR_HANDLE *depthDescriptor)
	{
		auto rtv{ viewHeap.GetHandleCpu(currentBackbufferIndex) };
		list.RecordSetRenderTargets(1, &rtv, false, depthDescriptor);

		list.RecordSetViewports(&defaultViewport, 1);
		list.RecordSetScissorRects(&defaultRect, 1);
					
	}


			
	void WindowSurfaceImpl::RecordPreparationForRendering(CmdList &list)
	{
		list.RecordBarrierTransition(GetBackbuffer(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
		list.RecordClearRtv(viewHeap.GetHandleCpu(currentBackbufferIndex), clearColor);
		
	}
		
		ID3D12Resource *WindowSurfaceImpl::GetBackbuffer()
		{
			return buffers.at(currentBackbufferIndex).Get();
		
		}

	
	
	void WindowSurfaceImpl::RecordPreparationForPresenting(CmdList &list)
	{			
		list.RecordBarrierTransition(GetBackbuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON);			
		
	}
	


	void WindowSurfaceImpl::GoFullscreen()
	{
		ReleaseSwapchainBuffers();
		
		swapChain->SetFullscreenState(true, nullptr);
		ResizeToWindow();

		CreateViewsForChainBuffers();
		QueryViewportInformation();
				
	}

		void WindowSurfaceImpl::ReleaseSwapchainBuffers()
		{
			for(auto &&buffer : buffers)
			{
				buffer.Reset();
			}
		
		}

		void WindowSurfaceImpl::ResizeToWindow()
		{
			ReleaseSwapchainBuffers();
		
			swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			CreateViewsForChainBuffers();
			QueryViewportInformation();
		
		}


	
	void WindowSurfaceImpl::GoWindowed()
	{
		ReleaseSwapchainBuffers();
		
		swapChain->SetFullscreenState(false, nullptr);
		ResizeToWindow();

		CreateViewsForChainBuffers();
		QueryViewportInformation();
		
	}

	
}
