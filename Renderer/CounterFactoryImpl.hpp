#pragma once
#include "CounterFactory.hpp"
#include <vector>
#include <mutex>


namespace Renderer::DX12
{
	class CounterFactoryImpl final : public CounterFactory
	{
		private: std::vector<size_t> counterValues;

		private: mutable std::mutex mutexCounter;

		private: mutable std::condition_variable cvCounterWaiters;

		private: bool releaseAllWaiters;



		public: CounterFactoryImpl();

		public: ~CounterFactoryImpl() noexcept override;
		
		public: CounterFactoryImpl(const CounterFactoryImpl &) = delete;

		public: CounterFactoryImpl &operator=(const CounterFactoryImpl &) = delete;
		
		public: CounterFactoryImpl(CounterFactoryImpl &&) = delete;

		public: CounterFactoryImpl &operator=(CounterFactoryImpl &&) = delete;
						
		
		public: CounterID MakeCounter(size_t initialValue) override;
		
		public: size_t GetCounterValue(CounterID id) const override;
		
		public: size_t WaitForCounterToReach(CounterID id, size_t thresholdValue) const override;


		public: void IncreaseCounter(CounterID id, size_t value);
		
	};

	
}
