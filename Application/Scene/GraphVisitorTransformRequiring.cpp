#include "Scene/GraphVisitorTransformRequiring.h"
#include "Scene/GraphNode.hpp"


namespace App::Scene
{
	void GraphVisitorTransformRequiring::Visit(GraphNode &node)
	{
		nodeToWorld *= node.GetNodeTransform();

		node.ContentAccept(*this);
		node.ChildrenAccept(*this);

		nodeToWorld *= node.GetNodeTransform().Inverse();
		
	}
	
	
}
