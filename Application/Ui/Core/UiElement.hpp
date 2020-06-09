#pragma once
#include "Ui/Core/Slot.hpp"

namespace App::Ui
{
	class UiBuilder;
		
	class UiElement
	{
		private: UniquePtr<Slot> slot;


		
		public: UiElement() = default;

		public: UiElement(UiElement &&) noexcept = default;
		
		public: UiElement &operator=(UiElement &&) noexcept = default;
		
		public: virtual ~UiElement() = default;

		public: UiElement(const UiElement &) = delete;

		public: UiElement &operator=(const UiElement &) = delete;

				
		public: virtual void RenderAndQueryInput(UiBuilder &builder) = 0;

		protected: void SetSlot(UniquePtr<Slot> &&slot) { this->slot = std::move(slot); }

		protected: Slot *GetSlot() { return slot.get(); }
		
	};

		
}
