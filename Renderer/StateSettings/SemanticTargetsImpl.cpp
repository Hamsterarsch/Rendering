#include "StateSettings/SemanticTargetsImpl.hpp"


namespace Renderer::DX12
{
	const void* SemanticTargetsImpl::TargetPosition() const
	{
		static constexpr const char *semantic{ "POSITION" };
		return semantic;
		
	}



	const void *SemanticTargetsImpl::TargetTexcoord() const
	{
		static constexpr const char *semantic{ "TEXCOORD" };
		return semantic;
		
	}



	const void *SemanticTargetsImpl::TargetNormal() const
	{
		static constexpr const char *semantic{ "NORMAL" };
		return semantic;
		
	}



	const void *SemanticTargetsImpl::TargetColor() const
	{
		static constexpr const char *semantic{ "COLOR" };
		return semantic;
		
	}



	const SemanticTargetsImpl &SemanticTargetsImpl::Get()
	{
		static const SemanticTargetsImpl instance{};
		return instance;
		
	}


}
