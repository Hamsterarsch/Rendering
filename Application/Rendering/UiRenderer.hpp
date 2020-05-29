#pragma once
#include "Resources/HandleWrapper.hpp"


namespace Renderer { class Renderer; }
struct ImGuiContext;


namespace App::Rendering
{
	class RendererMediator;
	
	class UiRenderer
	{
		private: RendererMediator *mediator;
				 
		private: ImGuiContext *imguiContext;

		private: Renderer::HandleWrapper uiSignature;
		
		private: Renderer::HandleWrapper uiPso;

		private: Renderer::HandleWrapper uiFontTexture;
		
		private: Renderer::HandleWrapper uiVertexIndexBuffer;

		private: Renderer::HandleWrapper uiConstantBuffer;

		private: Renderer::HandleWrapper uiDescriptors;
		
		
		public: UiRenderer(RendererMediator &mediator, ::Renderer::Renderer *renderer);

		public: ~UiRenderer();

		public: bool IsInvalid() const;
		
		public: UiRenderer(UiRenderer &&other) noexcept;

		public: UiRenderer &operator=(UiRenderer &&other) noexcept;

				private: void Invalidate();

		
		public: void SubmitFrame();
		
	};
	
	
}
