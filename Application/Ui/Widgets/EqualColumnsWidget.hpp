#pragma once
#include "Ui/Core/WidgetBase.hpp"


#include "Ui/Core/WidgetBuilder.hpp"

namespace App::Ui::Widgets
{
	class EqualColumnsWidget : public WidgetBase
	{
		private: static inline unsigned constructedInstances{ 0 };

		private: std::string name;

		public: EqualColumnsWidget() : name{ "EqualGrid_" + std::to_string(constructedInstances) }
		{			
			++constructedInstances;
			
		}
		
		
		public: void RenderInternal(WidgetBuilder &builder) override
		{			
			builder
			.DeclareName(name.c_str())
			.MakeGrid(GetChildCount(), 1);
			
			size_t renderedChildren{ 0 };
			for(auto &&child : children)
			{
				builder.MakeCell(renderedChildren, 0, 1, 1);			
				child->Render(builder);
				builder.LeaveWidget();
				++renderedChildren;
			}

			builder.LeaveWidget();
						
		}

		WidgetBehavior* GetBehavior() override{ return nullptr; }
		
	};

	
}
