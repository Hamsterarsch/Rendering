#pragma once
#include "WidgetBuilder.hpp"
#include <forward_list>


#include "ThirdParty/imgui/imgui.h"


namespace App::Ui
{
	class WidgetBuilderImpl final : public WidgetBuilder
	{
		private: std::forward_list<void(*)()> desctructionFuncStack;
				 		
		
		private: struct
		{
			std::string name{ " " };
			float alignment{ 0 };
			ImGuiWindowFlags flagsWindow{ 0 };
			Math::Vector2 relativeSize{ .5, .5 };
			Math::Vector2 relativePos{ .5, .5 };
			Math::Vector2 pivot{ .5, .5 };
		} data, defaults;
		
		private: struct
		{
			unsigned columnCount;
			unsigned rowCount;
			float colWidth;
			float rowHeight;
		} gridData;
				 	
				 
		
		public: WidgetBuilder &DeclareAutoWidth() override;


		public: WidgetBuilder &LeaveWidget() override;

				
		public: WidgetBuilder &DeclareName(const char *name) override;

		public: WidgetBuilder &DeclareAlignment(float alignment) override;

		public: WidgetBuilder &DeclareTabStatic() override;

		public: WidgetBuilder &DeclareTabSize(const Math::Vector2 &relativeSize) override;

		public: WidgetBuilder &DeclareTabPos(const Math::Vector2 &relativePos, const Math::Vector2 &pivot) override;

		public: WidgetBuilder &DeclareTabNocollapse() override;
		
		public: WidgetBuilder &MakeTab() override;
		
		
		public: WidgetBuilder &MakeWrapper() override;

			private: void DoItemPrologue();

		public: WidgetBuilder &MakeButton(bool *isPressed, bool centerVertical) override;
				
		WidgetBuilder& MakeTextInput(StringInputTarget& target) override;

		
		WidgetBuilder& MakeGrid(size_t columns, size_t rows) override;
		
		WidgetBuilder& MakeCell(size_t startColIndex, size_t startRowIndex, size_t colSpan = 1, size_t rowSpan = 1) override;


		WidgetBuilder& MakeModal(bool* isOpen) override;
		WidgetBuilder& MakeText(const char* text) override;
		WidgetBuilder& MakeCheckbox(bool* isChecked) override;

			private: void CenterNextItem(float nextItemWidth) const;
	};

	
}
