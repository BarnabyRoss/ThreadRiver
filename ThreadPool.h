
#include <iostream>
#include <thread>
#include <condition_variable>
#include <memory>
#include "TaskQueue.h"

class ThreadPool{

private:
	TaskQueue taskQueue_;
	std::vector<std::thread> workers;
	
	void work();

public:
	ThreadPool(size_t threads);
  ~ThreadPool();
	
	void submit(std::function<void()> task);
};

