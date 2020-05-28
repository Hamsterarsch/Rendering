#pragma once
#include "Commands/Command.hpp"
#include "Shared/PtrTypes.hpp"
#include <vector>
#include "Resources/ResourceHandle.hpp"


namespace Renderer::Commands
{
	class RENDERER_DLLSPEC CompositeCommand : public Command
	{
		private: std::vector<UniquePtr<Command>> children;

		private: std::vector<ResourceHandle::t_hash> resources;
		

		public: CompositeCommand() = default;
		
		public: CompositeCommand(const CompositeCommand &) = delete;

		public: CompositeCommand &operator=(const CompositeCommand &) = delete;

		
		public: void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t)) final override;
		
		public: void Execute(CommandProcessor &context) override;

		public: void Add(UniquePtr<Command> &&command);

		public: void AddReferenceTo(ResourceHandle::t_hash resource);
								
	};

	
}
