#include "Ui/Elements/TreeLayout.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"
#include "FloatLayout.hpp"
#include "ButtonElement.hpp"
#include "TextElement.hpp"


namespace App::Ui
{	
	TreeLayout::TreeLayout(Core::UiFrontend &frontend, size_t isOpenIndex, const char *header, float indent)
		:
		headerElement{ Element<FloatLayout>(0) << Element<TextElement>("\\/") << Element<ToggleButtonElement>(frontend, isOpenIndex, header, true)->*Set{&ToggleButtonElement::size,{1,0}} },
		indent{ indent }
	{		
	}

	TreeLayout::TreeLayout(TreeLayoutFrontend &frontend, const char* header, float indent)
		:
		headerElement{ Element<FloatLayout>(0) << Element<TextElement>("\\/") << Element<ToggleButtonElement>(frontend, 0, header, true)->*Set{&ToggleButtonElement::size,{1,0}} },
		indent{ indent }
	{
		frontend.SetTargetTree(*this);
		
	}



	void TreeLayout::AddChild(UniquePtr<UiElement> &&child)
	{		
		this->child = std::move(child);
		
	}


	
	void TreeLayout::ClearChildren()
	{
		child.reset();
		
	}


	
	void TreeLayout::RenderAndQueryInternal(Core::UiBuilder &builder)
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
