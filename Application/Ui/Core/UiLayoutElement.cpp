#include "Ui/Core/UiLayoutElement.hpp"


namespace App::Ui
{	
	void UiLayoutElement::AddChild(UniquePtr<UiElement> &&child)
	{
		OnChildAdded(*children.emplace_back(std::move(child)));
		
	}


	
	void UiLayoutElement::RenderAndQueryInput(UiBuilder &builder)
	{
		OnPreRenderAndQueryChildren(builder);

		for(auto &&child : children)
		{
			OnPreRenderAndQueryChild(builder);
			child->RenderAndQueryInput(builder);
			OnPostRenderAndQueryChild(builder);
		}

		OnPostRenderAndQueryChildren(builder);
		
	}

	
}
