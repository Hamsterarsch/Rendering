#pragma once
#include "Ui/Core/UiLayoutElement.hpp"


namespace App::Ui
{
	class FloatLayout final : public Core::UiLayoutElement
	{
		public: float itemPadding;

		public: bool isVertical;


		public: FloatLayout(float itemPadding, bool isVertical = false) : itemPadding{ itemPadding }, isVertical{ isVertical } {}
		
		protected: void OnPreRenderAndQueryChild(Core::UiBuilder& builder, size_t childIndex, UiElement& child) override;
		
	};

	
}
