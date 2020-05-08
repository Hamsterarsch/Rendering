#pragma once
#include "ThirdParty/glm/vec2.hpp"
#include "ThirdParty/glm/vec3.hpp"
#include "ThirdParty/glm/vec4.hpp"


namespace Math
{
	template<class t_derivedClass, class t_glm>
	class VectorBase
	{
		private: using t_target = t_derivedClass;

		public: constexpr auto operator+=(const t_target &other) -> t_target &;

		public: constexpr auto operator-=(const t_target &other) -> t_target &;

		public: constexpr VectorBase &operator/=(const typename t_glm::value_type &scalar);
		
	};
	   	
	template <class T>
	constexpr auto operator+(const T &lhs, const T &rhs) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<T>());

	template <class T>
	constexpr auto operator-(const T &lhs, const T &rhs) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<T>());

	template <class T>
	constexpr auto operator/(const T &lhs, const typename T::t_value &scalar) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<T>());

	template <class T>
	constexpr auto operator/(const typename T::t_value &scalar, const T &rhs) -> decltype(VectorBase<T, typename T::t_wrapped>(std::declval<T>()), std::declval<T>());
	
	
	template<class t_glm, unsigned length = t_glm::length()>
	class VectorProxy : public VectorBase<VectorProxy<t_glm>, t_glm> { static_assert(true, "invalid vector base type"); };	

	template<class t_glm>
	class VectorProxy<t_glm, 2> : public VectorBase<VectorProxy<t_glm>, t_glm> 
	{
		public: using t_wrapped = t_glm;
		
		public: using t_value = typename t_wrapped::value_type;

		union
		{
			struct{ t_value x; t_value y; };
						
			t_glm _internal;
			
		};


		public: constexpr VectorProxy() = default;
		
		public: template<class ...t_args> constexpr VectorProxy(t_args ...Args) : _internal{Args...} {}
										
	};

	using Vector2 = VectorProxy<glm::vec2>;
	using VectorInt2 = VectorProxy<glm::ivec2>;
	using VectorUint2 = VectorProxy<glm::uvec2>;



	template<class t_glm>
	class VectorProxy<t_glm, 3> : public VectorBase<VectorProxy<t_glm>, t_glm> 
	{
		public: using t_wrapped = t_glm;
		
		public: using t_value = typename t_wrapped::value_type;

		union
		{
			struct{ t_value x; t_value y; t_value z; };

			t_glm _internal;
			
		};


		public: constexpr VectorProxy() = default;
		
		public: template<class ...t_args> constexpr VectorProxy(t_args ...Args) : _internal{Args...} {}
										
	};
	
	using Vector3 = VectorProxy<glm::vec3>;
	using VectorInt3 = VectorProxy<glm::ivec3>;
	using VectorUint3 = VectorProxy<glm::uvec3>;

	
	
	template<class t_glm>
	class VectorProxy<t_glm, 4> : public VectorBase<VectorProxy<t_glm>, t_glm> 
	{
		public: using t_wrapped = t_glm;
		
		public: using t_value = typename t_wrapped::value_type;

		union
		{
			struct{ t_value x; t_value y; t_value z; t_value w; };

			t_glm _internal;
			
		};


		public: constexpr VectorProxy() = default;
		
		public: template<class ...t_args> constexpr VectorProxy(t_args ...Args) : _internal{Args...} {}
										
	};

	using Vector4 = VectorProxy<glm::vec4>;
	using VectorInt4 = VectorProxy<glm::ivec4>;
	using VectorUint4 = VectorProxy<glm::uvec4>;
	
	
}

#include "Vector.inl"
