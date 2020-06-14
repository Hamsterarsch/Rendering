#include "Ui/User/PropertyCompositeElement.hpp"


namespace App::Ui::User
{
	PropertyCompositeElement::PropertyCompositeElement(const SharedPtr<Core::UiFrontend> &frontend)
		:
		layout{ columnCount, 1 },
		propertyCount{ 0 },
		frontend{ frontend }
	{
	}


	
	void PropertyCompositeElement::RenderAndQueryInternal(Core::UiBuilder &builder)
	{
		layout.RenderAndQueryInternal(builder);
		
	}
	
	
}
