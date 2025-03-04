
#include <iostream>
#include <thread>
#include <condition_variable>
#include <memory>
#include <atomic>
#include <vector>
#include "TaskQueue.h"

class ThreadPool{

private:
	TaskQueue taskQueue_;
	std::vector<std::thread> workers;
	std::atomic<bool> stop_;
	
	void work();

public:
	ThreadPool(size_t threads);
  ~ThreadPool();
	
	void submit(std::function<void()> task);
	//void stop();
	void shutdown();
};

