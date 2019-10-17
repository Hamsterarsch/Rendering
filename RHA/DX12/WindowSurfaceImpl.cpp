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
			cmdAllocator{ resources, D3D12_COMMAND_LIST_TYPE_DIRECT },
			currentBackbufferIndex{ 0 }
		{
			CreateSwapChain(resources, queue, window);

			CreateViewsForChainBuffers(resources);
					   			
			PopulateBufferData(resources);
			
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
				for (size_t bufferIndex{ 0 }; bufferIndex < bufferData.size(); ++bufferIndex)
				{
					const auto result
					{
						swapChain->GetBuffer(bufferIndex, IID_PPV_ARGS(&bufferData.at(bufferIndex).resource))
					};
					CheckBufferQuery(result);

					resources->GetDevice()->CreateRenderTargetView(bufferData.at(bufferIndex).resource.Get(), nullptr, viewHeap.GetHandleCpu(bufferIndex));
				}
			
			}

				void WindowSurfaceImpl::CheckBufferQuery(HRESULT result)
				{
					if (FAILED(result))
					{
						throw Exception::CreationFailed{ "Could not get buffer for dx12 window surface" };
					}
				
				}

			void WindowSurfaceImpl::PopulateBufferData(DeviceResources* resources)
			{
				for (unsigned bufferIndex{ 0 }; bufferIndex < bufferCount; ++bufferIndex)
				{
					CreateClearCommandForBuffer(bufferIndex);
					CreatePresentCommandForBuffer(bufferIndex);
					CreateEventsForBuffer(bufferIndex);
					CreateFencesForBuffer(bufferIndex, resources);
					AssignEventsToFencesForBuffer(bufferIndex);
				}

			}

				void WindowSurfaceImpl::CreateClearCommandForBuffer(unsigned bufferIndex)
				{
					bufferData.at(bufferIndex).clearCmd = cmdAllocator.AllocateList();
					auto graphicsList{ bufferData.at(bufferIndex).clearCmd->AsGraphicsList() };

					D3D12_RESOURCE_BARRIER toOutputTarget{};
					toOutputTarget.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
					toOutputTarget.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
					toOutputTarget.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
					toOutputTarget.Transition.pResource = bufferData.at(bufferIndex).resource.Get();

					graphicsList->ResourceBarrier(1, &toOutputTarget);
					graphicsList->ClearRenderTargetView(viewHeap.GetHandleCpu(bufferIndex), clearColor, 0, nullptr);
					graphicsList->Close();

				}

				void WindowSurfaceImpl::CreatePresentCommandForBuffer(unsigned bufferIndex)
				{
					bufferData.at(bufferIndex).presentCmd = cmdAllocator.AllocateList();
					auto graphicsList{ bufferData.at(bufferIndex).presentCmd->AsGraphicsList() };

					D3D12_RESOURCE_BARRIER toPresentable{};
					toPresentable.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
					toPresentable.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
					toPresentable.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
					toPresentable.Transition.pResource = bufferData.at(bufferIndex).resource.Get();

					graphicsList->ResourceBarrier(1, &toPresentable);
					graphicsList->Close();

				}

				void WindowSurfaceImpl::CreateEventsForBuffer(unsigned bufferIndex)
				{
					constexpr decltype(nullptr) DEFAULT_SECURITY{ nullptr };
			
					bufferData.at(bufferIndex).clearEvent = CreateEvent(DEFAULT_SECURITY, false, true, nullptr);
					bufferData.at(bufferIndex).presentEvent = CreateEvent(DEFAULT_SECURITY, false, false, nullptr);

				}

				void WindowSurfaceImpl::CreateFencesForBuffer(unsigned bufferIndex, DeviceResources *resources)
				{
					bufferData.at(bufferIndex).clearFence = RHA::DX12::Facade::MakeFence(resources);
					bufferData.at(bufferIndex).presentFence = RHA::DX12::Facade::MakeFence(resources);

				}

				void WindowSurfaceImpl::AssignEventsToFencesForBuffer(unsigned bufferIndex)
				{
					bufferData.at(bufferIndex).clearFence->GetFence()->SetEventOnCompletion(1, bufferData.at(bufferIndex).clearEvent);
					bufferData.at(bufferIndex).presentFence->GetFence()->SetEventOnCompletion(1, bufferData.at(bufferIndex).presentEvent);

				}
		
		
		void WindowSurfaceImpl::ScheduleBackbufferClear(Queue *queue)
		{			
			WaitForSingleObject(GetBackbufferData().clearEvent, INFINITE);
			
			queue->SubmitCommandList(GetBackbufferData().clearCmd.get());
			GetBackbufferData().clearFence->Signal(1, queue);
			
		}

			BufferData &WindowSurfaceImpl::GetBackbufferData()
			{
				return bufferData.at(currentBackbufferIndex);
			}

		void WindowSurfaceImpl::SchedulePresentation(Queue *queue)
		{
			queue->SubmitCommandList(GetBackbufferData().presentCmd.get());
			GetBackbufferData().presentFence->Signal(1, queue);

			WaitForSingleObject(GetBackbufferData().presentEvent, INFINITE);
			
			currentBackbufferIndex = (currentBackbufferIndex + 1) % bufferCount;
			swapChain->Present(0, 0);
			
		}
		
		
	}

	
}
