

namespace Math
{
	template <class t_derivedClass, class t_glm>
	constexpr auto VectorBase<t_derivedClass, t_glm>::operator+=(const t_target& other) -> t_target&
	{		
		static_cast<t_derivedClass *>(this)->_internal += other._internal;
		return *static_cast<t_derivedClass *>(this);
		
	}


	
	template <class t_derivedClass, class t_glm>
	constexpr auto VectorBase<t_derivedClass, t_glm>::operator-=(const t_target& other) -> t_target&
	{		
		static_cast<t_derivedClass *>(this)->_internal-= other._internal;
		return *static_cast<t_derivedClass *>(this);
		
	}





	
	template <class T>
	constexpr auto operator+(const T &lhs, const T &rhs) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<T>())
	{		
		T temp{ lhs };
		return temp += rhs;
		
	}



	
	template <class T>
	constexpr auto operator-(const T &lhs, const T &rhs) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<T>())
	{		
		T temp{ lhs };
		return temp -= rhs;
		
	}
	
	
}
