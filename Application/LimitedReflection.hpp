#pragma once
#include "Shared/InterfaceHelpers.hpp"


namespace App::Reflection
{
	class TypeIdCounter
	{
		protected: static unsigned GetIdForType()
		{
			static unsigned counter{ 0 };
			return counter++;
		}
		
	};


	
	template<class T>
	class TypeId : public TypeIdCounter
	{
		public: using type = T;
		
		private: inline static unsigned id{ GetIdForType() };

		public: static unsigned GetId() { return id; }

		public: operator unsigned() const { return id; }
						
	};



	class LimitedReflected
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS(LimitedReflected)

		private: static constexpr TypeId<LimitedReflected> typeId{};
		
		public: virtual bool CastableTo(unsigned id) const
		{
			return id == typeId.GetId();				
				
		}

		public: virtual unsigned GetTypeId() const
		{
			return typeId;
		}
		
	};


	
	template<class T>
	bool CastableTo(const LimitedReflected &from) { return from.CastableTo(TypeId<T>::GetId()); }

	template<class T>
	bool CastableTo(const LimitedReflected *from) { return from->CastableTo(TypeId<T>::GetId()); }
	

}

#define DEFINE_TYPE_INFO(type, base)					\
private: static constexpr TypeId<type>	typeId{};		\
														\
public: bool CastableTo(unsigned id) const override		\
{														\
	if(id == typeId.GetId())							\
	{													\
		return true;									\
	}													\
														\
	return base::CastableTo(id);						\
}														\
														\
public: unsigned GetTypeId() const override				\
{														\
	return typeId;										\
}	
