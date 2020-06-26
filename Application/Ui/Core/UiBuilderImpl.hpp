#pragma once
#include "UiBuilder.hpp"
#include <forward_list>


#include "ThirdParty/imgui/imgui.h"
struct ImVec2;


namespace App::Ui::Core
{
	class UiBuilderImpl final : public UiBuilder
	{		
		private: std::forward_list<void(*)()> widgetLeveFunct;

		private: std::forward_list<Math::Vector2> lastItemPosStack;
		

		
		private: struct
		{
			std::string name{ " " };			
			Math::Vector2 size{ 0, 0 };
			Math::Vector2 position{ 0, 0 };
			Math::Vector2 pivot{ .5, .5 };
			float padding{ 0 };
			ImGuiWindowFlags flagsWindow{ 0 };
			bool isButtonDisabled{ false };
		} userSettings;
		
		private: struct
		{
			unsigned columnCount;
			unsigned rowCount;
			float colWidth;
			float rowHeight;
			float cellPadding{ 0 };
		} gridData;
				 	
				 		

		public: UiBuilderImpl();
		
		public:	UiBuilder &DeclareSize(const Math::Vector2& size) override;

		public: UiBuilder &DeclarePosition(const Math::Vector2 &position, const Math::Vector2 &pivot) override;

		public: UiBuilder &DeclareName(const char *name) override;

		public: UiBuilder &DeclarePadding(float padding) override;
		
		public: UiBuilder &DeclareTabStatic() override;

		public: UiBuilder &DeclareTabNocollapse() override;

		public: UiBuilder &DeclareButtonDisabled() override;

						
		public: Math::Vector2 GetItemPos() const override;
		
		public: Math::Vector2 GetItemSize() const override;

		public: Math::Vector2 GetContentRegion() override;
		
				
		public: UiBuilder &LeaveWidget() override;
		
		public: UiBuilder &MakeTab(bool *isOpenTarget) override;

			private: void DoItemPrologue();
		
			private: void ApplyDimensionsForWindowTypeElements() const;

				private: void ApplyUserSizing(float &width, float &height, bool forWindow = false) const;

					private: static bool ShouldNotUseDefaultValue(float userSpecification);

					private: static float GetRelativeOrAbsoluteValue(float userValue, float valueMaximum);

				private: void ApplyUserPositioning(float &positionX, float &positionY, bool forWindow = false) const;

			private: void OnBeginCanvas();
		
			private: void DoItemEpilogue();
							
		public: UiBuilder &MakeModal() override;
		
		public: UiBuilder &MakeButton(bool *isPressed) override;

			private: ImVec2 ApplyDimensionsForTextTypeElements(const char *text) ;

				private: static void SetNextItemSize(float width, float height);

				private: void ApplyUserPivot(float &positionX, float &positionY, float itemWidth, float itemHeight);
		
		public: UiBuilder &MakeText(const char* text) override;
		
		public: UiBuilder &MakeTextInput(StringInputTarget& target) override;						
		
		public: UiBuilder &MakeCheckbox(bool* isChecked) override;

		
		public: UiBuilder &MakeImageButton(const App::Core::ImageView &image, bool *isPressed) override;


		//imgui exposing functions
		
		public: UiBuilder &MakeGrid(size_t columns, size_t rows) override;

		
		public: UiBuilder &MakeCell(size_t startColIndex, size_t startRowIndex, size_t colSpan = 1, size_t rowSpan = 1) override;

		


	};

	
}
