#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace App::Scene
{
	class GraphVisitor;
	
	class GraphNodeContent
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(GraphNodeContent)
				
		public: virtual void Accept(GraphVisitor &visitor) = 0;
		
	};
	
	
}