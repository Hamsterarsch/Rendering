#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "Types/Vector.hpp"


namespace App::Ui
{
	inline bool IsAbsolute(const Math::Vector2 &size)
	{
		return std::abs(size.x) > 1 || std::abs(size.y) > 1;
		
	}

	
	class StringInputTarget;
	
	class WidgetBuilder
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(WidgetBuilder)

		public: virtual WidgetBuilder &DeclareAutoWidth() = 0;

		public: virtual WidgetBuilder &LeaveWidget() = 0;


		public: virtual WidgetBuilder &DeclareName(const char *name) = 0;

		public: virtual WidgetBuilder &DeclareAlignment(float alignment) = 0;

		public: virtual WidgetBuilder &DeclareTabStatic() = 0;

		public: virtual WidgetBuilder &DeclareTabSize(const Math::Vector2 &relativeSize) = 0;

		public: virtual WidgetBuilder &DeclareTabPos(const Math::Vector2 &relativePos, const Math::Vector2 &pivot) = 0;

		public: virtual WidgetBuilder &DeclareTabNocollapse() = 0;
		
		public: virtual WidgetBuilder &MakeTab() = 0;


		
		public: virtual WidgetBuilder &MakeWrapper() = 0;

		public: virtual WidgetBuilder &MakeButton(bool *isPressed, bool centerVertical) = 0;


		public: virtual WidgetBuilder &MakeTextInput(StringInputTarget &target) = 0;

		
		public: virtual WidgetBuilder &MakeGrid(size_t columns, size_t rows) = 0;

		public: virtual WidgetBuilder &MakeCell(size_t startColIndex, size_t startRowIndex, size_t colSpan = 1, size_t rowSpan = 1) = 0;


		
		
		
	};
	
	
}

