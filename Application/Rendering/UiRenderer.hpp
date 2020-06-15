#pragma once
#include "Resources/HandleWrapper.hpp"


namespace Renderer
{
	class SerializeTarget;
	class RendererFacade;

	namespace Commands
	{
		class CompositeCommand;
		class CommandFactory;
	}
}

struct ImGuiContext;
struct ImDrawData;
struct ImDrawList;


namespace App::Rendering
{
	class RendererMediator;
	using namespace Renderer;
	
	class UiRenderer
	{
		
		private: RendererMediator *mediator;
				 
		private: ImGuiContext *imguiContext;

		private: HandleWrapper uiSignature;
		
		private: HandleWrapper uiPso;

		private: HandleWrapper uiFontTexture;
		
		private: HandleWrapper uiVertexIndexBuffer;

		private: HandleWrapper uiConstantBuffer;

		private: HandleWrapper uiDescriptors;

		private: ImDrawData *submitDrawData;

		private: size_t vertexIndexBufferSizeInBytes;

		private: struct
		{
			float x{ 0 };
			float y{ 0 };
		} lastSubmitDisplayPos, lastSubmitDisplaySize;

		private: size_t constantBufferSizeInBytes;

		private: unsigned drawRecordVertexOffset;
		
		private: unsigned drawRecordIndexOffset;
		

		
		public: UiRenderer(RendererMediator &mediator, ::Renderer::RendererFacade &renderer);

			private: void CreateUiSignature(::Renderer::RendererFacade &renderer);
		
			private: void CreateUiFontTexture(::Renderer::RendererFacade &renderer);

			private: void CreateUiPipeline(::Renderer::RendererFacade &renderer);

				private: static SerializeTarget CreateUiVertexShader(::Renderer::RendererFacade &renderer);

				private: static SerializeTarget CreateUiPixelShader(::Renderer::RendererFacade &renderer);
						 		

		public: ~UiRenderer();

		public: bool IsValid() const;

			private: void Free();
		
		public: UiRenderer(UiRenderer &&other) noexcept;

		public: UiRenderer &operator=(UiRenderer &&other) noexcept;

				private: void Invalidate();

		
		public: void SubmitFrame();

			private: bool ScreenIsMinimized() const;

			private: bool DrawDataIsEmpty() const;

			private: void CreateBufferWithUiVertexIndexData();

			private: bool TryToUpdateUiConstantBuffer();

				private: bool ConstantBufferValuesHaveNotChanged();

			private: void CreateDescriptorBlockForUiResources();

			private: void SubmitUiRendererWork();

			private: void RecordDrawDataDrawLists(Commands::CompositeCommand &targetCommand, Commands::CommandFactory &cmdFactory);

				private: void RecordDrawListDrawCommands(const ImDrawList &drawList, Commands::CompositeCommand &targetCommand, Commands::CommandFactory &cmdFactory);
		
	};
	
	
}
