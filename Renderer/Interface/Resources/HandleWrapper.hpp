#pragma once
#include "RendererExportHelper.hpp"


namespace Renderer
{
	namespace DX12
	{
		class Renderer;
		
		class RENDERER_DLLSPEC HandleWrapper
		{
			private: Renderer *origin;
			
			private: size_t handle;
					 


			public: HandleWrapper();
			
			public: HandleWrapper(Renderer *origin, size_t handle);

			public: HandleWrapper(const HandleWrapper &) = delete;

			public: HandleWrapper &operator=(const HandleWrapper &) = delete;

			public: HandleWrapper(HandleWrapper &&other) noexcept;

			public: HandleWrapper &operator=(HandleWrapper &&rhs) noexcept;
			
			public: ~HandleWrapper();
								
			public: void Reset();

			public: bool IsValid() const;
			
			public: inline operator size_t() const { return handle; } 
			
		};
	
		
	}

	
}
