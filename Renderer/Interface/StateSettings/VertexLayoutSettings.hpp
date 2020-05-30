#pragma once
#include "StateSettings/StateSettings.hpp"
#include "StateSettings/FormatTargets.hpp"
#include "StateSettings/SemanticTargets.hpp"


namespace Renderer
{	
	class VertexLayoutSettings : public StateSettings
	{		
		public: virtual VertexLayoutSettings &AddLayoutElementDesc(t_semantic_target semantic, unsigned char semanticIndex, t_format_target format, unsigned byteOffset) = 0;
								
	};
	
	
}
