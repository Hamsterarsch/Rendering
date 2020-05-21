#pragma once
#include "Commands/DX12Command.hpp"
#include "Resources/ResourceRegistry.hpp"


namespace Renderer::DX12::Commands
{

	
	class BindPsoAndSignatureCommand : public DX12Command
	{
		private: size_t signature;

		private: size_t pso;


		
		public: BindPsoAndSignatureCommand();

		public: BindPsoAndSignatureCommand(size_t signatureHandle, size_t psoHandle);

		
		void ExecuteOperationOnResourceReferences(UsesReferences &registry, void(UsesReferences::* operation)(size_t))	override;
		
		public:	void Execute(DX12CommandProcessor &context) override;

			private: void RecordPsoBinding(DX12CommandProcessor &context) const;
					
				private: bool PsoIsValid() const;

		protected: bool SignatureIsValid() const;

		protected: virtual void RecordSignatureBinding(DX12CommandProcessor &context) const = 0;
				
		protected: size_t GetSignatureHandle() const { return signature; }
				   
	};

		
}
