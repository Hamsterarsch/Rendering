#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace App::Scene
{
	class GraphNode;
	class ContentMesh;
	class ContentLight;

	
	class GraphVisitor
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(GraphVisitor)

		
		public: virtual void Visit(GraphNode &node) = 0;
		
		public: virtual void Visit(ContentMesh &mesh) = 0;

		public: virtual void Visit(ContentLight &light) = 0;
		
	};

	
}
