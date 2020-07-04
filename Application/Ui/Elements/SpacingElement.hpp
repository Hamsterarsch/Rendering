#pragma once
#include "Ui/Core/UiElement.hpp"


namespace App::Ui
{	
	class SpacingElement final : public Core::UiElement
	{
		public: SpacingElement(const Math::Vector2 &size) { this->size = size; }

						
		protected: void RenderAndQueryInternal(Core::UiBuilder& builder) override {}
		
	};

	
}
