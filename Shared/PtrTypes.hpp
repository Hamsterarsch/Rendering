#pragma once
#include <memory>


template<class t, class t_deleter = std::default_delete<t>>
using UniquePtr = std::unique_ptr<t, t_deleter>;


template<class t>
using SharedPtr = std::shared_ptr<t>;



template<class t, class ...t_args>
UniquePtr<t> MakeUnique(t_args &&... args)
{
	return std::make_unique<t>(std::forward<t_args>(args)...);
	
}



template<class t, class ...t_args>
SharedPtr<t> MakeShared(t_args &&... args)
{
	return std::make_shared<t>(std::forward<t_args>(args)...);
		
}

