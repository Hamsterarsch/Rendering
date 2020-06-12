#pragma once
#include "Ui/Core/UiAccessElement.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/StringInputTarget.hpp"
#include <string>
#include "Ui/Core/UiFrontend.hpp"



namespace App::Ui
{
	class InputElementBase : public Core::UiAccessElement
	{
		private: std::string name;

		private: bool isReadOnly;

		protected: using t_frontend_param = Core::UiFrontend &;


		
		public: InputElementBase(t_frontend_param frontend, const char *name)
			:
			UiAccessElement{ frontend },
			name{ name },
			isReadOnly{ false }
		{}

		
		public: const char *GetName() const { return name.c_str(); }

		public: bool IsReadOnly() const { return isReadOnly; }
						
	};


	
	
	template<class t_input>
	class InputElement final : public InputElementBase
	{
		static_assert("InputWidget is not specialized for this input type");
	};

	
	
	
	template<>
	class InputElement<Core::StringInputTarget> final : public InputElementBase
	{		
		private: size_t targetIndex;

		
		
		public: InputElement(t_frontend_param frontend, size_t targetIndex, const char *name)
			:
			InputElementBase{ frontend, name },
			targetIndex{ targetIndex }
		{}

		
		void RenderAndQueryInternal(Core::UiBuilder &builder) override;

	};

	inline void InputElement<Core::StringInputTarget>::RenderAndQueryInternal(Core::UiBuilder &builder)
	{		
		builder		
		.MakeTextInput(*GetFrontend().GetInputTargetString(targetIndex));
	
	}



	
	template<>
	class InputElement<bool> final : public InputElementBase
	{
		private: size_t targetIndex;

				 				
		
		public: InputElement(t_frontend_param frontend, size_t targetIndex, const char *name)
			:
			InputElementBase{ frontend, name },
			targetIndex{ targetIndex }
		{}


		void RenderAndQueryInternal(Core::UiBuilder &builder) override;

	};

	inline void InputElement<bool>::RenderAndQueryInternal(Core::UiBuilder &builder)
	{
		builder
		.DeclareName(GetName())		
		.MakeCheckbox(GetFrontend().GetInputTargetBool(targetIndex));
		
	}
	
	
}
