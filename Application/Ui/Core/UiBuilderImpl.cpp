#include "Ui/Core/UiBuilderImpl.hpp"
#include "ThirdParty/imgui/imgui.h"
#include "Ui/ImguiTypeArithmetics.hpp"
#include <string>
#include <algorithm>
#include "StringInputTarget.hpp"
#include "ThirdParty/imgui/imgui_internal.h"
#include "Core/ImageView.hpp"


namespace App::Ui::Core
{
	UiBuilderImpl::UiBuilderImpl()
	{
		ImGui::GetStyle().WindowPadding = { 0,0 };
		
	}

	

	UiBuilder &UiBuilderImpl::DeclareSize(const Math::Vector2 &size)
	{
		userSettings.size = size;

		return *this;
		
	}


	
	UiBuilder &UiBuilderImpl::DeclarePosition(const Math::Vector2 &position, const Math::Vector2 &pivot)
	{
		userSettings.position = position;
		userSettings.pivot = pivot;
		
		return *this;
		
	}


	
	UiBuilder &UiBuilderImpl::DeclareName(const char *name)
	{
		userSettings.name = name;

		return *this;
		
	}


	
	UiBuilder &UiBuilderImpl::DeclarePadding(const float padding)
	{
		userSettings.padding = padding;

		return *this;
		
	}



	UiBuilder &UiBuilderImpl::DeclareTabStatic()
	{
		userSettings.flagsWindow |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

		return *this;
		
	}


	
	UiBuilder &UiBuilderImpl::DeclareTabNocollapse()
	{
		userSettings.flagsWindow = ImGuiWindowFlags_NoCollapse;

		return *this;
		
	}


	
	UiBuilder &UiBuilderImpl::DeclareButtonDisabled()
	{
		userSettings.isButtonDisabled = true;

		return *this;
		
	}



	UiBuilder &UiBuilderImpl::LeaveWidget()
	{
		(*desctructionFuncStack.front())();
		desctructionFuncStack.pop_front();

		return *this;
		
	}



	UiBuilder &UiBuilderImpl::MakeTab(bool *isOpenTarget)
	{		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2{ .5, .5 });		

		ApplyDimensionsForWindowTypeElements();
		
		ImGui::Begin(userSettings.name.c_str(), isOpenTarget, userSettings.flagsWindow);
		desctructionFuncStack.push_front(&ImGui::End);
				
		ImGui::PopStyleVar();

