#pragma once
#include "StateSettings/StateSettings.hpp"
#include "StateSettings/FormatTargets.hpp"
#include "StateSettings/SemanticTargets.hpp"


namespace Renderer
{	
	class VertexLayoutSettings : public StateSettings
	{		
		public: virtual VertexLayoutSettings &AddLayoutElementDesc(Semantic semantic, unsigned char semanticIndex, Format format, unsigned byteOffsetFromVertexStart) = 0;
								
	};
	
	
}
