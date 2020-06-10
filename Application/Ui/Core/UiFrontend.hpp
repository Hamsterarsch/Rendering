#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "Ui/Core/UiElement.hpp"
#include <forward_list>


namespace App::Ui::Core
{
	class StringInputTarget;
	class UiBuilder;

	
	class UiFrontend
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(UiFrontend)

		protected: std::forward_list<UniquePtr<UiElement>> uiElements;


		
		public: virtual void Update(UiBuilder &builder) = 0;
		
		public: virtual StringInputTarget *GetInputTargetString(size_t index) { return nullptr; }

		public: virtual bool *GetInputTargetBool(size_t index) { return nullptr; }

		protected: bool RenderAndQueryInputForUiElements(UiBuilder &builder)
		{
			bool executedWork{ false };
			for(auto &&element : uiElements)
			{
				executedWork |= element->RenderAndQueryInput(builder);
			}

			return executedWork;
			
		}
									
	};
		
	
}
