#pragma once
#include "Math/Interface/Types/Matrix.hpp"


namespace assetSystem { class AssetSystem; }
namespace Renderer{ class RendererFacade; }


namespace App::Rendering
{
	class RendererMediator;
	
	class SceneRenderer
	{
		private: RendererMediator *mediator;
		
		private: Math::Matrix projection;

		private: Math::Matrix view;

		
		
		public: SceneRenderer
		(
			RendererMediator &mediator,
			Renderer::RendererFacade &renderer,
			assetSystem::AssetSystem &internalShaderProvider, 
			const Math::VectorUint2 &surfaceDimensions
		);

		public: void SubmitFrame();
		
		public: void SetCamera(const Math::Vector3 &position, const Math::Vector3 &rotation);
		
		public: void OnProjectionChanged();
		
		
	};


	
		
}
