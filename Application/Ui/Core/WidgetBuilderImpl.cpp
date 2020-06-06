#include "Ui/Core/WidgetBuilderImpl.hpp"
#include "ThirdParty/imgui/imgui.h"
#include "Ui/ImguiTypeArithmetics.hpp"
#include "ThirdParty/imgui/imgui_internal.h"
#include <string>


namespace App::Ui
{
	WidgetBuilder &WidgetBuilderImpl::DeclareAutoWidth()
	{		
		ImGui::SetNextItemWidth( ImGui::GetContentRegionAvail().x );		

		return *this;
				
	}


	
	WidgetBuilder &WidgetBuilderImpl::LeaveWidget()
	{
		(*desctructionFuncStack.front())();
		desctructionFuncStack.pop_front();

		return *this;
		
	}


	
	WidgetBuilder &WidgetBuilderImpl::DeclareName(const char *name)
	{
		data.name = name;
		return *this;
		
	}


	
	WidgetBuilder &WidgetBuilderImpl::DeclareAlignment(const float alignment)
	{
		data.alignment = alignment;
		return *this;
		
	}


	
	WidgetBuilder &WidgetBuilderImpl::DeclareTabStatic()
	{
		data.flagsWindow |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		return *this;
		
	}


	
	WidgetBuilder &WidgetBuilderImpl::DeclareTabSize(const Math::Vector2 &relativeSize)
	{
		data.relativeSize = relativeSize;
		return *this;
		
	}


	
	WidgetBuilder &WidgetBuilderImpl::DeclareTabPos(const Math::Vector2 &relativePos, const Math::Vector2 &pivot)
	{
		data.relativePos = relativePos;
		data.pivot = pivot;
		return *this;
		
	}


	
	WidgetBuilder &WidgetBuilderImpl::DeclareTabNocollapse()
	{
		data.flagsWindow = ImGuiWindowFlags_NoCollapse;
		return *this;
		
	}



	WidgetBuilder &WidgetBuilderImpl::MakeTab()
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


	
	WidgetBuilder &WidgetBuilderImpl::MakeWrapper()
	{
		ImGui::Begin("Wrapper", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		desctructionFuncStack.push_front(&ImGui::End);

		return *this;
		
		
	}



	WidgetBuilder &WidgetBuilderImpl::MakeButton(bool *isPressed, const bool centerVertical)
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
	

	WidgetBuilder &WidgetBuilderImpl::MakeGrid(size_t columns, size_t rows)
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

		WidgetBuilder &WidgetBuilderImpl::MakeCell(size_t startColIndex, size_t startRowIndex, size_t colSpan, size_t rowSpan)
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


	
}
