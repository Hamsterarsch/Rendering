#include "Resources/HandleWrapper.hpp"
#include "Resources/MaintainsRenderResources.hpp"
#include <utility>


namespace Renderer::DX12
{
	HandleWrapper::HandleWrapper() :
		handle{ 0 },
		retirementTarget{ nullptr }
	{
	}


	
	HandleWrapper::HandleWrapper(MaintainsRenderResources *retirementTarget, const size_t handle) :
		handle{ handle },
		retirementTarget{ retirementTarget }
	{
	}


	
	HandleWrapper::HandleWrapper(HandleWrapper &&other) noexcept :
		HandleWrapper{}
	{
		*this = std::move(other);
		
	}


	
	HandleWrapper &HandleWrapper::operator=(HandleWrapper &&rhs) noexcept
	{
		if(this == &rhs)
		{
			return *this;
		}
		
		Reset();
		
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
		
				void HandleWrapper::Invalidate()
				{
					handle = 0;
					retirementTarget = nullptr;
				
				}

	
}
