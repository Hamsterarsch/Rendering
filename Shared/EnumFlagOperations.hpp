#pragma once
#include <type_traits>

#define GENERATE_BINARY_OPERATORS_FOR_ENUM_FLAGS(EnumType)							\
																					\
/*|	*/																				\
																					\
inline EnumType operator|(EnumType Lhs, EnumType Rhs) noexcept						\
{																					\
	using T = std::underlying_type_t<EnumType>;										\
	return static_cast<EnumType>( static_cast<T>(Lhs) | static_cast<T>(Rhs) );		\
																					\
}																					\
																					\
inline EnumType &operator|=(EnumType &Lhs, EnumType Rhs) noexcept					\
{																					\
	Lhs = Lhs | Rhs;																\
	return Lhs;																		\
																					\
}																					\
																					\
																					\
/*&	*/																				\
																					\
inline EnumType operator&(EnumType Lhs, EnumType Rhs) noexcept						\
{																					\
	using T = std::underlying_type_t<EnumType>;										\
	return static_cast<EnumType>(static_cast<T>(Lhs) & static_cast<T>(Rhs));		\
																					\
}																					\
																					\
inline EnumType &operator&=(EnumType &Lhs, EnumType Rhs) noexcept					\
{																					\
	Lhs = Lhs & Rhs;																\
	return Lhs;																		\
																					\
}																					\
																					\
																					\
/*^	*/																				\
																					\
inline EnumType operator^(EnumType Lhs, EnumType Rhs) noexcept						\
{																					\
	using T = std::underlying_type_t<EnumType>;										\
	return static_cast<EnumType>(static_cast<T>(Lhs) ^ static_cast<T>(Rhs));		\
																					\
}																					\
																					\
inline EnumType &operator^=(EnumType &Lhs, EnumType Rhs) noexcept					\
{																					\
	Lhs = Lhs ^ Rhs;																\
	return Lhs;																		\
																					\
}																					\
																					\
																					\
/*~	*/																				\
																					\
inline EnumType operator~(EnumType Lhs)	noexcept									\
{																					\
	using T = std::underlying_type_t<EnumType>;										\
	return static_cast<EnumType>(~static_cast<T>(Lhs));								\
																					\
}																																									

