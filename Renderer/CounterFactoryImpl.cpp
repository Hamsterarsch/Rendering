#include "CounterFactoryImpl.hpp"


namespace Renderer::DX12
{
	CounterFactoryImpl::CounterFactoryImpl() : releaseAllWaiters{ false }
	{
	}


	
	CounterFactoryImpl::~CounterFactoryImpl() noexcept
	{
		releaseAllWaiters = true;		
		cvCounterWaiters.notify_all();
		
	}


	
	CounterFactory::CounterID CounterFactoryImpl::MakeCounter(const size_t initialValue)
	{
		std::lock_guard lock{ mutexCounter };
		
		counterValues.emplace_back(initialValue);

		return counterValues.size() - 1;
		
	}


	
	size_t CounterFactoryImpl::GetCounterValue(const CounterID id) const
	{
		std::lock_guard lock{ mutexCounter };
		
		return counterValues.at(id);
		
	}


	
	size_t CounterFactoryImpl::WaitForCounterToReach(const CounterID id, const size_t thresholdValue) const
	{
		std::unique_lock lock{ mutexCounter };

		
		if(counterValues.at(id) >= thresholdValue)
		{
			return counterValues.at(id);
			
		}

		cvCounterWaiters.wait(lock, [&counters = counterValues, id = id, threshold = thresholdValue, &b = releaseAllWaiters]()
		{
			return counters.at(id) >= threshold || b;
			
		});

		return counterValues.at(id);
		
	}


	
	void CounterFactoryImpl::IncreaseCounter(const CounterID id, const size_t value)
	{
		std::unique_lock lock{ mutexCounter };

		counterValues.at(id) += value;
		lock.unlock();
		
		cvCounterWaiters.notify_all();
		
	}

	
}
