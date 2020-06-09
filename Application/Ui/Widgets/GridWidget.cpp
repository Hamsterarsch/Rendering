#include "Ui/Widgets/GridWidget.hpp"
#include "Ui/Core/WidgetBuilder.hpp"
#include "Ui/Core/Slot.hpp"


namespace App::Ui::Widgets
{
	class GridSlot final : public Slot
	{
		public: unsigned startColumnIndex;

		public: unsigned startRowIndex;

		public: unsigned spanColumn;

		public: unsigned spanRow;



		GridSlot(unsigned startColumnIndex, unsigned startRowIndex, unsigned spanColumn, unsigned spanRow)
			:
			startColumnIndex{ startColumnIndex },
			startRowIndex{ startRowIndex },
			spanColumn{ spanColumn },
			spanRow{ spanRow }
		{}
		
	};



	GridWidget::GridWidget(const unsigned numColumns, const unsigned numRows)
		:
		name{ "Grid_" + std::to_string(constructedInstances) },
		numColumns{ numColumns },
		numRows{ numRows },
		currentSlot{ MakeUnique<GridSlot>(0,0,1,1) }
	{			
		++constructedInstances;
		
	}

	

	GridWidget &GridWidget::DeclareChildPos
	(
		const unsigned startColumnIndex,
		const unsigned startRowIndex,
		const unsigned spanColumn,
		const unsigned spanRow
	)
	{
		currentSlot->startColumnIndex = startColumnIndex;
		currentSlot->startRowIndex = startRowIndex;
		currentSlot->spanColumn = spanColumn;
		currentSlot->spanRow = spanRow;

		return *this;
		
	}



	void GridWidget::OnChildAdded(WidgetBase &child)
	{
		child.SetSlot(MakeUnique<GridSlot>(*currentSlot));
		
	}


	
	void GridWidget::RenderInternal(WidgetBuilder &builder)
	{			
		builder
		.DeclareName(name.c_str())
		.MakeGrid(numColumns, numRows);

		size_t renderedChildren{ 0 };
		for(auto &&child : children)
		{
			const auto *slot{ static_cast<GridSlot *>(child->GetSlot()) };
			
			builder.MakeCell(slot->startColumnIndex, slot->startRowIndex, slot->spanColumn, slot->spanRow);			
			child->Render(builder);
			builder.LeaveWidget();
			++renderedChildren;
		}

		builder.LeaveWidget();
					
	}
	
	
}
