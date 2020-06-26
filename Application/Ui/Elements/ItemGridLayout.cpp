#include "Ui/Elements/ItemGridLayout.hpp"
#include "Ui/Core/UiBuilder.hpp"


namespace App::Ui
{
	ItemGridLayout::ItemGridLayout(const Math::Vector2 &itemSize, const float itemBorder, const bool takeAvailableWidth)
		:
		minItemBorder{ itemBorder },
		itemSize{ itemSize },
		takeAvailableWidth{ takeAvailableWidth }
	{
	}


	
	ItemGridLayout::~ItemGridLayout() = default;

	

	void ItemGridLayout::OnChildAdded(UiElement &child)
	{
		child.size = itemSize;
		
		child.pivot = {.5 , 0};
		
	}

	void ItemGridLayout::OnPreRenderAndQueryChildren(Core::UiBuilder &builder)
	{
		const auto contentRegion{ builder.GetContentRegion() };
		itemsPerRow = static_cast<unsigned>((contentRegion.x-minItemBorder) / (itemSize.x+minItemBorder));
		widthPerItem = static_cast<unsigned>(contentRegion.x) / itemsPerRow - minItemBorder;
			
	}

	void ItemGridLayout::OnPreRenderAndQueryChild(Core::UiBuilder &builder, const size_t childIndex, UiElement &child)
	{		
		const auto offsetToItemStart{ childIndex % itemsPerRow * (widthPerItem+minItemBorder) };
		if(takeAvailableWidth)
		{
			child.size.x = widthPerItem-minItemBorder;
			child.pivot.x = 0;
			child.position.x = offsetToItemStart;
		}
		else
		{
			child.position.x = offsetToItemStart + widthPerItem*.5 + minItemBorder;			
		}
				
		const auto rowIndex{ childIndex / itemsPerRow };
		const auto offsetToItemRow{ minItemBorder + rowIndex * (itemSize.y+rowIndex) };
		child.position.y = offsetToItemRow;
		
	}

	
}
