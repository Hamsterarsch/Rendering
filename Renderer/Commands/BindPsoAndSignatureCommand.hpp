#pragma once
#include "Commands/Command.hpp"


#include "DX12/CmdList.hpp"
#include "Resources/ResourceRegistry.hpp"

namespace Renderer::DX12::Commands
{

	
	class BindPsoAndSignatureCommand : public Command
	{
		private: size_t signature;

		private: size_t pso;


		
		public: BindPsoAndSignatureCommand();

		public: BindPsoAndSignatureCommand(size_t signatureHandle, size_t psoHandle);

		
		void ExecuteOperationOnResourceReferences(UsesReferences &registry, void( UsesReferences::* operation)(size_t))	override;
		
		public:	void Execute(CommandProcessor &context) override;

			private: void RecordPsoBinding(CommandProcessor &context) const;
					
				private: bool PsoIsValid() const;

		protected: bool SignatureIsValid() const;

		protected: virtual void RecordSignatureBinding(CommandProcessor &context) const = 0;
				
		protected: size_t GetSignatureHandle() const { return signature; }
				   
	};

		
}
