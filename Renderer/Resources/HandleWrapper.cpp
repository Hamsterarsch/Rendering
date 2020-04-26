#include "Resources/HandleWrapper.hpp"
#include "Renderer.hpp"



namespace Renderer
{
	namespace DX12
	{
		HandleWrapper::HandleWrapper() :
			origin{ nullptr },
			handle{ 0 }
		{
		}


		
		HandleWrapper::HandleWrapper(IRenderer *origin, const size_t handle) :
			origin{ origin },
			handle{ handle }
		{
		}


		
		HandleWrapper::HandleWrapper(HandleWrapper &&other) noexcept :
			origin{ other.origin },
			handle{ other.handle }
		{
			other.Invalidate();
			
		}

			void HandleWrapper::Invalidate()
			{
				handle = 0;
				origin = nullptr;
			
			}


		
		HandleWrapper &HandleWrapper::operator=(HandleWrapper &&rhs) noexcept
		{
			origin = rhs.origin;			
			handle = rhs.handle;

			rhs.Invalidate();
			return *this;
			
		}


		
		HandleWrapper::~HandleWrapper()
		{
			Reset();
			
		}

			void HandleWrapper::Reset()
			{
				if(IsValid())
				{
					origin->RetireHandle(handle);
					Invalidate();
				}
			
			}

				bool HandleWrapper::IsValid() const
				{
					return origin != nullptr && handle > 0;
			
				}

		
	}
	
	
}
