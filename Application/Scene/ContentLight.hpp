#pragma once
#include "GraphNodeContent.hpp"
#include "Rendering/ShaderRelevantTypes/Light.hpp"


namespace App::Scene
{
	class ContentLight final : public GraphNodeContent
	{
		private: Rendering::Light lightData;

		
				
		public: ContentLight(float radius, const Math::Vector3 &color, float innerBlendRadians, float outerBlendRadians);

		
		public: void Accept(GraphVisitor &visitor) override;

		public: UniquePtr<Ui::Core::UiFrontend> MakeContentEditor(Ui::Core::UiLayoutElement &layoutParent) override;

		public: UniquePtr<Ui::Core::UiFrontend> MakeTransformEditor(Math::Matrix &transformToEdit, Ui::Core::UiLayoutElement &layoutParent) override { return {}; }
		
		public: Rendering::Light GetLightData(const Math::Matrix &forLightTransform) const;
		
	};

	
}
