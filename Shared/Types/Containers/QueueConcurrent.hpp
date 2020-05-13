#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>


template<class T>
class QueueConcurrent
{
	private: std::queue<T> queue;

	private: mutable std::mutex mutex;

	private: mutable std::condition_variable condVarEmpty, condVarPush;
			 			 	

	
	public: void Push(const T &item);
	
	public: void Push(T &&item);

	public: T Pop();

	public: void Empty();
	
	public: bool IsEmpty() const;
	
	public: size_t Size() const;

	public: void WaitForEmpty() const;

	public: void WaitForItems() const;

	public: T ExtractFront();
				
};

template <class T>
void QueueConcurrent<T>::Push(const T &item)
{
	std::lock_guard lock{ mutex };

	queue.push(item);
	condVarPush.notify_all();
	
}



template <class T>
void QueueConcurrent<T>::Push(T &&item)
{
	std::lock_guard lock{ mutex };

	queue.push(std::move(item));
	condVarPush.notify_all();
	
}



template <class T>
T QueueConcurrent<T>::Pop()
{
	std::lock_guard lock{ mutex };

	T item{ std::move(queue.front()) };
	queue.pop();

	if(queue.empty())
	{
		condVarEmpty.notify_all();
	}
	
	return item;
	
}



template <class T>
void QueueConcurrent<T>::Empty()
{
	std::lock_guard lock{ mutex };

	while(!queue.empty())
	{
		queue.pop();
	}
	condVarEmpty.notify_all();

	
}



template <class T>
bool QueueConcurrent<T>::IsEmpty() const
{
	std::lock_guard lock{ mutex };
	
	return queue.empty();
	
}



template <class T>
size_t QueueConcurrent<T>::Size() const
{
	std::lock_guard lock{ mutex };
	
	return queue.size();
	
}



template <class T>
void QueueConcurrent<T>::WaitForEmpty() const
{
	std::unique_lock lock{ mutex };
	condVarEmpty.wait(lock, [&q=queue](){ return q.empty(); });
	
}



template <class T>
void QueueConcurrent<T>::WaitForItems() const
{
	std::unique_lock lock{ mutex };
	condVarPush.wait(lock, [&q=queue](){ return !q.empty(); });
	
}



template <class T>
T QueueConcurrent<T>::ExtractFront()
{
	std::lock_guard lock{ mutex };

	auto out{ std::move(queue.front()) };
	return out;
	
}
