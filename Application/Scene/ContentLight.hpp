#pragma once
#include "GraphNodeContent.hpp"
#include "Rendering/ShaderRelevantTypes/Light.hpp"


namespace App::Scene
{
	class ContentLight final : public GraphNodeContent
	{
		private: Rendering::Light lightData;

		
		
		public: void Accept(GraphVisitor &visitor) override;

		public: Rendering::Light GetLightData() const { return lightData; }
		
	};

	
}
