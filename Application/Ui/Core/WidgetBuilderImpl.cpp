#include "Ui/Core/WidgetBuilderImpl.hpp"
#include "ThirdParty/imgui/imgui.h"
#include "Ui/ImguiTypeArithmetics.hpp"
#include <string>
#include "StringInputTarget.hpp"
#include "ThirdParty/imgui/imgui_internal.h"


namespace App::Ui
{
	UiBuilder &WidgetBuilderImpl::DeclareAutoWidth()
	{		
		ImGui::SetNextItemWidth( ImGui::GetContentRegionAvail().x );		

		return *this;
				
	}


	
	UiBuilder &WidgetBuilderImpl::LeaveWidget()
	{
		(*desctructionFuncStack.front())();
		desctructionFuncStack.pop_front();

		return *this;
		
	}


	
	UiBuilder &WidgetBuilderImpl::DeclareName(const char *name)
	{
		data.name = name;
		return *this;
		
	}


	
	UiBuilder &WidgetBuilderImpl::DeclareAlignment(const float alignment)
	{
		data.alignment = alignment;
		return *this;
		
	}


	
	UiBuilder &WidgetBuilderImpl::DeclareTabStatic()
	{
		data.flagsWindow |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		return *this;
		
	}


	
	UiBuilder &WidgetBuilderImpl::DeclareTabSize(const Math::Vector2 &relativeSize)
	{
		data.relativeSize = relativeSize;
		return *this;
		
	}


	
	UiBuilder &WidgetBuilderImpl::DeclareTabPos(const Math::Vector2 &relativePos, const Math::Vector2 &pivot)
	{
		data.relativePos = relativePos;
		data.pivot = pivot;
		return *this;
		
	}


	
	UiBuilder &WidgetBuilderImpl::DeclareTabNocollapse()
	{
		data.flagsWindow = ImGuiWindowFlags_NoCollapse;
		return *this;
		
	}



	UiBuilder &WidgetBuilderImpl::MakeTab()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2{ data.alignment, 0.5 });		

		auto windowSize{ ImGui::GetWindowViewport()->Size };
		windowSize.x *= data.relativeSize.x;
		windowSize.y *= data.relativeSize.y;
		ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);

		auto windowPos{ ImGui::GetWindowViewport()->Size };
		windowPos.x *= data.relativePos.x;
		windowPos.y *= data.relativePos.y;

		windowPos.x -= windowSize.x * data.pivot.x;
		windowPos.y -= windowSize.y * data.pivot.y;
		
		ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once);
		
		ImGui::Begin(data.name.c_str(), nullptr, data.flagsWindow);
		desctructionFuncStack.push_front(&ImGui::End);

		
		ImGui::PopStyleVar();

		data = defaults;
				
		return *this;
		
	}


	
	UiBuilder &WidgetBuilderImpl::MakeWrapper()
	{
		ImGui::Begin("Wrapper", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		desctructionFuncStack.push_front(&ImGui::End);

		return *this;
		
		
	}



	UiBuilder &WidgetBuilderImpl::MakeButton(bool *isPressed, const bool centerVertical)
	{
		const auto buttonRect{ ImGui::GetStyle().FramePadding*2 + ImGui::CalcTextSize(data.name.c_str()) };
		auto offset{ ( ImGui::GetContentRegionAvail() - buttonRect) };

		offset.x *= data.alignment;
		offset.y = centerVertical ? offset.y * .5f : 0;
		
		ImGui::SetCursorPos(ImGui::GetCursorPos() + offset);
				
		const auto pressedResult{ ImGui::Button(data.name.c_str()) };
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
	
	UiBuilder &WidgetBuilderImpl::MakeTextInput(StringInputTarget &target)
	{
		ImGuiStringInputTargetAdapter::inputTarget = &target;		
		CenterNextItem(ImGui::GetCurrentContext()->CurrentWindow->DC.ItemWidth);

		auto flags{ ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackCharFilter };
		if(target.IsReadOnly())
		{
			flags |= ImGuiInputTextFlags_ReadOnly;
		}
		
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


	UiBuilder &WidgetBuilderImpl::MakeGrid(size_t columns, size_t rows)
	{
		gridData.columnCount = columns;
		gridData.rowCount = rows;

		const auto query{  ImGui::GetContentRegionAvail() };
		gridData.colWidth = query.x / columns;
		gridData.rowHeight = query.y / rows;
				
		ImGui::BeginChild(data.name.c_str());
		desctructionFuncStack.push_front(&ImGui::EndChild);

		data = defaults;
		
		return *this;
		
	}

	UiBuilder &WidgetBuilderImpl::MakeCell(size_t startColIndex, size_t startRowIndex, size_t colSpan, size_t rowSpan)
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

	
	UiBuilder& WidgetBuilderImpl::MakeModal(bool* isOpen)
	{		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2{ data.alignment, 0.5 });		

		auto windowSize{ ImGui::GetWindowViewport()->Size };
		windowSize.x *= data.relativeSize.x;
		windowSize.y *= data.relativeSize.y;
		ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);

		ImGui::OpenPopup(data.name.c_str());//ensure that begin always returns true. if you dont want to display the modal just dont call this function
		ImGui::BeginPopupModal(data.name.c_str(), isOpen);
		desctructionFuncStack.push_front(&ImGui::EndPopup);

		ImGui::PopStyleVar();

		return *this;
		
	}

	UiBuilder& WidgetBuilderImpl::MakeText(const char* text)
	{
		ImGui::Text(text);
		return *this;
	}

	UiBuilder& WidgetBuilderImpl::MakeCheckbox(bool* isChecked)
	{
		CenterNextItem(ImGui::GetFrameHeight());		
		
		ImGui::Checkbox(("##" + data.name).c_str(), isChecked);
		data = defaults;
		return *this;
		
	}

		void WidgetBuilderImpl::CenterNextItem(const float nextItemWidth) const
		{
			auto availableWidth{ (ImGui::GetContentRegionAvail().x - nextItemWidth) };
			if(availableWidth < 0)
			{
				availableWidth = 0;
			}
		
			ImGui::SetCursorPosX(ImGui::GetCursorPos().x + availableWidth * data.alignment);
			
		}
	
}
