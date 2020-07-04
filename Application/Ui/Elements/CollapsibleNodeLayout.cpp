#include "Ui/Elements/CollapsibleNodeLayout.hpp"
#include "Ui/Elements/CollapsibleNodeLayoutFrontend.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"
#include "FloatLayout.hpp"
#include "ButtonElement.hpp"
#include "TextElement.hpp"


namespace App::Ui
{	
	CollapsibleNodeLayout::CollapsibleNodeLayout(Core::UiFrontend &frontend, size_t isOpenIndex, const char *header, float indent)
		:
		headerElement{ Element<FloatLayout>(0.f) << Element<TextElement>("\\/") << Element<ToggleButtonElement>(frontend, isOpenIndex, header, true)->*Set{&ToggleButtonElement::size,{1,0}} },
		indent{ indent }
	{		
	}

	
	
	CollapsibleNodeLayout::CollapsibleNodeLayout(CollapsibleNodeLayoutFrontend &frontend, const char* header, float indent)
		:
		CollapsibleNodeLayout{ frontend, 0, header, indent }
	{
		frontend.SetTargetNode(*this);
		
	}



	void CollapsibleNodeLayout::AddChild(UniquePtr<UiElement> &&child)
	{		
		this->child = std::move(child);
		
	}


	
	void CollapsibleNodeLayout::ClearChildren()
	{
		child.reset();
		
	}


	
	void CollapsibleNodeLayout::RenderAndQueryInternal(Core::UiBuilder &builder)
	{
		headerElement->position = position;
		headerElement->pivot = pivot;

		headerElement->RenderAndQueryInput(builder);

		if(child) 
		{
			child->position = builder.GetItemPos();
			child->position.x += indent;
			child->position.y += builder.GetItemSize().y + headerSpacing;
			
			child->RenderAndQueryInput(builder);			
		}
				
	}

	
}
