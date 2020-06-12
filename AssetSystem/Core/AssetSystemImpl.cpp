#include "Core/AssetSystemImpl.hpp"


namespace assetSystem
{
	Core::AssetSystemImpl::AssetSystemImpl(AssetRegistry &&registry)
		:
		registry{ std::move(registry) }
	{}

	
	
}
