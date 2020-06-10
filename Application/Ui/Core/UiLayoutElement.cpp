#include "Ui/Core/UiLayoutElement.hpp"


namespace App::Ui::Core
{	
	void UiLayoutElement::AddChild(UniquePtr<UiElement> &&child)
	{
		OnChildAdded(*children.emplace_back(std::move(child)));
		
	}


	
	void UiLayoutElement::RenderAndQueryInternal(UiBuilder &builder)
	{
		OnPreRenderAndQueryChildren(builder);

		for(size_t childIndex{ 0 }; childIndex < children.size(); ++childIndex)
		{
			OnPreRenderAndQueryChild(builder, childIndex, *children.at(childIndex));
			children.at(childIndex)->RenderAndQueryInternal(builder);
			OnPostRenderAndQueryChild(builder, childIndex, *children.at(childIndex));
		}

		OnPostRenderAndQueryChildren(builder);
		
	}

	
}
