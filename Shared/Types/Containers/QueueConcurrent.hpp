#pragma once
#include <queue>
#include <mutex>


template<class T>
class QueueConcurrent
{
	private: std::queue<T> queue;

	private: mutable std::mutex mutex;
				 	

	
	public: void Push(const T &item);

	public: void Push(T &&item);

	public: T Pop();

	public: bool IsEmpty() const;
	
	public: size_t Size() const;
				
};

template <class T>
void QueueConcurrent<T>::Push(const T &item)
{
	std::lock_guard<std::mutex> lock{ mutex };

	queue.push(item);
	
}

template <class T>
void QueueConcurrent<T>::Push(T &&item)
{
	std::lock_guard<std::mutex> lock{ mutex };

	queue.push(std::move(item));
	
}

template <class T>
T QueueConcurrent<T>::Pop()
{
	std::lock_guard<std::mutex> lock{ mutex };
	
	T item{ std::move(queue.front()) };
	queue.pop();

	return item;
	
}

template <class T>
bool QueueConcurrent<T>::IsEmpty() const
{
	std::lock_guard<std::mutex> lock{ mutex };
	
	return queue.empty();
	
}

template <class T>
size_t QueueConcurrent<T>::Size() const
{
	std::lock_guard<std::mutex> lock{ mutex };
	
	return queue.size();
	
}
