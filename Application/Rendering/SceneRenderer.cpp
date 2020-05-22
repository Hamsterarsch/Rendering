#include "Rendering/SceneRenderer.hpp"
#include "Utility.hpp"


namespace App::Rendering
{
	SceneRenderer::SceneRenderer(RendererMediator &mediator, const Math::VectorUint2 &surfaceDimensions) :
		mediator{ &mediator },
		projection{ Math::Matrix::MakeProjection(Math::Radians(90), surfaceDimensions.x, surfaceDimensions.y, 100, 50'000) }
	{		
	}



	void SceneRenderer::SubmitFrame()
	{
	}

	void SceneRenderer::OnProjectionChanged()
	{
	}

	
}
