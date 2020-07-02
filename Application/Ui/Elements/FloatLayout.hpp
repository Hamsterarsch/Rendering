#pragma once
#include "Ui/Core/UiLayoutElementBasic.hpp"


namespace App::Ui
{
	class FloatLayout final : public Core::UiLayoutElement
	{
		private: std::vector<UniquePtr<UiElement>> children;
		
		private: bool invertDirection;
		
		public: float itemPadding;

		public: bool isVertical;
				


		public: FloatLayout(float itemPadding, bool isVertical = false) : FloatLayout{itemPadding, false, isVertical} {}

		public: FloatLayout(float itemPadding, bool invertDirection, bool isVertical) : invertDirection{ invertDirection }, itemPadding{ itemPadding }, isVertical{ isVertical } {}

		
		public: void ClearChildren() override { children.clear(); }
		
		public:	void AddChild(UniquePtr<UiElement>&& child) override;

			protected: void OnChildAdded(UiElement &child);
		
		protected: void RenderAndQueryInternal(Core::UiBuilder& builder) override;

			private: void OnPreRenderAndQueryChild(Core::UiBuilder& builder, size_t childIndex, UiElement& child) const;		

		public: void RemoveLastChild();

	};

	
}
