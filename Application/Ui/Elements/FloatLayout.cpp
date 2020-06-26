#include "Ui/Elements/FloatLayout.hpp"
#include "Ui/Core/UiBuilder.hpp"


namespace App::Ui
{	
	void FloatLayout::OnPreRenderAndQueryChild(Core::UiBuilder &builder, const size_t childIndex, UiElement &child)
	{		
		child.position = position;
		child.pivot = pivot;
		
		if(childIndex > 0)
		{			
			if(isVertical)
			{				
				child.position.y = builder.GetItemPos().y + builder.GetItemSize().y + itemPadding;
			}
			else
			{
				child.position.x = builder.GetItemPos().x + builder.GetItemSize().x + itemPadding;				
			}			
		}
						
	}

	
}
