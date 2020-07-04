#include "Ui/Elements/CollapsibleNodeLayoutFrontend.hpp"


namespace App::Ui
{
	void CollapsibleNodeLayoutFrontend::Update(Core::UiBuilder &builder)
	{
		if(targetTree)
		{
			targetTree->SetChildIsHidden(not isHeaderNodeToggledOn);				
		}

		if(childFrontend && isHeaderNodeToggledOn)
		{
			childFrontend->Update(builder);
		}
					
	}

	
}
