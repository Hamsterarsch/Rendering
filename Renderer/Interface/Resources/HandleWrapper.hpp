#pragma once
#include "RendererExportHelper.hpp"
#include "Renderer/Interface/Resources/ResourceHandle.hpp"


namespace Renderer
{
	class MaintainsRenderResources;
		
	class RENDERER_DLLSPEC HandleWrapper
	{
		private: ResourceHandle::t_hash handle;
		
		private: MaintainsRenderResources *retirementTarget;
				 
	
	
		public: HandleWrapper();
		
		public: HandleWrapper(MaintainsRenderResources *retirementTarget, ResourceHandle::t_hash handle);
	
		public: HandleWrapper(const HandleWrapper &) = delete;
	
		public: HandleWrapper &operator=(const HandleWrapper &) = delete;
	
		public: HandleWrapper(HandleWrapper &&other) noexcept;
	
		public: HandleWrapper &operator=(HandleWrapper &&rhs) noexcept;
		
		public: void Reset();
		
		public: bool IsValid() const;
		
			private: void Invalidate();
		
		public: ~HandleWrapper();
	
		
		public: operator ResourceHandle::t_hash() const { return handle; }

		public: operator bool() const { return IsValid(); }
	
		public: ResourceHandle::t_hash Get() const { return handle; }
		
	};

	
}
