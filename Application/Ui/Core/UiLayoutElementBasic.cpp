#include "Ui/Core/UiLayoutElementBasic.hpp"


namespace App::Ui::Core
{	
	void UiLayoutElementBasic::AddChild(UniquePtr<UiElement> &&child)
	{
		OnChildAdded(*children.emplace_back(std::move(child)));
		
	}


	
	void UiLayoutElementBasic::RenderAndQueryInternal(UiBuilder &builder)
	{
		OnPreRenderAndQueryChildren(builder);

		for(size_t childIndex{ 0 }; childIndex < children.size(); ++childIndex)
		{
			OnPreRenderAndQueryChild(builder, childIndex, *children.at(childIndex));
			children.at(childIndex)->RenderAndQueryInput(builder);
			OnPostRenderAndQueryChild(builder, childIndex, *children.at(childIndex));
		}

		OnPostRenderAndQueryChildren(builder);
		
	}

	
}
