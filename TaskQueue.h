
#include <iostream>
#include <thread>
#include <condition_variable>
#include <queue>
#include <functional>
#include <memory>
#include <optional>
#include <mutex>
#include <atomic>


class TaskQueue{

public:
	using Task = std::function<void()>;
	
private:
	std::queue<Task> queue_;
	std::mutex mtx_;
	std::condition_variable cv_;
	const size_t max_size_;
	bool stop_;
	std::atomic<int> exitNumber_{0};
	
public:
	TaskQueue(size_t size, bool stop = false);
	~TaskQueue();
	
	void push(Task task);
	std::optional<Task> pop();
	bool empty();
	void notifyAll();
	void setCounter(int num){ exitNumber_.store(num); }

};

