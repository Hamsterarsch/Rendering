#pragma once
#include "Types/Vector.hpp"
#include "Shared/InterfaceHelpers.hpp"


namespace App::Core{ struct ImageView; }


namespace App::Ui::Core
{
	class StringInputTarget;

		
	class UiBuilder
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(UiBuilder)
		

		public: virtual UiBuilder &DeclareSize(const Math::Vector2 &size) = 0;
		
		public: virtual UiBuilder &DeclarePosition(const Math::Vector2 &position, const Math::Vector2 &pivot) = 0;
		
		public: virtual UiBuilder &DeclareName(const char *name) = 0;

		public: virtual UiBuilder &DeclarePadding(float padding) = 0;
		
		public: virtual UiBuilder &DeclareTabStatic() = 0;

		public: virtual UiBuilder &DeclareTabNocollapse() = 0;

		public: virtual UiBuilder &DeclareTabChildDirectionRight() = 0;

		public: virtual UiBuilder &DeclareTabChildDirectionDown() = 0;

		public: virtual UiBuilder &DeclareTabChildSize(float percentIntoDeclaredDirection) = 0;

		public: virtual UiBuilder &DeclareButtonDisabled() = 0;


		public: virtual Math::Vector2 GetItemPos() const = 0;
		
		public: virtual Math::Vector2 GetItemSize() const = 0;

		public: virtual Math::Vector2 GetContentRegion() = 0;

		public: virtual bool IsRelativeSize(float value) const = 0;


		
		public: virtual UiBuilder &LeaveWidget() = 0;
		
		public: virtual UiBuilder &MakeTab(bool *isOpenTarget) = 0;

		public: virtual UiBuilder &MakeTabWithChild(bool *isOpenTarget) = 0;
				
		public: virtual UiBuilder &MakeModal() = 0;

		public: virtual UiBuilder &MakeButton(bool *isPressed) = 0;

		public: virtual UiBuilder &MakeText(const char *text) = 0;

		public: virtual UiBuilder &MakeTextInput(StringInputTarget &target, bool isMultiline) = 0;

		public: virtual UiBuilder &MakeCheckbox(bool *isChecked) = 0;


		public: virtual UiBuilder &MakeImage(const App::Core::ImageView &image) = 0;
		
		public: virtual UiBuilder &MakeImageButton(const App::Core::ImageView &image, bool *isPressed) = 0;
		
		
		public: virtual UiBuilder &MakeGrid(size_t columns, size_t rows) = 0;

		public: virtual UiBuilder &MakeCell(size_t startColIndex, size_t startRowIndex, size_t colSpan = 1, size_t rowSpan = 1) = 0;


		
		public: virtual UiBuilder &DeclareReadonlyInput() = 0;
		
		public: virtual UiBuilder &MakeInputInt(int &data) = 0;
		
		public: virtual UiBuilder &MakeInputFloat(float &data) = 0;

		public: virtual UiBuilder &MakeInputFloat4(float data[4]) = 0;

		public: virtual UiBuilder &MakeInputUnsigned(unsigned &data) = 0;
						
	};

	
}

