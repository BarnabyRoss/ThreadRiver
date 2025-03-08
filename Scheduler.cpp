
#include "Scheduler.h"


Scheduler::Scheduler(size_t size, bool stop) : max_size_(size), stop_(stop) {

}

Scheduler::~Scheduler(){
	
	
}

void Scheduler::push(std::shared_ptr<Task> task){
	
	std::unique_lock lock(mtx_);
	cv_.wait(lock, [this](){ return tasks_.size() < max_size_ || stop_; });
	
	tasks_.push(std::move(task));
	cv_.notify_one();
}

std::optional<std::shared_ptr<Task>> Scheduler::pop(){
	
	std::unique_lock lock(mtx_);
	cv_.wait(lock, [this](){ return !tasks_.empty() || stop_ || exitNumber_ > 0; });

	if( exitNumber_ > 0){

		--exitNumber_;
		return std::nullopt;
	}

	if( tasks_.empty() && stop_ )
		return std::nullopt;

	auto nowTime = std::chrono::system_clock::now();
	if( !tasks_.empty() && !stop_ && tasks_.top()->getNextExecuteTime() <= nowTime){

		std::shared_ptr<Task> ptr = tasks_.top();
		tasks_.pop();

		return std::move(ptr);
	}
	
	return std::nullopt;
}

bool Scheduler::empty(){
	
	std::lock_guard lock(mtx_);
	
	return tasks_.empty();
}

void Scheduler::notifyAll(){

	cv_.notify_all();
}