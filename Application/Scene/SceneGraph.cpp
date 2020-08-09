#include "Scene/SceneGraph.hpp"


namespace App::Scene
{
	SceneGraph::SceneGraph()
		:
		rootNode{ "root", {}, {}}
	{
	}


	
	void SceneGraph::SetCameraPos(const float x, const float y, const float z)
	{
		const auto dx{ lastOriginPos.x - x };
		const auto dy{ lastOriginPos.y - y };
		const auto dz{ lastOriginPos.z - z };
		const auto manhattanDist{ dx*dx+dy*dy+dz*dz };

		if(manhattanDist >= manhattanDistanceFromOriginPos)
		{
			CorrectScenePosition();
		}
				
	}


	
	void SceneGraph::Accept(GraphVisitor &visitor)
	{
		rootNode.Accept(visitor);

	}


	
	void SceneGraph::CorrectScenePosition()
	{
	}

	
}
