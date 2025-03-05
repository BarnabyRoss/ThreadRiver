
#include <iostream>
#include <thread>
#include <condition_variable>
#include <memory>
#include <atomic>
#include <vector>
#include <future>
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
	
	template< typename F, typename... Args >
	auto submit(F&& func, Args... args) -> std::future<std::invoke_result_t<F, Args...>>;
	//void stop();
	void shutdown();
};

