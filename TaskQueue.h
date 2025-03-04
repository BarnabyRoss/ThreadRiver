
#include <iostream>
#include <thread>
#include <condition_variable>
#include <queue>
#include <functional>
#include <memory>


class TaskQueue{

public:
	using Task = std::function<void()>;
	
private:
	std::queue<Task> queue_;
	std::mutex mtx_;
	std::condition_variable cv_;
	const size_t max_size_;
	bool stop_;
	
public:
	TaskQueue(size_t size, bool stop = false);
	~TaskQueue();
	
	void push(Task task);
	Task pop();
	bool empty();
	void notifyAll();

};

