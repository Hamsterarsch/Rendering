#include "Ui/Core/WidgetBase.hpp"
#include "Ui/Core/WidgetBehavior.hpp"
#include "Ui/Core/Slot.hpp"
#include "Shared/Exception/Exception.hpp"


namespace App::Ui
{
	WidgetBase::WidgetBase()
		:
		childCount{ 0 },
		isHidden{ false },
		zIndex{ 0 } 
	{
	}


	
	WidgetBase::~WidgetBase() = default;

	   

	void WidgetBase::Render(UiBuilder &builder)
	{
		if(IsHidden())
		{
			return;
			
		}
		
		RenderInternal(builder);
				
	}


	
	void WidgetBase::RenderChildren(UiBuilder &builder)
	{
		for(auto &&child : children)
		{
			child->Render(builder);
		}
		
	}


	
	void WidgetBase::AddChild(UniquePtr<WidgetBase>&& widget)
	{
		children.emplace_front(std::move(widget));
		++childCount;

		OnChildAdded(*children.front());
		
	}

	
	void WidgetBase::UpdateBehaviors()
	{
		if(GetBehavior())
		{
			GetBehavior()->Update();
			
		}
		
		for(auto &&child : children)
		{
			child->UpdateBehaviors();
		}
		
	}

	

	void WidgetBase::SetSlot(UniquePtr<Slot> &&slot)
	{
		this->slot = std::move(slot);
		
	}
	
	
}
