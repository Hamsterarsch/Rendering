#pragma once
#include "Shared/PtrTypes.hpp"
#include "Ui/Core/UiElement.hpp"


namespace App::Ui::Core
{	
	class UiLayoutElement : public UiElement
	{				
		public: virtual void AddChild(UniquePtr<UiElement> &&child) = 0;
				
		public: virtual void ClearChildren() = 0;
		
	};

	
}
