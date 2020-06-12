#pragma once
#include "Shared/PtrTypes.hpp"
#include "UiElement.hpp"


namespace App::Ui
{
	template<class t>
	auto operator+=(UniquePtr<t> &instance, UniquePtr<Core::UiDecorator> &&decorator) -> decltype( std::enable_if_t<std::is_base_of_v<Core::UiElement, t>>(), std::declval<UniquePtr<t> &>() )
	{
		instance->AddDecorator(std::move(decorator));
		return instance;
				
	}

	template<class t>
	auto operator+=(UniquePtr<t> &&instance, UniquePtr<Core::UiDecorator> &&decorator) -> decltype( std::enable_if_t<std::is_base_of_v<Core::UiElement, t>>(), std::declval<UniquePtr<t>>() )
	{
		instance->AddDecorator(std::move(decorator));
		return std::move(instance);
				
	}
	/*
	template<class t>
	auto operator+=(t &instance, UniquePtr<Core::UiDecorator> &&decorator) -> decltype( std::enable_if_t<std::is_base_of_v<Core::UiElement, t>>(), std::declval<t &>() )
	{
		instance.AddDecorator(std::move(decorator));
		return instance;
		
	}
	*/

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
	/*
	template<class t>
	auto operator+=(t &instance, UniquePtr<Core::UiElement> &&element) -> decltype( std::enable_if_t<std::is_base_of_v<Core::UiLayoutElement, t>>(), std::declval<t &>() )
	{
		instance.AddChild(std::move(element));
		return instance;
		
	}*/
	

	
	template<class t, class ...t_args> auto Decorator(t_args &&... args) -> decltype( std::enable_if_t<std::is_base_of_v<Core::UiDecorator, t>>(), UniquePtr<t>() )
	{
		return MakeUnique<t>(std::forward<t_args>(args)...);
		
	}

	template<class t, class ...t_args> auto Element(t_args &&... args) -> decltype( std::enable_if_t<std::is_base_of_v<Core::UiElement, t>>(), UniquePtr<t>() )
	{		
		return MakeUnique<t>(std::forward<t_args>(args)...);
		
	}
	
}
