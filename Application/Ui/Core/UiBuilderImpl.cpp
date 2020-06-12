#include "Ui/Core/UiBuilderImpl.hpp"
#include "ThirdParty/imgui/imgui.h"
#include "Ui/ImguiTypeArithmetics.hpp"
#include <string>
#include "StringInputTarget.hpp"
#include "ThirdParty/imgui/imgui_internal.h"
#include "Shared/Exception/Exception.hpp"


namespace App::Ui::Core
{
	UiBuilder &UiBuilderImpl::DeclareSize(const float width, const float height)
	{		
		data.userSpecifiedSize.x = width;
		data.userSpecifiedSize.y = height;

		return *this;
				
	}


	
	UiBuilder &UiBuilderImpl::LeaveWidget()
	{
		(*desctructionFuncStack.front())();
		desctructionFuncStack.pop_front();

		return *this;
		
	}


	
	UiBuilder &UiBuilderImpl::DeclareName(const char *name)
	{
		data.name = name;
		return *this;
		
	}


	
	UiBuilder &UiBuilderImpl::DeclareAlignment(const float alignment)
	{
		data.alignment = alignment;			
		
		return *this;
		
	}


	
	UiBuilder &UiBuilderImpl::DeclareTabStatic()
	{
		data.flagsWindow |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		return *this;
		
	}



	UiBuilder &UiBuilderImpl::DeclareTabPos(const Math::Vector2 &relativePos, const Math::Vector2 &pivot)
	{
		data.relativePos = relativePos;
		data.pivot = pivot;
		return *this;
		
	}


	
	UiBuilder &UiBuilderImpl::DeclareTabNocollapse()
	{
		data.flagsWindow = ImGuiWindowFlags_NoCollapse;
		return *this;
		
	}



