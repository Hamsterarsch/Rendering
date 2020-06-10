#pragma once
#include "Ui/Core/UiLayoutElement.hpp"
#include <string>


namespace App::Ui
{
	class GridSlot;
	
	class GridLayout final : public Core::UiLayoutElement
	{
		private: static inline unsigned constructedInstances{ 0 };

		private: std::string name;

		private: unsigned numColumns;

		private: unsigned numRows;

		private: UniquePtr<GridSlot> currentSlot;
		

		
		public: GridLayout(unsigned numColumns, unsigned numRows);

		public: ~GridLayout() noexcept override;

		
		public: GridLayout &DeclareChildPos(unsigned startColumnIndex, unsigned startRowIndex, unsigned spanColumn = 1, unsigned spanRow = 1);
		
		protected: void OnChildAdded(UiElement &child) override;

		protected: void OnPreRenderAndQueryChildren(Core::UiBuilder &builder) override;
		
		protected: void OnPreRenderAndQueryChild(Core::UiBuilder &builder, size_t childIndex, UiElement &child) override;
		
		protected: void OnPostRenderAndQueryChild(Core::UiBuilder &builder, size_t childIndex, UiElement &child) override;
		
		protected: void OnPostRenderAndQueryChildren(Core::UiBuilder &builder) override;
		
	};

	
}

