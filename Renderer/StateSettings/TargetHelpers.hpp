#pragma once
#include "FormatTargetsImpl.hpp"


namespace Renderer::DX12
{
	template<class t_format_interface> struct FormatMapping{ static_assert("no there is no implementation mapped for this format interface!"); };
	template<> struct FormatMapping<FormatTargets>{ using mapping = FormatTargetsImpl; };


	
	template<class t_cast_deref, class t_target>
	t_cast_deref GetTargetValue(const void *(t_target:: *t)() const)
	{
		return *reinterpret_cast<const t_cast_deref *>((FormatMapping<t_target>::mapping::Get().*t)());
		
	}

	
}
