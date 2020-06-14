#pragma once
#include "Ui/Core/UiLayoutElement.hpp"


#include "Ui/Core/UiBuilder.hpp"

namespace App::Ui
{
	class EqualColumnsLayout final : public Core::UiLayoutElement
	{
		private: static inline unsigned constructedInstances{ 0 };

		private: std::string name;

		private: float columPadding;


		
		public: EqualColumnsLayout(const float columnPadding = 0) : name{ "EqualGrid_" + std::to_string(constructedInstances) }, columPadding{ columnPadding }
		{			
			++constructedInstances;

		}
				
		protected: void OnPreRenderAndQueryChildren(Core::UiBuilder &builder) override
		{
			builder
			.DeclareName(name.c_str())
			.MakeGrid(GetChildCount(), 1);
			
		}

		protected: void OnPreRenderAndQueryChild(Core::UiBuilder &builder, size_t childIndex, UiElement &child) override
		{
			builder.DeclarePadding(columPadding).MakeCell(childIndex, 0, 1, 1);
			
		}

		protected: void OnPostRenderAndQueryChild(Core::UiBuilder &builder, size_t childIndex, UiElement &child) override
		{
			builder.LeaveWidget();
			
		}

		protected: void OnPostRenderAndQueryChildren(Core::UiBuilder &builder) override
		{
			builder.LeaveWidget();
			
		}
		
	};

	
}
