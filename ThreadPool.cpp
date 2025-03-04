
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

void ThreadPool::submit(std::function<void()> task){
	
	taskQueue_.push(std::move(task));
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