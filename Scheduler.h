
#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <thread>
#include <condition_variable>
#include <queue>
#include <functional>
#include <memory>
#include <optional>
#include <mutex>
#include "Task.h"

struct TaskCompare{

	bool operator() (const std::shared_ptr<Task>& lhs, const std::shared_ptr<Task>& rhs) const{

		auto lhsTime = lhs->getNextExecuteTime();
		auto rhsTime = rhs->getNextExecuteTime();
		if( lhsTime != rhsTime ) return lhsTime > rhsTime;

		if( lhs->getPriority() != rhs->getPriority() ) return lhs->getPriority() > rhs->getPriority();
		return lhs->getId() > rhs->getId();
	}

};

class Scheduler{

public:
	using TaskQueue = std::priority_queue<std::shared_ptr<Task>, std::vector<std::shared_ptr<Task>>, TaskCompare>;
	
private:
	TaskQueue tasks_;
	std::mutex mtx_;
	std::condition_variable cv_;
	const size_t max_size_;
	bool stop_;
	std::atomic<int> exitNumber_{0};
	ErrorHandler errorHandler_;
	
public:
	Scheduler(size_t size, bool stop = false);
	~Scheduler();
	
	void push(std::shared_ptr<Task> task);
	std::optional<std::shared_ptr<Task>> pop();
	bool empty();
	void notifyAll();
	void setCounter(int num){ exitNumber_.store(num); }

	size_t getTaskQueueSize(){ 

		std::lock_guard lock(mtx_);
		return tasks_.size(); 
	}  

	ErrorHandler& getErrorHandler(){ return errorHandler_; }

};


#endif