		DoItemEpilogue();				
		return *this;
		
	}

		void UiBuilderImpl::ApplyDimensionsForWindowTypeElements() const
		{
			auto defaultWindowSize{ ImGui::GetWindowViewport()->Size * .5 };
			ApplyUserSizing(defaultWindowSize.x, defaultWindowSize.y, true);
			ImGui::SetNextWindowSize(defaultWindowSize, ImGuiCond_Appearing);

			auto defaultWindowPos{ ImGui::GetWindowViewport()->Size * .5 };
			ApplyUserPositioning(defaultWindowPos.x, defaultWindowPos.y, true);		
			ImGui::SetNextWindowPos(defaultWindowPos, ImGuiCond_Appearing, {userSettings.pivot.x, userSettings.pivot.y});
			
		}

			void UiBuilderImpl::ApplyUserSizing(float &width, float &height, const bool forWindow) const
			{
				const auto availableRegion{ forWindow ? ImGui::GetWindowViewport()->Size : ImGui::GetContentRegionAvail() };
			
				if(ShouldNotUseDefaultValue(userSettings.size.x))
				{
					width = GetRelativeOrAbsoluteValue(userSettings.size.x, availableRegion.x);				
				}

				if(ShouldNotUseDefaultValue(userSettings.size.y))
				{
					height = GetRelativeOrAbsoluteValue(userSettings.size.y, availableRegion.y);
				}
							   		
			}

				bool UiBuilderImpl::ShouldNotUseDefaultValue(const float userSpecification)
				{
					return userSpecification > 0;
			
				}

				float UiBuilderImpl::GetRelativeOrAbsoluteValue(float userValue, float valueMaximum)
				{
					return userValue <= 1 ? valueMaximum * userValue : userValue;
				
				}

			void UiBuilderImpl::ApplyUserPositioning(float &positionX, float &positionY, const bool forWindow) const
			{
				const auto availableRegion{ forWindow ? ImGui::GetWindowViewport()->Size : ImGui::GetContentRegionAvail() };
			
				if(ShouldNotUseDefaultValue(userSettings.position.x))
				{
					positionX = GetRelativeOrAbsoluteValue(userSettings.position.x, availableRegion.x);
				}

				if(ShouldNotUseDefaultValue(userSettings.position.y))
				{
					positionY = GetRelativeOrAbsoluteValue(userSettings.position.y, availableRegion.y);
				}
						
			}

			void UiBuilderImpl::ApplyUserPivot(float &positionX, float &positionY, float itemWidth, float itemHeight) const
			{
				positionX -= itemWidth * userSettings.pivot.x;
				positionY -= itemHeight * userSettings.pivot.y;
			
			}

		void UiBuilderImpl::DoItemEpilogue()
		{
			userSettings = decltype(userSettings){};
		
		}



	UiBuilder &UiBuilderImpl::MakeModal()
	{		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2{ .5, .5 });		

		ApplyDimensionsForWindowTypeElements();

		if(not ImGui::IsPopupOpen(userSettings.name.c_str()))
		{
			ImGui::OpenPopup(userSettings.name.c_str());//ensure that begin always returns true. if you dont want to display the modal just dont call this function			
		}
		ImGui::BeginPopupModal(userSettings.name.c_str(), nullptr, userSettings.flagsWindow | ImGuiWindowFlags_NoResize);
		desctructionFuncStack.push_front(&ImGui::EndPopup);
		
		ImGui::PopStyleVar();

		DoItemEpilogue();
		return *this;
		
	}

	
	
	UiBuilder &UiBuilderImpl::MakeButton(bool *isPressed)
	{	
		const auto size{ ApplyDimensionsForTextTypeElements(userSettings.name.c_str()) };

		ImGuiButtonFlags flags{};
		if(userSettings.isButtonDisabled)
		{
			flags |= ImGuiButtonFlags_Disabled;			
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));

			auto disabledButtonColor{ ImGui::GetStyleColorVec4(ImGuiCol_Button) };
			disabledButtonColor.w = .5;
			
			ImGui::PushStyleColor(ImGuiCol_Button, disabledButtonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledButtonColor);
		}
		
		const auto pressedResult{ ImGui::ButtonEx(userSettings.name.c_str(), size, flags) };
		if(isPressed)
		{
			*isPressed = pressedResult;
		}

		if(userSettings.isButtonDisabled)
		{			
			ImGui::PopStyleColor(3);
		}

		DoItemEpilogue();		
		return *this;
		
	}

		ImVec2 UiBuilderImpl::ApplyDimensionsForTextTypeElements(const char *text) const
		{
			auto defaultSize{ ImGui::CalcTextSize(text) +  ImGui::GetStyle().FramePadding*2 };
			
			auto usersDesiredSize{ defaultSize };
			ApplyUserSizing(usersDesiredSize.x, usersDesiredSize.y);						
			SetNextItemSize(usersDesiredSize.x, usersDesiredSize.y);				
			
			ImVec2 defaultPos{};
			ApplyUserPositioning(defaultPos.x, defaultPos.y);
			ApplyUserPivot(defaultPos.x, defaultPos.y, usersDesiredSize.x, usersDesiredSize.y);
					
			ImGui::SetCursorPos(ImGui::GetCursorPos() + defaultPos);

			return usersDesiredSize;
		
		}

			void UiBuilderImpl::SetNextItemSize(float width, float height) 
			{
				if(width > 0)
				{
					ImGui::SetNextItemWidth(width);			
				}
				
				if(height > 0)
				{
					ImGui::GetCurrentContext()->CurrentWindow->DC.CurrLineSize.y = height;			
				}
				
			}



	UiBuilder &UiBuilderImpl::MakeText(const char *text)
	{
		auto defaultSize{ ImGui::GetContentRegionAvail() };
		defaultSize.y = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y*2;

		auto usersDesiredSize{ defaultSize };
		ApplyUserSizing(usersDesiredSize.x, usersDesiredSize.y);
		
		ImVec2 wrappedSize{ ImGui::GetStyle().FramePadding*2 + ImGui::CalcTextSize(text, nullptr, false, usersDesiredSize.x) };		
		SetNextItemSize(wrappedSize.x, wrappedSize.y);				
		
		ImVec2 defaultPos{};
		ApplyUserPositioning(defaultPos.x, defaultPos.y);
		ApplyUserPivot(defaultPos.x, defaultPos.y, wrappedSize.x, wrappedSize.y);
				
		ImGui::SetCursorPos(ImGui::GetCursorPos() + defaultPos);

		
		ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset = std::clamp(ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset, ImGui::GetStyle().FramePadding.x, ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset);
		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + wrappedSize.x);
		ImGui::Text(text);
		ImGui::PopTextWrapPos();
		
		return *this;
	}


	
	struct ImGuiStringInputTargetAdapter
	{
		static inline StringInputTarget *inputTarget;
		static int InputTextCallback(ImGuiInputTextCallbackData *data)
		{
			int out{ 0 };
			
			if(data->Flags & ImGuiInputTextFlags_CallbackCharFilter)
			{
				if(inputTarget->CharacterIsForbidden(data->EventChar))
				{
					out = 1;					
				}
				inputTarget->NotifyContentChanged();
			}

			if(data->Flags & ImGuiInputTextFlags_CallbackResize)
			{
				inputTarget->Resize(data->BufTextLen);
			}
			
			return out;
		}
		
	};
	
	UiBuilder &UiBuilderImpl::MakeTextInput(StringInputTarget &target)
	{
		ApplyDimensionsForTextTypeElements(nullptr);		
		
		auto flags{ ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackCharFilter };
		if(target.IsReadOnly())
		{
			flags |= ImGuiInputTextFlags_ReadOnly;
		}
		
		ImGuiStringInputTargetAdapter::inputTarget = &target;		
		ImGui::InputText
		(
			userSettings.name.c_str(),
			target.GetBuffer(),
			target.GetCapacity(),
			flags,
			&ImGuiStringInputTargetAdapter::InputTextCallback
		);
		
		DoItemEpilogue();
		return *this;
		
	}



	UiBuilder &UiBuilderImpl::MakeCheckbox(bool *isChecked)
	{
		ImVec2 defaultSize{ ImGui::GetFrameHeight(), ImGui::GetFrameHeight() };
		ApplyUserSizing(defaultSize.x, defaultSize.y);
		SetNextItemSize(defaultSize.x, defaultSize.y);

		ImVec2 defaultPos{};
		ApplyUserPositioning(defaultPos.x, defaultPos.y);
		ApplyUserPivot(defaultPos.x, defaultPos.y, defaultSize.x, defaultSize.y);
		
		ImGui::SetCursorPos(ImGui::GetCursorPos() + defaultPos);

		
		ImGui::Checkbox(("##" + userSettings.name).c_str(), isChecked);

		DoItemEpilogue();
		return *this;
		
	}



	UiBuilder &UiBuilderImpl::MakeImageButton(const App::Core::ImageView &image, bool *isPressed)
	{		
		const ImVec2 defaultSize{ 50, 50 };
			
		auto usersDesiredSize{ defaultSize };
		ApplyUserSizing(usersDesiredSize.x, usersDesiredSize.y);						
		SetNextItemSize(usersDesiredSize.x, usersDesiredSize.y);				
		
		ImVec2 defaultPos{};
		ApplyUserPositioning(defaultPos.x, defaultPos.y);
		ApplyUserPivot(defaultPos.x, defaultPos.y, usersDesiredSize.x, usersDesiredSize.y);
				
		//drawing impl does not change the handle so we can cast from const
		const auto pressedResult{	ImGui::ImageButton(&const_cast<App::Core::ImageView &>(image).descriptorHandle, usersDesiredSize, {image.uvMinX, image.uvMinY}, {image.uvMaxX, image.uvMaxY}) };
		if(isPressed)
		{
			*isPressed = pressedResult;
		}

		return *this;
		
	}
	

	
	UiBuilder &UiBuilderImpl::MakeGrid(const size_t columns, const size_t rows)
	{
		gridData.columnCount = columns;
		gridData.rowCount = rows;

		ImVec2 defaultGridSize{ ImGui::GetContentRegionAvail() };
		ApplyUserSizing(defaultGridSize.x, defaultGridSize.y);

		ImVec2 defaultPos{};
		ApplyUserPositioning(defaultPos.x, defaultPos.y);
		ApplyUserPivot(defaultPos.x, defaultPos.y, defaultGridSize.x, defaultGridSize.y);

		ImGui::SetCursorPos(ImGui::GetCursorPos() + defaultPos);
		
		
		gridData.colWidth = defaultGridSize.x / columns;
		gridData.rowHeight = defaultGridSize.y / rows;

		gridData.cellPadding = userSettings.padding;
				
		ImGui::BeginChild(userSettings.name.c_str());
		desctructionFuncStack.push_front(&ImGui::EndChild);

		DoItemEpilogue();		
		return *this;
		
	}


	
	UiBuilder &UiBuilderImpl::MakeCell(const size_t startColIndex, const size_t startRowIndex, const size_t colSpan, const size_t rowSpan)
	{
		const ImVec2 cellOffset
		{
			gridData.colWidth * startColIndex + gridData.cellPadding,
			gridData.rowHeight * startRowIndex + gridData.cellPadding
		};
		ImGui::SetCursorPos(cellOffset);
		
		/*
		float spacing{ 0 };
	
		if (spacing < 0.0f) spacing = ImGui::GetStyle().ItemSpacing.x;
		window->DC.CursorPos.x = window->DC.CursorPosPrevLine.x + spacing;
		window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    
		window->DC.CurrLineSize = window->DC.PrevLineSize;
		window->DC.CurrLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
		*/
	
		const ImVec2 cellSize
		{
			gridData.colWidth * colSpan - gridData.cellPadding*2,
			gridData.rowHeight * rowSpan - gridData.cellPadding*2
		};
		
		ImGui::BeginChild
		(
			( "col"+std::to_string(startColIndex) + "_s"+std::to_string(colSpan)	+    "row"+std::to_string(startRowIndex) + "_s"+std::to_string(rowSpan) ).c_str(),
			cellSize
		);
		desctructionFuncStack.push_front(&ImGui::EndChild);

		DoItemEpilogue();
		return *this;
	
	}

	
}
