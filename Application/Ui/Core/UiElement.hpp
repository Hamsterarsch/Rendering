#pragma once
#include "Shared/PtrTypes.hpp"
#include "Ui/Core/Slot.hpp"
#include "Types/Vector.hpp"


namespace App::Ui::Core
{
	class UiBuilder;
	class UiDecorator;
	
	
	class UiElement
	{
		private: UniquePtr<Slot> slot;

		private: bool isHidden{ false };

		
		public: Math::Vector2 size{ 0, 0 };
		
		public: Math::Vector2 pivot{ 0, 0 };

		public: Math::Vector2 position{ 0, 0 };

		
		public: UiElement() = default;

		public: virtual ~UiElement() = default;
		
		public: UiElement(UiElement &&) noexcept = default;
		
		public: UiElement &operator=(UiElement &&) noexcept = default;		

		public: UiElement(const UiElement &) = delete;

		public: UiElement &operator=(const UiElement &) = delete;


		public: bool RenderAndQueryInput(UiBuilder &builder);
		
		protected: virtual void RenderAndQueryInternal(UiBuilder &builder) = 0;

		public: bool IsHidden() const { return isHidden; }

		public: void SetIsHidden(bool value) { isHidden = value; }

		public: void SetSlot(UniquePtr<Slot> &&slot) { this->slot = std::move(slot); }

		public: Slot *GetSlot() { return slot.get(); }
		
	};

		
}
