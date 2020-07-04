#include "Ui/User/ObjectList.hpp"
#include "Ui/Core/ConstructionHelpers.hpp"
#include "Ui/Elements/FloatLayout.hpp"


namespace App::Ui::User
{
	ObjectListFrontend::ObjectListFrontend()
	{
		auto layout{ Element<FloatLayout>(5, true) };
		list = layout.get();		
		
		uiElements.push_front(std::move(layout));
				
	}


	
	ObjectListFrontend::ObjectListFrontend(Core::UiLayoutElement &parent)
	{
		auto layout{ Element<FloatLayout>(5, true) };
		list = layout.get();		
				
		parent.AddChild(std::move(layout));
		
	}


	
	void ObjectListFrontend::Update(Core::UiBuilder &builder)
	{
		if(not uiElements.empty() && not RenderAndQueryInputForUiElements(builder))
		{
			return;
			
		}

		for(auto &&frontend : nodeFrontends)
		{
			frontend->Update(builder);
		}
		
	}


	
	void ObjectListFrontend::PushBack
	(
		const char *name,
		UniquePtr<Core::UiElement> &&editUi,
		UniquePtr<UiFrontend> &&editFrontend
	)
	{
		nodeFrontends.emplace_back(MakeUnique<CollapsibleNodeLayoutFrontend>(std::move(editFrontend)));
		list->AddChild(Element<CollapsibleNodeLayout>(*nodeFrontends.back(), name) << std::move(editUi));
		
	}


	
	void ObjectListFrontend::PopBack()
	{
		if(not nodeFrontends.empty())
		{
			list->RemoveLastChild();
			nodeFrontends.pop_back();			
		}
		
	}


	
	void ObjectListFrontend::ForEachFrontend(const std::function<void(UiFrontend &)> &operation)
	{
		for(auto &&node : nodeFrontends)
		{
			if(auto *frontend{ node->GetChildFrontend() })
			{
				operation(*frontend);
			}
		}
		
	}

	
}
