#pragma once
#include "Shared/InterfaceHelpers.hpp"


struct ID3D12GraphicsCommandList;


namespace RHA
{
	namespace DX12
	{
		class WindowSurface
		{
			DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(WindowSurface)


			public: virtual inline float GetWidth() const = 0;
			
			public: virtual inline float GetHeight() const = 0;

			public: virtual inline void EnableVerticalSync() = 0;

			public: virtual inline void DisableVerticalSync() = 0;
			
			public: virtual DxPtr<ID3D12Resource> GetResourceTemplate() = 0;
			
			public: virtual void Present() = 0;

			public: virtual void RecordPipelineBindings(ID3D12GraphicsCommandList *list, const D3D12_CPU_DESCRIPTOR_HANDLE *depthDescriptor) = 0;

			public: virtual void RecordPreparationForRendering(ID3D12GraphicsCommandList *list) = 0;

			public: virtual void RecordPreparationForPresenting(ID3D12GraphicsCommandList *list) = 0;
							
		};

		
	}

	
}
