#pragma once
#include "UiBuilder.hpp"
#include <forward_list>


#include "ThirdParty/imgui/imgui.h"


namespace App::Ui::Core
{
	class UiBuilderImpl final : public UiBuilder
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
				 	
				 
		
		public: UiBuilder &DeclareAutoWidth() override;


		public: UiBuilder &LeaveWidget() override;

				
		public: UiBuilder &DeclareName(const char *name) override;

		public: UiBuilder &DeclareAlignment(float alignment) override;

		public: UiBuilder &DeclareTabStatic() override;

		public: UiBuilder &DeclareTabSize(const Math::Vector2 &relativeSize) override;

		public: UiBuilder &DeclareTabPos(const Math::Vector2 &relativePos, const Math::Vector2 &pivot) override;

		public: UiBuilder &DeclareTabNocollapse() override;
		
		public: UiBuilder &MakeTab(bool *isOpenTarget) override;
		
		
		public: UiBuilder &MakeWrapper() override;

			private: void DoItemPrologue();

		public: UiBuilder &MakeButton(bool *isPressed, bool centerVertical) override;
				
		UiBuilder& MakeTextInput(Core::StringInputTarget& target) override;

		
		UiBuilder& MakeGrid(size_t columns, size_t rows) override;
		
		UiBuilder& MakeCell(size_t startColIndex, size_t startRowIndex, size_t colSpan = 1, size_t rowSpan = 1) override;


		UiBuilder& MakeModal() override;
		UiBuilder& MakeText(const char* text) override;
		UiBuilder& MakeCheckbox(bool* isChecked) override;

			private: void CenterNextItem(float nextItemWidth) const;
	};

	
}
