#include "Scene/ContentLight.hpp"
#include "Scene/GraphVisitor.hpp"


namespace App::Scene
{
	ContentLight::ContentLight(const float radius, const Math::Vector3 &color, const float innerBlendRadians, const float outerBlendRadians)
	{
		lightData.worldForwardVector = {0,0,1};
		lightData.color = color;
		lightData.radius = radius;
		lightData.innerBlendRadians = innerBlendRadians;
		lightData.outerBlendRadians = outerBlendRadians;
				
	}


	
	void ContentLight::Accept(GraphVisitor &visitor)
	{
		visitor.Visit(*this);
		
	}


	
	Rendering::Light ContentLight::GetLightData(const Math::Matrix &forLightTransform) const
	{
		auto outLight{ lightData };

		outLight.worldPos = forLightTransform.Transform(outLight.worldPos);
				
		auto noTranslationTransform{ forLightTransform };
		noTranslationTransform.SetTranslation({0,0,0});
		
		outLight.worldForwardVector = noTranslationTransform.Transform(outLight.worldForwardVector);

		return outLight;
		
	}

	
}
