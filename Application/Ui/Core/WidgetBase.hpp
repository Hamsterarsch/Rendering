#pragma once
#include "Shared/PtrTypes.hpp"
#include <forward_list>


namespace App::Ui
{	
	class UiBuilder;
	class WidgetBehavior;
	class Slot;
	
	class WidgetBase
	{
		private: size_t childCount;
		
		private: bool isHidden;
		
		private: UniquePtr<Slot> slot;
		
		protected: std::forward_list<UniquePtr<WidgetBase>> children;

		protected: unsigned zIndex;


		
		public: WidgetBase();

		public: virtual ~WidgetBase();
		
		protected: virtual WidgetBehavior *GetBehavior() { return nullptr; }
		
		public: void SetIsHidden(bool value) { isHidden = value; }

		protected: size_t GetChildCount() const { return childCount; }

		public: Slot *GetSlot() { return slot.get(); }

		
		
		public: void Render(UiBuilder &builder);
		
		public: bool IsHidden() const { return isHidden; }
		
		protected: virtual void RenderInternal(UiBuilder &builder) = 0;

		protected: void RenderChildren(UiBuilder &builder);
				   					 
		
		public: void AddChild(UniquePtr<WidgetBase> &&widget);

		protected: virtual void OnChildAdded(WidgetBase &child) {};

		public: void UpdateBehaviors();
		
		public: void SetSlot(UniquePtr<Slot> &&slot);
							   				 						
	};

	
}
