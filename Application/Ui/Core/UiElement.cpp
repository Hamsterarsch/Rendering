#include "Ui/Core/UiElement.hpp"
#include "Ui/Core/UiBuilder.hpp"


namespace App::Ui::Core
{	
	bool UiElement::RenderAndQueryInput(UiBuilder &builder)
	{
		if(this->IsHidden())
		{
			return false;
			
		}

		builder
		.DeclareSize(size)
		.DeclarePosition(position, pivot);
				
		RenderAndQueryInternal(builder);
		
		return true;
		
	}

	
}
