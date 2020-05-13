#pragma once
#include "Shared/InterfaceHelpers.hpp"
#include "Shared/PtrTypes.hpp"


namespace RHA { namespace DX12{ class CmdList; } }

namespace Renderer::DX12
{
	class UsesReferences;
	class HasQueriableResources;
	class ResourceRegistry;
		
	class RenderCommand
	{
		DEFAULTED_INTERFACE_CONSTRUCTION_OPERATIONS_NOCTOR(RenderCommand)


		private: size_t signatureHandle;

		private: size_t psoHandle;


		
		public: RenderCommand();

		public: RenderCommand(size_t signatureHandle, size_t psoHandle);
			
		public: void AddSignatureReference(UsesReferences &to) const;

		protected: bool SignatureIsValid() const;
		
		public: void AddPsoReference(UsesReferences &to) const;

		protected: bool PsoIsValid() const;

		protected: size_t GetPsoHandle() const { return psoHandle; }

		protected: size_t GetSignatureHandle() const { return signatureHandle; }

		protected: static constexpr unsigned char GetGlobalBufferSlot() { return 0; }

		public: virtual void ExecutePostGpuWork() {}

		public: virtual void RecordFixedCommandState(RHA::DX12::CmdList *list, HasQueriableResources &registry, size_t globalBufferHandle) const = 0;
								
		public: virtual void ExecuteOperationOnResourceReferences(UsesReferences *registry, void(UsesReferences:: *operation)(size_t)) = 0;
			
		public: virtual void Record(RHA::DX12::CmdList *list, HasQueriableResources &registry) = 0;
			
	};

		
}
