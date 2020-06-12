#include "Ui/Elements/GridLayout.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/Slot.hpp"
#include "Shared/Exception/Exception.hpp"


namespace App::Ui
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



	GridLayout::GridLayout(const unsigned numColumns, const unsigned numRows)
		:
		name{ "Grid_" + std::to_string(constructedInstances) },
		numColumns{ numColumns },
		numRows{ numRows },
		currentSlot{ MakeUnique<GridSlot>(0,0,1,1) }
	{			
		++constructedInstances;
		
	}


	
	GridLayout::~GridLayout() noexcept = default;



	GridLayout &GridLayout::AddChildAt(const GridCell &cell, UniquePtr<UiElement> &&child)
	{
		DeclareChildPos(cell.startColumnIndex, cell.startRowIndex, cell.spanColumn, cell.spanRow);
		AddChild(std::move(child));

		const auto isValid
		{
			cell.spanColumn > 0 && cell.spanColumn <= numColumns
			&& cell.spanRow > 0 && cell.spanRow <= numRows
			&& numColumns-cell.startColumnIndex >= 1
			&& numRows-cell.startRowIndex >= 1
		};
		Exception::ThrowIfDebug(!isValid, {"GridLayout: The specified grid cell is out of bounds"});
				
		return *this;
		
	}
	
		void GridLayout::DeclareChildPos
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
					
		}

	
	
	void GridLayout::OnChildAdded(UiElement &child)
	{
		child.SetSlot(MakeUnique<GridSlot>(*currentSlot));
		
	}


	
	void GridLayout::OnPreRenderAndQueryChildren(Core::UiBuilder &builder)
	{
		builder
		.DeclareName(name.c_str())
		.MakeGrid(numColumns, numRows);

	}


	
	void GridLayout::OnPreRenderAndQueryChild(Core::UiBuilder &builder, size_t childIndex, UiElement &child)
	{
		auto *slot{ static_cast<GridSlot *>(child.GetSlot()) };
		
		builder.MakeCell
		(
			slot->startColumnIndex,
			slot->startRowIndex, 
			slot->spanColumn,
			slot->spanRow
		);			
		
	}


	
	void GridLayout::OnPostRenderAndQueryChild(Core::UiBuilder &builder, size_t childIndex, UiElement &child)
	{
		builder.LeaveWidget();

	}


	
	void GridLayout::OnPostRenderAndQueryChildren(Core::UiBuilder &builder)
	{
		builder.LeaveWidget();
		
	}

	
}
