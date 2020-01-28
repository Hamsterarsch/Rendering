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


			public: virtual DxPtr<ID3D12Resource> GetResourceTemplate() = 0;
			
			public: virtual void ScheduleBackbufferClear(Queue *queue) = 0;

			public: virtual void ScheduleCopyToBackbuffer(Queue *queue, CmdList *targetList, ID3D12Resource *source) = 0;
			
			public: virtual void SchedulePresentation(Queue *queue) = 0;

			public: virtual void RecordPipelineBindings(ID3D12GraphicsCommandList *list, const D3D12_CPU_DESCRIPTOR_HANDLE *depthDescriptor) = 0;

			public: virtual void RecordPreparationForRendering(ID3D12GraphicsCommandList *list) = 0;
							
							
		};

		
	}

	
}
