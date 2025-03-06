
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
	auto submit(F&& func, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>{

		using ReturnType = std::invoke_result_t<F, Args...>;
		auto prom = std::make_shared<std::promise<ReturnType>>();
		auto fut = prom->get_future();

		auto task = [prom, func = std::forward<F>(func), ...args = std::forward<Args>(args)](){
			try{
				prom->set_value(func(args...));
			}catch(...){
				prom->set_execption(std::current_exception());
			}
		};

		taskQueue_.push(std::move(task));

		return fut;
	}
	//void stop();
	void shutdown();
};

