
#include "ThreadPool.h"


ThreadPool::ThreadPool(size_t threads) : taskQueue_(2000), stop_(false){
	
	for(int i = 0; i < threads; ++i)
		workers.emplace_back(&ThreadPool::work, this);
}

ThreadPool::~ThreadPool(){

  if( !stop_ )
		shutdown();
}

void ThreadPool::work(){

	while( !stop_ ){
		
		/*auto task = taskQueue_.pop();
		task();
		if( stop_ ) break;
		*/
		taskQueue_.pop()();
	}

}

template< typename F, typename... Args >
auto ThreadPool::submit(F&& func, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>{
	
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

/*void ThreadPool::stop(){

	stop_ = true;
	taskQueue_.notifyAll();
}
*/

void ThreadPool::shutdown(){

	stop_ = true;
	taskQueue_.notifyAll();
	for(auto& worker : workers)
		if( worker.joinable() )
			worker.join();
}