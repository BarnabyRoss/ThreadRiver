
#include "ThreadPool.h"


ThreadPool::ThreadPool(size_t threads) : taskQueue_(2000){
	
	for(int i = 0; i < threads; ++i)
		workers.emplace_back(ThreadPool::work, this);
}

ThreadPool::~ThreadPool(){

  
}

void ThreadPool::work(){

	while(true){
		
		auto task = taskQueue_.pop();
		task();
	}

}