#pragma once
#include "Math/Interface/Types/Matrix.hpp"
#include "Resources/HandleWrapper.hpp"


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

		private: PipelineData createVolumeTileGridPipeline;
		
		
		public: SceneRenderer
		(
			RendererMediator &mediator,
			Renderer::RendererFacade &renderer,
			assetSystem::AssetSystem &internalShaderProvider, 
			const Math::VectorUint2 &surfaceDimensions
		);

			private: static PipelineData MakePsoVolumeTileGridCreationPipeline(assetSystem::AssetSystem &shaderProvider, Renderer::RendererFacade &renderer);
		
		
		public: void SubmitFrame();
		
		public: void SetCamera(const Math::Vector3 &position, const Math::Vector3 &rotation);
		
		public: void OnProjectionChanged();
		
		
	};


	
		
}
