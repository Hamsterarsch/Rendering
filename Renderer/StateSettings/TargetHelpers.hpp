#pragma once


namespace Renderer::DX12
{
	template<class t_format_interface> struct FormatMapping{ static_assert("no there is no implementation mapped for this format interface!"); };
		
	template<class t_target>
	typename FormatMapping<t_target>::t_dataDeref GetTargetValue(const void *(t_target:: *t)() const)
	{
		return *reinterpret_cast<const typename FormatMapping<t_target>::t_dataDeref *>((FormatMapping<t_target>::mapping::Get().*t)());
		
	}

	
}
