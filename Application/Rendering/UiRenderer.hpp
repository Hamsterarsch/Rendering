#pragma once


namespace ImGui
{
	class ImGuiContext;
}

namespace App::Rendering
{
	class RendererMediator;
	
	class UiRenderer
	{
		private: RendererMediator *mediator;
				 
		private: ImGui::ImGuiContext *imguiContext;
		
		
		
		public: UiRenderer(RendererMediator &mediator, void *windowHandle);

		public: void SubmitFrame();

		public: ~UiRenderer();
		
	};
	
	
}
