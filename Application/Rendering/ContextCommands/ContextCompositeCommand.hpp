#pragma once
#include "Renderer/Interface/Commands/CompositeCommand.hpp"


namespace App::Rendering
{
	class ContextCompositeCommand final : public Renderer::Commands::CompositeCommand
	{
		private: Renderer::Commands::CommandContextEventFlags triggerEventsFlags;

		
		
		public: ContextCompositeCommand(Renderer::Commands::CommandContextEventFlags eventsThatTriggerExecution) : triggerEventsFlags{ eventsThatTriggerExecution } {}

		public: bool ShouldBeExecuted(const Renderer::Commands::CommandProcessor &context) const override { return context.DoesContextEventMatchAny(triggerEventsFlags); }
				
	};
	
	
}