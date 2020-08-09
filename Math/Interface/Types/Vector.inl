

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
		static_cast<t_derivedClass *>(this)->_internal -= other._internal;
		return *static_cast<t_derivedClass *>(this);
		
		
	}



	template <class t_derivedClass, class t_glm>
	constexpr auto VectorBase<t_derivedClass, t_glm>::operator/=(const typename t_glm::value_type &scalar) -> t_target&
	{		
		static_cast<t_derivedClass *>(this)->_internal /= scalar;		
		return *static_cast<t_derivedClass *>(this);
	}


	
	template <class t_derivedClass, class t_glm>
	constexpr auto VectorBase<t_derivedClass, t_glm>::operator*=(const typename t_glm::value_type &scalar) -> t_target &
	{		
		static_cast<t_derivedClass *>(this)->_internal *= scalar;		
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



	template <class T>
	constexpr auto operator/(const T &lhs, const typename T::t_value &scalar) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<T>())
	{
		T temp{ lhs };
		return temp /= scalar;
		
	}


	
	template <class T>
	constexpr auto operator/(const typename T::t_value &scalar, const T &rhs) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<T>())
	{
		T temp{ rhs };
		return temp /= scalar;
		
	}



	template <class T>
	constexpr auto operator*(const T &lhs, const typename T::t_value &scalar) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<T>())
	{
		T temp{ lhs };
		return temp *= scalar;
		
	}


	
	template <class T>
	constexpr auto operator*(const typename T::t_value &scalar, const T &rhs) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<T>())
	{
		T temp{ rhs };
		return temp *= scalar;
		
	}
	

	
	template <class T>
	constexpr auto operator==(const T &lhs, const T &rhs) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<bool>())
	{
		return lhs._internal == rhs._internal;
		
	}


	
	template <class T>
	constexpr auto operator!=(const T &lhs, const T &rhs) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<bool>())
	{
		return not (lhs == rhs);
		
	}
	
	
}
