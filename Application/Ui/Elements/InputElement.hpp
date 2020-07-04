#pragma once
#include "Ui/Core/UiAccessElement.hpp"
#include "Ui/Core/UiBuilder.hpp"
#include "Ui/Core/StringInputTarget.hpp"
#include <string>
#include "Ui/Core/UiFrontend.hpp"



namespace App::Ui
{
	template<class t_input>
	class InputElement{ static_assert("InputWidget is not specialized for this input type"); };
	
	class InputElementBase : public Core::UiAccessElement
	{
		private: std::string name;

		private: bool isReadOnly;

		private: size_t targetIndex;
		
		protected: using t_frontend_param = Core::UiFrontend &;


		
		public: InputElementBase(t_frontend_param frontend, size_t targetIndex, const char *name)
			:
			UiAccessElement{ frontend },
			name{ name },
			isReadOnly{ false },
			targetIndex{ targetIndex }
		{}

		
		public: const char *GetName() const { return name.c_str(); }

		public: size_t GetIndex() const { return targetIndex; }
		
		public: bool IsReadOnly() const { return isReadOnly; }
						
	};
	   	
		
	
	
	template<>
	class InputElement<Core::StringInputTarget> final : public InputElementBase
	{	
		private: bool isMultiline;
		

		
		public: InputElement(t_frontend_param frontend, size_t targetIndex, const char *name)
			: InputElement{ frontend, targetIndex, false, name }
		{}
		
		public: InputElement(t_frontend_param frontend, size_t targetIndex, bool isMultiline, const char *name)
			:
			InputElementBase{ frontend, targetIndex, name },
			isMultiline{ isMultiline }
		{}

		
		protected: void RenderAndQueryInternal(Core::UiBuilder &builder) override;

	};

	inline void InputElement<Core::StringInputTarget>::RenderAndQueryInternal(Core::UiBuilder &builder)
	{		
		builder		
		.MakeTextInput(*GetFrontend().GetInputTargetString(GetIndex()), isMultiline);
	
	}



	
	template<>
	class InputElement<bool> final : public InputElementBase
	{
		private: inline static size_t serial{ 0 };
		
		public: InputElement(t_frontend_param frontend, size_t targetIndex)
			:
			InputElementBase{ frontend, targetIndex, ("inputBool"+std::to_string(++serial)).c_str() }
		{}


		protected: void RenderAndQueryInternal(Core::UiBuilder &builder) override;

	};

	inline void InputElement<bool>::RenderAndQueryInternal(Core::UiBuilder &builder)
	{
		builder
		.DeclareName(GetName())		
		.MakeCheckbox(GetFrontend().GetInputTargetBool(GetIndex()));
		
	}




	template<>
	class InputElement<float> final : public InputElementBase
	{
		private: inline static size_t serial{ 0 };
		
		public: bool isReadOnly;

		
		public: InputElement(t_frontend_param frontend, size_t targetIndex) : InputElementBase{ frontend, targetIndex, ("inputFloat"+std::to_string(++serial)).c_str() }, isReadOnly{ false } {}
		
		protected: void RenderAndQueryInternal(Core::UiBuilder &builder) override;
		
	};

	inline void InputElement<float>::RenderAndQueryInternal(Core::UiBuilder &builder)
	{
		if(auto *target{ GetFrontend().GetInputTargetFloat(GetIndex()) })
		{
			if(isReadOnly)
			{
				builder.DeclareReadonlyInput();
			}
			
			builder.DeclareName(GetName()).MakeInputFloat(*target);			
		}
		
	}




	template<>
	class InputElement<float[4]> final : public InputElementBase
	{
		private: inline static size_t serial{ 0 };
		
		public: bool isReadOnly;

		
		public: InputElement(t_frontend_param frontend, size_t targetIndex) : InputElementBase{ frontend, targetIndex, ("inputFloat4"+std::to_string(++serial)).c_str() }, isReadOnly{ false } {}
		
		protected: void RenderAndQueryInternal(Core::UiBuilder &builder) override;
		
	};

	inline void InputElement<float[4]>::RenderAndQueryInternal(Core::UiBuilder &builder)
	{
		if(auto *target{ GetFrontend().GetInputTargetFloat(GetIndex()) })
		{
			if(isReadOnly)
			{
				builder.DeclareReadonlyInput();
			}
			
			builder.DeclareName(GetName()).MakeInputFloat4(target);			
		}
		
	}


	
	template<>
	class InputElement<int> final : public InputElementBase
	{
		private: inline static size_t serial{ 0 };
		
		public: bool isReadOnly;

		
		public: InputElement(t_frontend_param frontend, size_t targetIndex) : InputElementBase{ frontend, targetIndex, ("intInput"+std::to_string(++serial)).c_str() }, isReadOnly{ false } {}
		
		protected: void RenderAndQueryInternal(Core::UiBuilder &builder) override;
		
	};

	inline void InputElement<int>::RenderAndQueryInternal(Core::UiBuilder &builder)
	{
		if(auto *target{ GetFrontend().GetInputTargetInt(GetIndex()) })
		{
			if(isReadOnly)
			{
				builder.DeclareReadonlyInput();
			}
			
			builder.DeclareName(GetName()).MakeInputInt(*target);			
		}
		
	}




	template<>
	class InputElement<unsigned> final : public InputElementBase
	{
		private: inline static size_t serial{ 0 };
		
		public: bool isReadOnly;

		
		public: InputElement(t_frontend_param frontend, size_t targetIndex) : InputElementBase{ frontend, targetIndex, ("uintInput"+std::to_string(++serial)).c_str() }, isReadOnly{ false } {}
		
		protected: void RenderAndQueryInternal(Core::UiBuilder &builder) override;
		
	};

	inline void InputElement<unsigned>::RenderAndQueryInternal(Core::UiBuilder &builder)
	{
		if(auto *target{ GetFrontend().GetInputTargetUnsigned(GetIndex()) })
		{
			if(isReadOnly)
			{
				builder.DeclareReadonlyInput();
			}
			
			builder.DeclareName(GetName()).MakeInputUnsigned(*target);			
		}
		
	}

	
}
