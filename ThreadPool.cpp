
#include "ThreadPool.h"

std::atomic<std::uint64_t> ThreadPool::id_counter_ = 0;

ThreadPool::ThreadPool(size_t threads) : scheduler_(2000), stop_(false), 
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
		
		auto task = scheduler_.pop();
		if( task.has_value() ){
		
			task.value()->execute();
			++completed_tasks_;
			
		}
		if( stop_ ) break;
		
	}

}

void ThreadPool::shutdown(){

	stop_ = true;
	scheduler_.notifyAll();
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
		scheduler_.setCounter(num);
		scheduler_.notifyAll();

		//清理退出线程
		for(auto& worker : workers){
			if( worker.joinable() ){
				worker.join();
			}
		}
		workers.erase(std::remove_if(workers.begin(), workers.end(), [](const std::thread& t){ return !t.joinable();}) ,workers.end());
	}
}