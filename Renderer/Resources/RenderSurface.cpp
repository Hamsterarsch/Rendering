#include "Resources/RenderSurface.hpp"
#include "RHA/Interface/DX12/CmdList.hpp"


namespace Renderer
{
	namespace DX12
	{
		RenderSurface::RenderSurface(RHA::DX12::WindowSurface *const colorSurface, RHA::DX12::DepthSurface *const depthSurface)
			:
			surfaceColor{ colorSurface },
			surfaceDepth{ depthSurface }
		{
		}

		

		void RenderSurface::RecordSurfacePreparations(RHA::DX12::CmdList &list)
		{
			RecordClearDepthSurface(list);

			if(ColorSurfaceIsVaild())
			{
				surfaceColor->RecordPreparationForRendering(list);				
			}
			RecordBindSurfaces(list);
			
		}
		
			void RenderSurface::RecordClearDepthSurface(RHA::DX12::CmdList &list) const
			{
				if(DepthSurfaceIsValid())
				{					
					list.RecordClearDsv(surfaceDepth->GetHandleCpu(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);					
				}
			
			}
			
			void RenderSurface::RecordBindSurfaces(RHA::DX12::CmdList &list) const
			{
				if(ColorSurfaceIsVaild())
				{
					RecordBindColorAndDepthSurface(list);	
				}
				else if(DepthSurfaceIsValid())
				{
					RecordBindDepthSurface(list);
				}
			
			}

				void RenderSurface::RecordBindColorAndDepthSurface(RHA::DX12::CmdList &list) const
				{
					D3D12_CPU_DESCRIPTOR_HANDLE dsv{ DepthSurfaceIsValid() ? surfaceDepth->GetHandleCpu() : D3D12_CPU_DESCRIPTOR_HANDLE{} };
					surfaceColor->RecordPipelineBindings
					(
						list,
						&dsv
					);
			
				}

				void RenderSurface::RecordBindDepthSurface(RHA::DX12::CmdList &list) const
				{
					auto dsv{ surfaceDepth->GetHandleCpu() };
					list.RecordSetRenderTargets(0, nullptr, false, &dsv);					
								
				}
		
		

		void RenderSurface::RecordPresentPreparations(RHA::DX12::CmdList &list)
		{
			if(ColorSurfaceIsVaild())
			{
				surfaceColor->RecordPreparationForPresenting(list);					
			}
			
		}

		

		void RenderSurface::Present()
		{
			if(ColorSurfaceIsVaild())
			{
				surfaceColor->Present();				
			}
			
		}

		
	}

	
}