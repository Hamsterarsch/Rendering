#pragma once
#include "RendererExportHelper.hpp"


namespace Renderer
{
	class MaintainsRenderResources;
		
	class RENDERER_DLLSPEC HandleWrapper
	{
		private: size_t handle;
		
		private: MaintainsRenderResources *retirementTarget;
				 
	
	
		public: HandleWrapper();
		
		public: HandleWrapper(MaintainsRenderResources *retirementTarget, size_t handle);
	
		public: HandleWrapper(const HandleWrapper &) = delete;
	
		public: HandleWrapper &operator=(const HandleWrapper &) = delete;
	
		public: HandleWrapper(HandleWrapper &&other) noexcept;
	
		public: HandleWrapper &operator=(HandleWrapper &&rhs) noexcept;
		
		public: void Reset();
		
		public: bool IsValid() const;
		
			private: void Invalidate();
		
		public: ~HandleWrapper();
	
		
		public: operator size_t() const { return handle; }
	
		public: size_t Get() const { return handle; }
		
	};

	
}
