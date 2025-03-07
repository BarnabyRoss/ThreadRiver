
#include "ThreadPool.h"


ThreadPool::ThreadPool(size_t threads) : taskQueue_(2000), stop_(false), 
																			max_threads_(std::max(2u, std::thread::hardware_concurrency() * 2)){
	
	for(int i = 0; i < threads; ++i)
		workers.emplace_back(&ThreadPool::work, this);
}

ThreadPool::~ThreadPool(){

  if( !stop_ )
		shutdown();
}

void ThreadPool::work(){

	while( !stop_ ){
		
		auto task = taskQueue_.pop();
		if( task.has_value() ){
			task.value()();
		}
		if( stop_ ) break;
		
		//taskQueue_.pop()();
	}

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

void ThreadPool::resize(size_t newSize){

	std::lock_guard lock(mtx_);
	if( stop_ || newSize > getMaxThreads() ) return;

	if( newSize > getThreadCount() ){

		size_t num = newSize - getThreadCount();
		for(size_t i = 0; i < num; ++i){
			workers.emplace_back(&ThreadPool::work, this);
		}
	}else if( newSize < getThreadCount() ){

		int num = getThreadCount() - newSize;
		taskQueue_.setCounter(num);
		taskQueue_.notifyAll();
	}
}