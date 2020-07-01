#pragma once
#include "Ui/Core/UiLayoutElement.hpp"

#include "Ui/Core/UiBuilder.hpp"


namespace App::Ui
{
	class TreeNode
	{
		std::string displayName;
		UniquePtr<Core::UiElement> element;
		
	};
	
	class TreeLayout final : public Core::UiLayoutElement
	{
		public: std::string nextDisplayName;


		
		protected: void OnChildAdded(UiElement &child) override
		{
			
		}
				
	};

	
}
