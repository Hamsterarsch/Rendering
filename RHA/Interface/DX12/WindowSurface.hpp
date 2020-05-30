#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "DxPtrTypes.hpp"


struct ID3D12Resource;
struct D3D12_CPU_DESCRIPTOR_HANDLE;


namespace RHA::DX12
{
	class CmdList;
	class WindowSurface
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(WindowSurface)


		public: virtual size_t GetWidth() const = 0;
		
		public: virtual size_t GetHeight() const = 0;

		public: virtual void EnableVerticalSync() = 0;

		public: virtual void DisableVerticalSync() = 0;
		
		public: virtual DxPtr<ID3D12Resource> GetResourceTemplate() = 0;
		
		public: virtual void Present() = 0;

		public: virtual void RecordPipelineBindings(CmdList &list, const D3D12_CPU_DESCRIPTOR_HANDLE *depthDescriptor) = 0;

		public: virtual void RecordPreparationForRendering(CmdList &list) = 0;

		public: virtual void RecordPreparationForPresenting(CmdList &list) = 0;


		public: virtual void GoFullscreen() = 0;

		public: virtual void ResizeToWindow() = 0;

		public: virtual void GoWindowed() = 0;
										
	};

	
}
