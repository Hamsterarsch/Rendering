#pragma once
#include "Shared/PtrTypes.hpp"
#include "UiElement.hpp"


namespace App::Ui
{	
	template<class t>
	auto operator+=(UniquePtr<t> &instance, UniquePtr<Core::UiElement> &&element) -> decltype( std::enable_if_t<std::is_base_of_v<Core::UiLayoutElement, t>>(), std::declval<UniquePtr<t> &>() )
	{
		instance->AddChild(std::move(element));
		return instance;
				
	}

	template<class t>
	auto operator+=(UniquePtr<t> &&instance, UniquePtr<Core::UiElement> &&element) -> decltype( std::enable_if_t<std::is_base_of_v<Core::UiLayoutElement, t>>(), std::declval<UniquePtr<t>>() )
	{
		instance->AddChild(std::move(element));
		return std::move(instance);
				
	}

	template<class t>
	auto operator<<(UniquePtr<t> &instance, UniquePtr<Core::UiElement> &&element) -> decltype( std::enable_if_t<std::is_base_of_v<Core::UiLayoutElement, t>>(), std::declval<UniquePtr<t> &>() )
	{
		instance->AddChild(std::move(element));
		return instance;
				
	}

	template<class t>
	auto operator<<(UniquePtr<t> &&instance, UniquePtr<Core::UiElement> &&element) -> decltype( std::enable_if_t<std::is_base_of_v<Core::UiLayoutElement, t>>(), std::declval<UniquePtr<t>>() )
	{
		instance->AddChild(std::move(element));
		return std::move(instance);
				
	}

	
	
	template<class t, class ...t_args> auto Element(t_args &&... args) -> decltype( std::enable_if_t<std::is_base_of_v<Core::UiElement, t>>(), UniquePtr<t>() )
	{		
		return MakeUnique<t>(std::forward<t_args>(args)...);
		
	}

	
	
	template<class t, class t_property>
	struct Set
	{
		t_property t::*property;
		t_property value;

		Set(t_property t::*property, t_property &&value) : property{ property }, value{ std::move(value) } {}
		
		
	};
	template<class t, class t_property> Set(t_property t::*, const t_property &) -> Set<t, t_property>;

	
	
	template<class t, class t_propertyObject, class t_property>
	auto operator->*(UniquePtr<t> &&instance, Set<t_propertyObject, t_property> &&setInfo) -> decltype( std::enable_if_t<std::is_base_of_v<t_propertyObject, t>>(), UniquePtr<t>() )
	{
		*instance.*setInfo.property = std::move(setInfo.value);
		return std::move(instance);
		
	}
	   	
	template<class t, class t_propertyObject, class t_property>
	auto operator->*(UniquePtr<t> &instance, Set<t_propertyObject, t_property> &&setInfo) -> decltype( std::enable_if_t<std::is_base_of_v<t_propertyObject, t>>(), std::add_lvalue_reference_t<UniquePtr<t>>() )
	{
		*instance.*setInfo.property = std::move(setInfo.value);
		return instance;
		
	}

	
}
