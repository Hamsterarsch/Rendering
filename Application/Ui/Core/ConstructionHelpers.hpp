#pragma once
#include "Shared/PtrTypes.hpp"
#include "UiLayoutElement.hpp"


namespace App::Ui
{
	//legace children operators todo: replace and remove
	template<class t>
	auto operator+=(UniquePtr<t> &instance, UniquePtr<Core::UiElement> &&element) -> UniquePtr<t> &
	{
		static_assert(std::is_base_of_v<Core::UiLayoutElement, t>, "UiElement children can only be added to subclasses of UiLayoutElement");
		
		instance->AddChild(std::move(element));
		return instance;
				
	}

	//legace children operators todo: replace and remove
	template<class t>
	auto operator+=(UniquePtr<t> &&instance, UniquePtr<Core::UiElement> &&element) -> UniquePtr<t>
	{
		static_assert(std::is_base_of_v<Core::UiLayoutElement, t>, "UiElement children can only be added to subclasses of UiLayoutElement");
		
		instance->AddChild(std::move(element));
		return std::move(instance);
				
	}


	
	template<class t>
	auto operator<<(UniquePtr<t> &instance, UniquePtr<Core::UiElement> &&element) -> UniquePtr<t> &
	{
		static_assert(std::is_base_of_v<Core::UiLayoutElement, t>, "UiElement children can only be added to subclasses of UiLayoutElement");
		
		instance->AddChild(std::move(element));
		return instance;
				
	}

	template<class t>
	auto operator<<(UniquePtr<t> &&instance, UniquePtr<Core::UiElement> &&element) -> UniquePtr<t>
	{
		static_assert(std::is_base_of_v<Core::UiLayoutElement, t>, "UiElement children can only be added to subclasses of UiLayoutElement");
		
		instance->AddChild(std::move(element));
		return std::move(instance);
				
	}

	
	
	template<class t, class ...t_args> auto Element(t_args &&... args) -> UniquePtr<t>
	{
		static_assert(std::is_base_of_v<Core::UiElement, t>, "Only subclasses of UiElement can be constructed with Element()");
		
		return MakeUnique<t>(std::forward<t_args>(args)...);
		
	}

	
	
	template<class t, class t_property>
	struct Set
	{
		t_property t::*property;
		t_property value;

		Set(t_property t::*property, t_property &&value) : property{ property }, value{ std::forward<t_property>(value) } {}
				
	};
	template<class t, class t_property> Set(t_property t::*, t_property &&) -> Set<t, t_property>;

	
	
	template<class t, class t_propertyObject, class t_property>
	auto operator->*(UniquePtr<t> &&instance, Set<t_propertyObject, t_property> &&setInfo) -> UniquePtr<t>
	{
		static_assert(std::is_base_of_v<t_propertyObject, t>, "The specified property pointer was not of object type");
		
		*instance.*setInfo.property = std::forward<t_property>(setInfo.value);
		return std::move(instance);
		
	}
	   	
	template<class t, class t_propertyObject, class t_property>
	auto operator->*(UniquePtr<t> &instance, Set<t_propertyObject, t_property> &&setInfo) -> UniquePtr<t> &
	{
		static_assert(std::is_base_of_v<t_propertyObject, t>, "The specified property pointer was not of object type");
		
		*instance.*setInfo.property = std::forward<t_property>(setInfo.value);
		return instance;
		
	}

	
}
