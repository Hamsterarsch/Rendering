#include "Resources/HandleWrapper.hpp"
#include "Resources/MaintainsRenderResources.hpp"



namespace Renderer
{
	namespace DX12
	{
		HandleWrapper::HandleWrapper() :
			retirementTarget{ nullptr },
			handle{ 0 }
		{
		}


		
		HandleWrapper::HandleWrapper(MaintainsRenderResources *origin, const size_t handle) :
			retirementTarget{ origin },
			handle{ handle }
		{
		}


		
		HandleWrapper::HandleWrapper(HandleWrapper &&other) noexcept :
			retirementTarget{ other.retirementTarget },
			handle{ other.handle }
		{
			other.Invalidate();
			
		}

			void HandleWrapper::Invalidate()
			{
				handle = 0;
				retirementTarget = nullptr;
			
			}


		
		HandleWrapper &HandleWrapper::operator=(HandleWrapper &&rhs) noexcept
		{
			retirementTarget = rhs.retirementTarget;			
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
					retirementTarget->RetireHandle(handle);
					Invalidate();
				}
			
			}

				bool HandleWrapper::IsValid() const
				{
					return retirementTarget != nullptr && handle > 0;
			
				}

		
	}
	
	
}
