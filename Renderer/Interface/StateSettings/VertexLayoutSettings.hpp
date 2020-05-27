#pragma once
#include "StateSettings/StateSettings.hpp"
#include "StateSettings/FormatTargets.hpp"
#include "StateSettings/SemanticTargets.hpp"


namespace Renderer
{	
	class VertexLayoutSettings : public StateSettings
	{
		public: using t_target_semantic = const void *(SemanticTargets:: *)() const;

		public: using t_target_format = const void *(FormatTargets:: *)() const;

		
		public: virtual void AddLayoutElementDesc(t_target_semantic semantic, unsigned char semanticIndex, t_target_format format, unsigned byteOffset) = 0;
								
	};
	
	
}
