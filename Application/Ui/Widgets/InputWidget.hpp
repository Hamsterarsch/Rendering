#pragma once
#include "Ui/Core/WidgetBase.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/StringInputTarget.hpp"
#include <string>
#include "Ui/Misc/AssetNameFilter.hpp"


namespace App::Ui::Widgets
{
	class InputWidgetBase : public WidgetBase
	{
		private: std::string name;

		private: bool isReadOnly;


		
		public: InputWidgetBase(const char *name) : name{ name }, isReadOnly{ false } {}

		
		public: const char *GetName() const { return name.c_str(); }

		public: bool IsReadOnly() const { return isReadOnly; }
		
	};


	
	
	template<class t_input>
	class InputWidget final : public InputWidgetBase
	{
		static_assert("InputWidget is not specialized for this input type");
	};

	
	
	
	template<>
	class InputWidget<StringInputTarget> final : public InputWidgetBase
	{		
		private: StringInputTarget *target;

		public: float alignment;
		

		
		public: InputWidget(const char *name, StringInputTarget &target) : InputWidgetBase{ name }, target{ &target }, alignment{ .5 } {}
		
		
		public: void RenderInternal(UiBuilder &builder) override;

		
	};

	inline void InputWidget<StringInputTarget>::RenderInternal(UiBuilder &builder)
	{
		builder
		.DeclareAlignment(alignment)
		.MakeTextInput(*target);
	
	}



	
	template<>
	class InputWidget<bool> final : public InputWidgetBase
	{
		private: bool *target;

		public: float alignment;

				
		
		public: InputWidget(const char *name, bool &target) : InputWidgetBase{ name }, target{ &target }, alignment{ .5 }  {}
		
		public: void RenderInternal(UiBuilder &builder) override;



		
	};

	inline void InputWidget<bool>::RenderInternal(UiBuilder &builder)
	{
		builder
		.DeclareName(GetName())
		.DeclareAlignment(alignment)
		.MakeCheckbox(target);
		
	}
	
	


	class PropertyWidget final : public WidgetBase
	{		
		public: void RenderInternal(UiBuilder &builder) override;
		
	};

	inline void PropertyWidget::RenderInternal(UiBuilder &builder)
	{
		builder
		.MakeGrid(4, 1)
		.MakeCell(0, 0);
		
		for(auto &&child : children)
		{
			auto *asInputWidget{ static_cast<InputWidgetBase *>(child.get()) };
			builder.MakeText(asInputWidget->GetName());				
		}

		builder
		.LeaveWidget()
		.MakeCell(1,0, 3);

		for(auto &&child : children)
		{
			child->Render(builder);
		}
		
		builder
		.LeaveWidget()
		.LeaveWidget();
		
	}

	
}
