#pragma once
#include "Shared/PtrTypes.hpp"
#include "Ui/Core/Slot.hpp"


namespace App::Ui::Core
{
	class UiBuilder;
		
	class UiElement
	{
		private: UniquePtr<Slot> slot;

		private: bool isHidden{ false };


		
		public: UiElement() = default;

		public: UiElement(UiElement &&) noexcept = default;
		
		public: UiElement &operator=(UiElement &&) noexcept = default;
		
		public: virtual ~UiElement() = default;

		public: UiElement(const UiElement &) = delete;

		public: UiElement &operator=(const UiElement &) = delete;


		public: bool RenderAndQueryInput(UiBuilder &builder)
		{
			if(this->IsHidden())
			{
				return false;
				
			}

			RenderAndQueryInternal(builder);
			return true;
			
		}
		
		public: virtual void RenderAndQueryInternal(UiBuilder &builder) = 0;

		public: bool IsHidden() const { return isHidden; }

		public: void SetIsHidden(bool value) { isHidden = value; }

		public: void SetSlot(UniquePtr<Slot> &&slot) { this->slot = std::move(slot); }

		public: Slot *GetSlot() { return slot.get(); }
		
	};

		
}
