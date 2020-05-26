#pragma once
#include <memory>

template<class T, class D = std::default_delete<T>>
using UniquePtr = std::unique_ptr<T, D>;

template<class T>
using SharedPtr = std::shared_ptr<T>;

template<class T, class ...Args>
UniquePtr<T> MakeUnique(Args &&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
	
}

