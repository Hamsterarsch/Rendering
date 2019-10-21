#pragma once
#include <Windows.h>
#include <condition_variable>
#include <future>
#include "RendererExportHelper.hpp"
#include "Shared/PtrTypes.hpp"


#include "DxPtrTypes.hpp"


namespace RHA
{
	namespace DX12
	{
		class DeviceResources;
		class Queue;
		class CmdAllocator;
		class CmdList;
		class WindowSurface;
		class Fence;
	}
}

struct ID3D12RootSignature;
struct ID3D12PipelineState;

namespace Renderer
{

	
	class RENDERER_DLLSPEC Renderer
	{
		private: const unsigned inflightFramesAmount;
		
		private: std::mutex updaterMutex;

		private: std::condition_variable updaterCondition;

		private: bool shouldUpdateRendering;
		
		private: std::future<int> updaterHandle;

		private: UniquePtr<RHA::DX12::DeviceResources> resources;

		private: UniquePtr<RHA::DX12::Queue> commonQueue;

		private: UniquePtr<RHA::DX12::CmdAllocator> commonAllocator;
		
		private: UniquePtr<RHA::DX12::WindowSurface> outputSurface;

				 DxPtr<ID3D12RootSignature> signature;
				 DxPtr<ID3D12PipelineState> pipeline;
				 		
		
		public: Renderer(HWND outputWindow);

			private: int UpdateRendering();

		public: ~Renderer();

					
		public: void SubmitFrameInfo();

		public: void WaitForCapacity();

		
		
	};
	
}
