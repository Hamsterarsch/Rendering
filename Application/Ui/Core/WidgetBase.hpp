#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "Types/Vector.hpp"
#include "Ui/Core/WidgetBehavior.hpp"

namespace App::Ui
{	
	class WidgetBuilder;
	class WidgetBehavior;

	
	class WidgetBase
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS_NOCTOR(WidgetBase)

		private: size_t childCount;
		
		protected: std::forward_list<UniquePtr<WidgetBase>> children;
		
		private: bool isHidden;

		protected: unsigned zIndex;


		
		public: WidgetBase() : childCount{ 0 }, isHidden{ false }, zIndex{ 0 } {}

		public: void Render(WidgetBuilder &builder)
		{
			if(IsHidden())
			{
				return;
				
			}
			
			RenderInternal(builder);

			
		}
		
		protected: virtual void RenderInternal(WidgetBuilder &builder) = 0;

		protected: void RenderChildren(WidgetBuilder &builder)
		{
			for(auto &&child : children)
			{
				child->Render(builder);
			}
		}

		protected: size_t GetChildCount() const { return childCount; }

		public: virtual WidgetBehavior *GetBehavior() = 0;

		
		public: void SetIsHidden(bool value) { isHidden = value; }

		public: bool IsHidden() const { return isHidden; }

		public: void AddChild(UniquePtr<WidgetBase> &&widget) { children.emplace_front(std::move(widget)); ++childCount; }

		public: void UpdateBehaviors()
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
						
	};


	
}
