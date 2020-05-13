#include "Resources/RenderSurface.hpp"
#include "RHA/Interface/DX12/CmdList.hpp"


namespace Renderer::DX12
{
	RenderSurface::RenderSurface()
		:
		surfaceColor{ nullptr },
		surfaceDepth{ nullptr },
		defaultViewport{},
		defaultRect{},
		depthSurfaceShouldBeClearedOnPrepare{ false }
	{
	}


	
	RenderSurface::RenderSurface(RHA::DX12::WindowSurface *const colorSurface, RHA::DX12::DepthSurface *const depthSurface, const size_t screenWidth, const size_t screenHeight)
		:
		surfaceColor{ colorSurface },
		surfaceDepth{ depthSurface },
		defaultViewport{},
		defaultRect{},
		depthSurfaceShouldBeClearedOnPrepare{ true }
	{
		if(!ColorSurfaceIsVaild())
		{
			defaultViewport.Width = screenWidth;
			defaultViewport.Height = screenHeight;
			defaultViewport.MinDepth = 0;
			defaultViewport.MaxDepth = 1;
			defaultViewport.TopLeftX = defaultViewport.TopLeftX = 0;
						
			defaultRect.left = defaultRect.top = 0;
			defaultRect.bottom = screenHeight;
			defaultRect.right = screenWidth;				
		}
		
	}

	

	void RenderSurface::RecordSurfacePreparations(RHA::DX12::CmdList &list)
	{
		if(depthSurfaceShouldBeClearedOnPrepare)
		{
			RecordClearDepthSurface(list);				
		}

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

				list.RecordSetViewports(&defaultViewport, 1);
				list.RecordSetScissorRects(&defaultRect, 1);
							
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


	
	void RenderSurface::ShouldClearDepthSurface(const bool value)
	{
		depthSurfaceShouldBeClearedOnPrepare = value;
		
	}

	
}
