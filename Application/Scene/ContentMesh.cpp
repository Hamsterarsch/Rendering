#include "Scene/ContentMesh.hpp"
#include "Scene/GraphVisitor.hpp"


namespace App::Scene
{
	void ContentMesh::Accept(GraphVisitor &visitor)
	{
		visitor.Visit(*this);		
		
	}

	
}
