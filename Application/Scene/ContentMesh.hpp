#pragma once
#include "GraphNodeContent.hpp"


namespace App::Scene
{
	class ContentMesh final : public GraphNodeContent
	{
		public: void Accept(GraphVisitor &visitor) override;
		
	};
	
	
}
