#include "Scene/ContentLight.hpp"
#include "Scene/GraphVisitor.hpp"


namespace App::Scene
{
	void ContentLight::Accept(GraphVisitor &visitor)
	{
		visitor.Visit(*this);
		
	}

	
}
