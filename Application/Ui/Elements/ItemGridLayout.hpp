#pragma once
#include "Ui/Core/UiLayoutElement.hpp"
#include <string>


namespace App::Ui
{
	class ItemGridLayout final : public Core::UiLayoutElement
	{
		private: static inline unsigned constructedInstances{ 0 };

		private: std::string name;

		private: unsigned itemsPerRow;
		
		private: unsigned widthPerItem;
				
		public: float minItemBorder;

		public: Math::Vector2 itemSize;

		private: bool takeAvailableWidth;


		
		public: ItemGridLayout(const Math::Vector2 &itemSize, float itemBorder, bool takeAvailableWidth);

		public: ~ItemGridLayout() override;

		
		protected: void OnChildAdded(UiElement &child) override;


		void OnPreRenderAndQueryChildren(Core::UiBuilder& builder) override;
		
		void OnPreRenderAndQueryChild(Core::UiBuilder& builder, size_t childIndex, UiElement& child) override;

	};


	
	
}

