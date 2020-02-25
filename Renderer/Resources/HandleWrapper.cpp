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


		
		HandleWrapper::HandleWrapper(Renderer *origin, const size_t handle) :
			origin{ origin },
			handle{ handle }
		{
		}


		
		HandleWrapper::HandleWrapper(HandleWrapper &&other) :
			origin{ other.origin },
			handle{ other.handle }
		{
			other.origin = nullptr;
			other.handle = 0;
			
		}


		
		HandleWrapper &HandleWrapper::operator=(HandleWrapper &&rhs)
		{
			origin = rhs.origin;
			rhs.origin = nullptr;
			
			handle = rhs.handle;
			rhs.handle = 0;

			return *this;
			
		}


		
		HandleWrapper::~HandleWrapper()
		{
			if(origin != nullptr && handle > 0)
			{
				origin->RetireHandle(handle);
			}
			
		}

		
	}
	
	
}
