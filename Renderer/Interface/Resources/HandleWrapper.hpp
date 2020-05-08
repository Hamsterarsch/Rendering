#pragma once
#include "RendererExportHelper.hpp"


namespace Renderer
{
	class MaintainsRenderResources;
		
	namespace DX12
	{
		
		class RENDERER_DLLSPEC HandleWrapper
		{
			private: MaintainsRenderResources *retirementTarget;
			
			private: size_t handle;
					 


			public: HandleWrapper();
			
			public: HandleWrapper(MaintainsRenderResources *retirementTarget, size_t handle);

			public: HandleWrapper(const HandleWrapper &) = delete;

			public: HandleWrapper &operator=(const HandleWrapper &) = delete;

			public: HandleWrapper(HandleWrapper &&other) noexcept;

				private: void Invalidate();
			
			public: HandleWrapper &operator=(HandleWrapper &&rhs) noexcept;

			
			public: ~HandleWrapper();
								
			public: void Reset();

			public: bool IsValid() const;
			
			public: inline operator size_t() const { return handle; } 
			
		};
	
		
	}

	
}
