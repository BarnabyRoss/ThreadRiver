
#include "TaskQueue.h"


TaskQueue::TaskQueue(size_t size) : max_size_(size) {

}

TaskQueue::~TaskQueue(){
	
	
}

void TaskQueue::push(Task task){
	
	std::unique_lock lock(mtx_);
	cv_.wait(lock, [this](){ return queue_.size() < max_size_; });
	
	queue_.push(task);
	cv_.notify_one();
}

TaskQueue::Task TaskQueue::pop(){
	
	std::unique_lock lock(mtx_);
	cv_.wait(lock, [this](){ return !queue_.empty(); });
	
	Task task = std::move(queue_.front());
	queue_.pop();
	
	return std::move(task);
}

bool TaskQueue::empty(){
	
	std::lock_guard lock(mtx_);
	
	return queue_.empty();
}