	UiBuilder &UiBuilderImpl::MakeTab(bool *isOpenTarget)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2{ data.alignment, 0.5 });		

		auto windowSize{ ImGui::GetWindowViewport()->Size };
		ApplyUserSizing(windowSize.x, windowSize.y);
		ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);

		auto windowPos{ ImGui::GetWindowViewport()->Size };
		windowPos.x *= data.relativePos.x;
		windowPos.y *= data.relativePos.y;

		windowPos.x -= windowSize.x * data.pivot.x;
		windowPos.y -= windowSize.y * data.pivot.y;
		
		ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once);
		
		ImGui::Begin(data.name.c_str(), isOpenTarget, data.flagsWindow);
		desctructionFuncStack.push_front(&ImGui::End);

		
		ImGui::PopStyleVar();

		data = defaults;
				
		return *this;
		
	}


	
	UiBuilder &UiBuilderImpl::MakeWrapper()
	{
		ImGui::Begin("Wrapper", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		desctructionFuncStack.push_front(&ImGui::End);

		return *this;
		
		
	}



	UiBuilder &UiBuilderImpl::MakeButton(bool *isPressed, const bool centerVertical)
	{
		if(data.userSpecifiedSize.x != 1)
		{		
			const auto buttonRect{ ImGui::GetStyle().FramePadding*2 + ImGui::CalcTextSize(data.name.c_str()) };
			auto offset{ ( ImGui::GetContentRegionAvail() - buttonRect) };

			offset.x *= data.alignment;
			offset.y = centerVertical ? offset.y * .5f : 0;
			
			ImGui::SetCursorPos(ImGui::GetCursorPos() + offset);
		}

		ImVec2 size{ 0, 0 };
		ApplyUserSizing(size.x, size.y);
		//SetNextItemSize(size.x, size.y);
		
		const auto pressedResult{ ImGui::Button(data.name.c_str(), size) };
		if(isPressed)
		{
			*isPressed = pressedResult;
		}

		data = defaults;
		
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
	
	UiBuilder &UiBuilderImpl::MakeTextInput(Core::StringInputTarget &target)
	{
		ImGuiStringInputTargetAdapter::inputTarget = &target;		
		CenterNextItem(ImGui::GetCurrentContext()->CurrentWindow->DC.ItemWidth);

		auto flags{ ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackCharFilter };
		if(target.IsReadOnly())
		{
			flags |= ImGuiInputTextFlags_ReadOnly;
		}

		ImVec2 defaultSize{ ImGui::GetCurrentContext()->CurrentWindow->DC.ItemWidth, ImGui::GetCurrentContext()->CurrentWindow->DC.CurrLineSize.y };
		ApplyUserSizing(defaultSize.x, defaultSize.y);
		SetNextItemSize(defaultSize.x, defaultSize.y);
		
		ImGui::InputText
		(
			data.name.c_str(),
			target.GetBuffer(),
			target.GetCapacity(),
			flags,
			&ImGuiStringInputTargetAdapter::InputTextCallback
		);
		
		data = defaults;				
		return *this;
		
	}


	UiBuilder &UiBuilderImpl::MakeGrid(size_t columns, size_t rows)
	{
		gridData.columnCount = columns;
		gridData.rowCount = rows;

		ImVec2 gridSize{ ImGui::GetContentRegionAvail() };
		ApplyUserSizing(gridSize.x, gridSize.y);
			   		
		gridData.colWidth = gridSize.x / columns;
		gridData.rowHeight = gridSize.y / rows;
				
		ImGui::BeginChild(data.name.c_str());
		desctructionFuncStack.push_front(&ImGui::EndChild);

		data = defaults;
		
		return *this;
		
	}

	UiBuilder &UiBuilderImpl::MakeCell(size_t startColIndex, size_t startRowIndex, size_t colSpan, size_t rowSpan)
	{
		const ImVec2 cellOffset
		{
			gridData.colWidth * startColIndex,
			gridData.rowHeight * startRowIndex
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
			gridData.colWidth * colSpan,
			gridData.rowHeight * rowSpan
		};
		
		ImGui::BeginChild
		(
			("col" + std::to_string(startColIndex+gridData.columnCount+colSpan) + "row" + std::to_string(startRowIndex+gridData.rowCount+rowSpan)).c_str(),
			cellSize
		);
		desctructionFuncStack.push_front(&ImGui::EndChild);
	
		return *this;
	
	}

	
	UiBuilder &UiBuilderImpl::MakeModal()
	{		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2{ data.alignment, 0.5 });		

		ImVec2 windowSize{ ImGui::GetWindowViewport()->Size };
		ApplyUserSizing(windowSize.x, windowSize.y);
		
		ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);

		ImGui::OpenPopup(data.name.c_str());//ensure that begin always returns true. if you dont want to display the modal just dont call this function
		ImGui::BeginPopupModal(data.name.c_str(), nullptr, data.flagsWindow | ImGuiWindowFlags_NoResize);
		desctructionFuncStack.push_front(&ImGui::EndPopup);

		ImGui::PopStyleVar();

		return *this;
		
	}

	UiBuilder& UiBuilderImpl::MakeText(const char* text)
	{
		ImGui::Text(text);
		return *this;
	}

	UiBuilder& UiBuilderImpl::MakeCheckbox(bool* isChecked)
	{
		CenterNextItem(ImGui::GetFrameHeight());		
		
		ImGui::Checkbox(("##" + data.name).c_str(), isChecked);
		data = defaults;
		return *this;
		
	}

		void UiBuilderImpl::CenterNextItem(const float nextItemWidth) const
		{
			auto availableWidth{ (ImGui::GetContentRegionAvail().x - nextItemWidth) };
			if(availableWidth < 0)
			{
				availableWidth = 0;
			}
		
			ImGui::SetCursorPosX(ImGui::GetCursorPos().x + availableWidth * data.alignment);
			
		}

	void UiBuilderImpl::ApplyUserSizing(float &width, float &height) const
	{
		if(data.userSpecifiedSize.x > 0)
		{
			width = data.userSpecifiedSize.x <= 1 ? width * data.userSpecifiedSize.x : data.userSpecifiedSize.x;			
		}

		if(data.userSpecifiedSize.y > 0)
		{
			height = data.userSpecifiedSize.y <= 1 ? height * data.userSpecifiedSize.y : data.userSpecifiedSize.y;			
		}
					   		
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
	
}
