#include "Ui/Elements/FloatLayout.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include <algorithm>


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
				if(invertDirection)
				{
					child.position.y = builder.GetItemPos().y - itemPadding;
				}
				else
				{
					child.position.y = builder.GetItemPos().y + builder.GetItemSize().y + itemPadding;					
				}
			}
			else
			{
				if(invertDirection)
				{
					child.position.x = builder.GetItemPos().x - itemPadding;
				}
				else
				{
					child.position.x = builder.GetItemPos().x + builder.GetItemSize().x + itemPadding;									
				}
			}			
		}

		if(isVertical)
		{
			if(builder.IsRelativeSize(child.size.y))
			{
				if(invertDirection)
				{
					child.size.y *= builder.GetItemPos().y - itemPadding;
					child.size.y = std::clamp(child.size.y, 0.f, std::abs(child.size.y));
				}
				else
				{
					child.size.y *= builder.GetContentRegion().y - (builder.GetItemPos().y + itemPadding);
					child.size.y = std::clamp(child.size.y, 0.f, std::abs(child.size.y));
				}
			}			
		}
		else
		{
			if(builder.IsRelativeSize(child.size.x))
			{
				if(invertDirection)
				{
					child.size.x *= builder.GetItemPos().x - itemPadding;
					child.size.x = std::clamp(child.size.x, 0.f, std::abs(child.size.x));
				}
				else
				{
					child.size.x *= builder.GetContentRegion().x - (builder.GetItemPos().x + itemPadding);
					child.size.x = std::clamp(child.size.x, 0.f, std::abs(child.size.x));
				}
			}
		}
						
	}

	
}
