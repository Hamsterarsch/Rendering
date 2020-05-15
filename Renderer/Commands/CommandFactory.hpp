#pragma once
#include "Shared/PtrTypes.hpp"


namespace Renderer{ class MaintainsInternalRenderResources; }

namespace Renderer::DX12
{
	class ResourceRegistry;
	class DescriptorMemory;
	
	class CommandFactory
	{
		private: MaintainsInternalRenderResources *factory;

		private: ResourceRegistry *registry;

		private: DescriptorMemory *descriptorMemory;



		public: CommandFactory(MaintainsInternalRenderResources &factory, ResourceRegistry &registry, DescriptorMemory &descriptorMemory) :
			factory{ &factory },
			registry{ &registry },
			descriptorMemory{ &descriptorMemory }
		{}

		public: template<class T, class ...Args>
		auto MakeCommand(Args ...args) -> std::remove_reference_t<decltype( T(std::forward<Args>(args)..., *factory, *registry, *descriptorMemory), std::declval<UniquePtr<T>>())>
		{			
			return std::make_unique<T>(std::forward<Args>(args)..., *factory, *registry, *descriptorMemory);
		
		}

		public: template<class T, class ...Args>
		auto MakeCommand(Args ...args) -> std::remove_reference_t<decltype( T(std::forward<Args>(args)...), std::declval<UniquePtr<T>>())>
		{
			return std::make_unique<T>(std::forward<Args>(args)...);
			
		}

		
		public: template<class T>
		auto MakeCommand(...) -> UniquePtr<T>
		{		
			static_assert(false, "Make command arguments are invalid parameters for command constructor");
			
		}
		
	};
	
	
}
