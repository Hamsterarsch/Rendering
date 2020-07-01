#pragma once
#include "Ui/Core/UiLayoutElementBasic.hpp"


namespace App::Ui
{
	class FloatLayout final : public Core::UiLayoutElementBasic
	{
		private: bool invertDirection;
		
		public: float itemPadding;

		public: bool isVertical;



		public: FloatLayout(float itemPadding, bool isVertical = false) : FloatLayout{itemPadding, false, isVertical} {}

		public: FloatLayout(float itemPadding, bool invertDirection, bool isVertical) : invertDirection{ invertDirection }, itemPadding{ itemPadding }, isVertical{ isVertical } {}

		
		protected: void OnChildAdded(UiElement &child) override;
		
		protected: void OnPreRenderAndQueryChild(Core::UiBuilder& builder, size_t childIndex, UiElement& child) override;

	};

	
}
