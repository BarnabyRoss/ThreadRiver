
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <thread>
#include <vector>
#include <future>
#include "Scheduler.h"

class ThreadPool{

private:
	Scheduler scheduler_;
	std::vector<std::thread> workers;
	std::atomic<bool> stop_;
	const size_t max_threads_;
	std::mutex mtx_;
	std::atomic<size_t> completed_tasks_{0};
	ErrorHandler errorHandler_;
	
	void work();

public:
	static std::atomic<std::uint64_t> id_counter_;

public:
	ThreadPool(size_t threads = std::thread::hardware_concurrency());
  ~ThreadPool();

	struct ThreadPoolStatus{

		size_t currentThreads;
		size_t maxThreads;
		size_t taskQueueSize;
		size_t completedTask;
	};
	
	template< typename F, typename... Args >
	auto submit(F&& func, uint8_t priority, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>{

		try{

			if( priority < Task::PRIORITY_LOW || priority > Task::PRIORITY_HIGH ) priority = Task::PRIORITY_NORMAL;

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

			std::shared_ptr<Task> ptr = std::make_shared<Task>(std::move(task), ThreadPool::id_counter_++, priority);
			scheduler_.push(std::move(ptr));

			return fut;

		}catch(const std::exception& e){

			errorHandler_.setError(ErrorType::THREAD_ERROR, std::string("Thread Pool Submit Failed : ") + e.what());
			throw;
		}
	}
	
	void shutdown();

	void resize(size_t newSize);

	size_t getMaxThreads() const { return max_threads_; }
	size_t getThreadCount() const { return workers.size(); }  //获取当前线程池中的线程数量
	size_t getTaskQueueSize(){ return scheduler_.getTaskQueueSize(); } //获取当前线程池中的任务数量
	size_t getCompletedTaskCount(){ return completed_tasks_.load(); } //获取当前线程池中已经完成的任务数量

	ThreadPoolStatus getStatus(){

		ThreadPoolStatus status;

		status.currentThreads = getThreadCount();
		status.maxThreads = getMaxThreads();
		status.taskQueueSize = getTaskQueueSize();
		status.completedTask = getCompletedTaskCount();

		return status;
	}

	ErrorHandler& getErrorHandler(){ return errorHandler_; }
};

#endif