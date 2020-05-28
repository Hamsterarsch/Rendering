#include "Interface/Commands/CompositeCommand.hpp"
#include "Resources/UsesReferences.hpp"


namespace Renderer::Commands
{
	void CompositeCommand::ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences:: *operation)(size_t))
	{
		for(auto &&command : children)
		{
			command->ExecuteOperationOnResourceReferences(registry, operation);
		}

		for(auto &&resource : resources)
		{
			Invoke(registry, operation, resource);
		}
		
	}


	
	void CompositeCommand::Execute(CommandProcessor &context)
	{
		for(auto &&command : children)
		{
			command->Execute(context);
		}
		
	}


	
	void CompositeCommand::Add(UniquePtr<Command> &&command)
	{
		children.emplace_back(std::move(command));
		
	}


	
	void CompositeCommand::AddReferenceTo(const ResourceHandle::t_hash resource)
	{
		resources.emplace_back(resource);
		
	}

	
}
