#include "Ui/Elements/FloatLayout.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include <algorithm>


namespace App::Ui
{
	class FloatSlot final : public Slot
	{
		public: Math::Vector2 initialSize;

		public: FloatSlot(const Math::Vector2 &initialSize) : initialSize{ initialSize } {}
		
	};



	void FloatLayout::AddChild(UniquePtr<UiElement>&& child)
	{
		OnChildAdded(*children.emplace_back(std::move(child)));
		
	}

		void FloatLayout::OnChildAdded(UiElement &child)
		{
			child.SetSlot(MakeUnique<FloatSlot>(child.size));
			
		}


	
	void FloatLayout::RenderAndQueryInternal(Core::UiBuilder &builder)
	{
		for(size_t childIndex{ 0 }; childIndex < children.size(); ++childIndex)
		{
			OnPreRenderAndQueryChild(builder, childIndex, *children.at(childIndex));
			children.at(childIndex)->RenderAndQueryInput(builder);
		}
		
	}



	void FloatLayout::OnPreRenderAndQueryChild(Core::UiBuilder &builder, const size_t childIndex, UiElement &child) const
	{		
		child.position = position;
		child.pivot = pivot;
		
		if(childIndex > 0)
		{			
			if(isVertical)
			{
				if(invertDirection)
				{
					child.position.y = builder.GetItemPos().y - itemPadding;
				}
				else
				{
					child.position.y = builder.GetItemPos().y + builder.GetItemSize().y + itemPadding;					
				}
			}
			else
			{
				if(invertDirection)
				{
					child.position.x = builder.GetItemPos().x - itemPadding;
				}
				else
				{
					child.position.x = builder.GetItemPos().x + builder.GetItemSize().x + itemPadding;									
				}
			}			
		}

		if(isVertical)
		{
			const auto initialSizeY{ reinterpret_cast<FloatSlot *>(child.GetSlot())->initialSize.y };
			if(builder.IsRelativeSize(initialSizeY))
			{				
				if(invertDirection)
				{
					child.size.y = initialSizeY * (builder.GetItemPos().y - itemPadding);
					child.size.y = std::clamp(child.size.y, 0.f, std::abs(child.size.y));
				}
				else
				{
					child.size.y = initialSizeY * builder.GetContentRegion().y - (builder.GetItemPos().y + itemPadding);
					child.size.y = std::clamp(child.size.y, 0.f, std::abs(child.size.y));
				}
			}			
		}
		else
		{
			const auto initialSizeX{ reinterpret_cast<FloatSlot *>(child.GetSlot())->initialSize.x };
			if(builder.IsRelativeSize(initialSizeX))
			{
				if(invertDirection)
				{
					child.size.x = initialSizeX * (builder.GetItemPos().x - itemPadding);
					child.size.x = std::clamp(child.size.x, 0.f, std::abs(child.size.x));
				}
				else
				{
					child.size.x = initialSizeX * builder.GetContentRegion().x - (builder.GetItemPos().x + itemPadding);
					child.size.x = std::clamp(child.size.x, 0.f, std::abs(child.size.x));
				}
			}
		}
						
	}
	
	
	void FloatLayout::RemoveLastChild()
	{
		children.pop_back();
		
	}
	
	
}
