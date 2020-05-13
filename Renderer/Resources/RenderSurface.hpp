#pragma once
#include "DX12/WindowSurface.hpp"
#include "DX12/DepthSurface.hpp"


namespace RHA {
	namespace DX12 {
		class CmdList;
	}
}

namespace Renderer
{
	namespace DX12
	{
		class RenderSurface
		{
			private: RHA::DX12::WindowSurface *surfaceColor;

			private: RHA::DX12::DepthSurface *surfaceDepth;

			private: D3D12_VIEWPORT defaultViewport;

			private: D3D12_RECT defaultRect;

			private: bool depthSurfaceShouldBeClearedOnPrepare;


					 			
			public: RenderSurface();
			
			public: RenderSurface(RHA::DX12::WindowSurface *colorSurface, RHA::DX12::DepthSurface *depthSurface, size_t screenWidth, size_t screenHeight);

			public: void RecordSurfacePreparations(RHA::DX12::CmdList &list);

				private: bool ColorSurfaceIsVaild() const { return surfaceColor != nullptr; }

			public: void RecordClearDepthSurface(RHA::DX12::CmdList &list) const;

				private: bool DepthSurfaceIsValid() const { return surfaceDepth != nullptr; }

			public: void RecordBindSurfaces(RHA::DX12::CmdList &list) const;

				private: void RecordBindColorAndDepthSurface(RHA::DX12::CmdList &list) const;

				private: void RecordBindDepthSurface(RHA::DX12::CmdList &list) const;

			public: void RecordPresentPreparations(RHA::DX12::CmdList &list);

			public: void Present();

			public: void ShouldClearDepthSurface(bool value);
			
		};


	}
	
	
}
