#pragma once
#include "Math/Interface/Types/Matrix.hpp"
#include "Resources/HandleWrapper.hpp"
#include "Shared/PtrTypes.hpp"

//use frwd decl
#include "Commands/CommandFactory.hpp"


namespace assetSystem { class AssetSystem; }
namespace Renderer{ class RendererFacade; }


namespace App::Rendering
{
	class RendererMediator;

	struct PipelineData
	{
		Renderer::HandleWrapper signature;
		Renderer::HandleWrapper pso;
	};
		
	class SceneRenderer
	{
		private: RendererMediator *mediator;
		
		private: Math::Matrix projection;

		private: Math::Matrix view;

		private: PipelineData pipelineCreateVolumeTileGrid;

		private: PipelineData pipelineMarkActiveVolumeTiles;

		private: PipelineData pipelineBuildActiveVolumeTileList;

		private: PipelineData pipelineAssignLightsToTiles;

		private: UniquePtr<Renderer::Commands::CommandFactory> commandFactory;

		private: Renderer::HandleWrapper gridDataBuffer;

		private: Renderer::HandleWrapper gridBoundingBoxBuffer;
				 		
				
		public: SceneRenderer
		(
			RendererMediator &mediator,
			Renderer::RendererFacade &renderer,
			assetSystem::AssetSystem &internalShaderProvider, 
			const Math::VectorUint2 &surfaceDimensions
		);

			private: static PipelineData MakeVolumeTileGridCreationPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer);

			private: static PipelineData MakeMarkActiveVolumeTilesPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer);

			private: static PipelineData MakeBuildActiveVolumeTileListPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer);

			private: static PipelineData MakeAssignLightsToTilesPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer);
		
		
		public: void SubmitFrame();
		
		public: void SetCamera(const Math::Vector3 &position, const Math::Vector3 &rotation);
		
		public: void OnProjectionChanged();
		
		
	};


	
		
}
