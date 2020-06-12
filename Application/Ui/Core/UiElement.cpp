#include "Ui/Core/UiElement.hpp"
#include "UiDecorator.hpp"


namespace App::Ui::Core
{
	UiElement::UiElement() : decorators(0)
	{
	}


	
	UiElement::~UiElement() = default;

	

	bool UiElement::RenderAndQueryInput(UiBuilder &builder)
	{
		if(this->IsHidden())
		{
			return false;
			
		}

		for(auto &&decorator : decorators)
		{
			decorator->Decorate(builder);
		}
		
		RenderAndQueryInternal(builder);

		for(auto &&decorator : decorators)
		{
			decorator->Undecorate(builder);
		}
		
		return true;
		
	}


	
	void UiElement::AddDecorator(UniquePtr<UiDecorator> &&decorator)
	{
		decorators.emplace_back(std::move(decorator));
		
	}

	
}
