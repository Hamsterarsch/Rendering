#pragma once
#include "Ui/Core/WidgetBase.hpp"
#include "Shared/PtrTypes.hpp"
#include <string>


namespace App::Ui::Widgets
{
	class GridSlot;
	
	class GridWidget : public WidgetBase
	{
		private: static inline unsigned constructedInstances{ 0 };

		private: std::string name;

		private: unsigned numColumns;

		private: unsigned numRows;

		private: UniquePtr<GridSlot> currentSlot;
		

		
		public: GridWidget(unsigned numColumns, unsigned numRows);

		
		public: GridWidget &DeclareChildPos(unsigned startColumnIndex, unsigned startRowIndex, unsigned spanColumn = 1, unsigned spanRow = 1);
		
		protected: void OnChildAdded(WidgetBase &child) override;

		public: void RenderInternal(UiBuilder &builder) override;

	};

	
}

