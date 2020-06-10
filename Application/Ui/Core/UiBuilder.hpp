#pragma once
#include "Types/Vector.hpp"
#include "Shared/InterfaceHelpers.hpp"


namespace App::Ui::Core
{
	 class StringInputTarget;
	
	inline bool IsAbsolute(const Math::Vector2 &size)
	{
		return std::abs(size.x) > 1 || std::abs(size.y) > 1;
		
	}

	
	
	class UiBuilder
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(UiBuilder)
		

		public: virtual UiBuilder &DeclareAutoWidth() = 0;

		public: virtual UiBuilder &LeaveWidget() = 0;


		public: virtual UiBuilder &DeclareName(const char *name) = 0;

		public: virtual UiBuilder &DeclareAlignment(float alignment) = 0;

		public: virtual UiBuilder &DeclareTabStatic() = 0;

		public: virtual UiBuilder &DeclareTabSize(const Math::Vector2 &relativeSize) = 0;

		public: virtual UiBuilder &DeclareTabPos(const Math::Vector2 &relativePos, const Math::Vector2 &pivot) = 0;

		public: virtual UiBuilder &DeclareTabNocollapse() = 0;
		
		public: virtual UiBuilder &MakeTab(bool *isOpenTarget) = 0;


		
		public: virtual UiBuilder &MakeWrapper() = 0;

		public: virtual UiBuilder &MakeButton(bool *isPressed, bool centerVertical) = 0;


		public: virtual UiBuilder &MakeTextInput(Core::StringInputTarget &target) = 0;

		
		public: virtual UiBuilder &MakeGrid(size_t columns, size_t rows) = 0;

		public: virtual UiBuilder &MakeCell(size_t startColIndex, size_t startRowIndex, size_t colSpan = 1, size_t rowSpan = 1) = 0;

		
		public: virtual UiBuilder &MakeModal() = 0;

		public: virtual UiBuilder &MakeText(const char *text) = 0;

		public: virtual UiBuilder &MakeCheckbox(bool *isChecked) = 0;


		
		
		
	};
	
	
}

