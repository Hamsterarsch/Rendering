#pragma once
#include "Resources/HandleWrapper.hpp"


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

		private: Renderer::HandleWrapper uiSignature;
		
		private: Renderer::HandleWrapper uiPso;

		private: Renderer::HandleWrapper uiFontTexture;
		
		private: Renderer::HandleWrapper uiVertexIndexBuffer;
		
		
		public: UiRenderer(RendererMediator &mediator);

		public: void SubmitFrame();

		public: ~UiRenderer();
		
	};
	
	
}
