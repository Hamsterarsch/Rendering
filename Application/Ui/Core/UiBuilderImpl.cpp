#include "Ui/Core/UiBuilderImpl.hpp"
#include "ThirdParty/imgui/imgui.h"
#include "Ui/ImguiTypeArithmetics.hpp"
#include <string>
#include <algorithm>
#include "StringInputTarget.hpp"
#include "ThirdParty/imgui/imgui_internal.h"
#include "Shared/Exception/Exception.hpp"


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
			ImGui::SetNextWindowSize(defaultWindowSize, ImGuiCond_Once);

			auto defaultWindowPos{ ImGui::GetWindowViewport()->Size * .5 };
			ApplyUserPositioning(defaultWindowPos.x, defaultWindowPos.y, true);		
			ImGui::SetNextWindowPos(defaultWindowPos, ImGuiCond_Once, {userSettings.pivot.x, userSettings.pivot.y});
			
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
		
		ImGui::OpenPopup(userSettings.name.c_str());//ensure that begin always returns true. if you dont want to display the modal just dont call this function
		ImGui::BeginPopupModal(userSettings.name.c_str(), nullptr, userSettings.flagsWindow | ImGuiWindowFlags_NoResize);
		desctructionFuncStack.push_front(&ImGui::EndPopup);

		ImGui::PopStyleVar();

		DoItemEpilogue();
		return *this;
		
	}

	
	
	UiBuilder &UiBuilderImpl::MakeButton(bool *isPressed)
	{	
		const auto size{ ApplyDimensionsForTextTypeElements(userSettings.name.c_str()) };
		
		const auto pressedResult{ ImGui::Button(userSettings.name.c_str(), size) }; //todo: check if apply for text elements changes button size
		if(isPressed)
		{
			*isPressed = pressedResult;
		}

		DoItemEpilogue();		
		return *this;
		
	}

		ImVec2 UiBuilderImpl::ApplyDimensionsForTextTypeElements(const char *text) const
		{
			ImVec2 size{ ImGui::GetStyle().FramePadding*2 + ImGui::CalcTextSize(text) };
				
			ApplyUserSizing(size.x, size.y);

			SetNextItemSize(size.x, size.y);

		
			ImVec2 defaultPos{};
			ApplyUserPositioning(defaultPos.x, defaultPos.y);
			ApplyUserPivot(defaultPos.x, defaultPos.y, size.x, size.y);
					
			ImGui::SetCursorPos(ImGui::GetCursorPos() + defaultPos);

			return size;
		
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
		ApplyDimensionsForTextTypeElements(text);

		ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset = std::clamp(ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset, ImGui::GetStyle().FramePadding.x, ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset);
		ImGui::Text(text);
		
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
		ApplyDimensionsForTextTypeElements(" ");		
		
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
			("col" + std::to_string(startColIndex+gridData.columnCount+colSpan) + "row" + std::to_string(startRowIndex+gridData.rowCount+rowSpan)).c_str(),
			cellSize
		);
		desctructionFuncStack.push_front(&ImGui::EndChild);

		DoItemEpilogue();
		return *this;
	
	}

	
}
