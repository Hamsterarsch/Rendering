#include "Ui/BasicFrontends/SetVisibilityFrontend.hpp"


namespace App::Ui
{
	SetVisibilityFrontend::SetVisibilityFrontend()
		:
		wasNotified{ false },
		onlyVisibleWhileTrue{ false },
		toggleInsteadOfSet{ false },
		targetElement{ nullptr }
	{
	}


	
	SetVisibilityFrontend::SetVisibilityFrontend(Core::UiElement &targetElement, const bool onlyVisibleWhileTrue, const bool toggleInsteadOfSet)
		:
		wasNotified{ false },
		onlyVisibleWhileTrue{ onlyVisibleWhileTrue },
		toggleInsteadOfSet{ toggleInsteadOfSet },
		targetElement{ &targetElement }
	{
	}



	void SetVisibilityFrontend::Update(Core::UiBuilder &builder)
	{
		if(!targetElement)
		{
			return;
			
		}
		
		if(toggleInsteadOfSet)
		{
			DoToggleUpdate();
		}
		else
		{
			DoSetUpdate();
		}
		
	}

		void SetVisibilityFrontend::DoToggleUpdate()
		{
			if(wasNotified)
			{
				targetElement->SetIsHidden(!targetElement->IsHidden());
			}
		
		}
			   	
		void SetVisibilityFrontend::DoSetUpdate()
		{
			if(wasNotified)
			{
				targetElement->SetIsHidden(false);			
			}
			else if(onlyVisibleWhileTrue)
			{
				targetElement->SetIsHidden(true);
			}
		
		}

	

	bool *SetVisibilityFrontend::GetInputTargetBool(const size_t index)
	{
		return &wasNotified;
		
	}

	
}
