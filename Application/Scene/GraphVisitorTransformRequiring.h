#pragma once
#include "GraphVisitor.hpp"
#include "Types/Matrix.hpp"


namespace App::Scene
{		
	class GraphVisitorTransformRequiring : public GraphVisitor
	{		
		private: Math::Matrix nodeToWorld;


		
		using GraphVisitor::Visit;
		
		public: void Visit(GraphNode &node) final override;
				
	};

	
}
